#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

#include "input.c"
#include "output.c"
#include "row_copy.c"

// mpicc parallel.c -o out/parallel -I/opt/homebrew/include -L/opt/homebrew/lib -lpng
// mpirun -np 4 ./out/parallel 20 4

int main(int argc, char *argv[])
{
    int mpi_rank, mpi_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    if (mpi_rank == 0)
    {
        printf("Steps: %s\n", argv[1]);
        printf("Size: %s\n", argv[2]);
    }

    float k = 0.3;
    int steps = atoi(argv[1]);

    int matrix_size = input_matrix_size(atoi(argv[2]));
    float input_matrix[matrix_size][matrix_size];

    fill_matrix(matrix_size, input_matrix);

    // START
    clock_t start_time = clock();

    int rows_per_process = (matrix_size - 2) / mpi_size;

    int start_row = mpi_rank * rows_per_process + 1;
    int end_row = (mpi_rank + 1) * rows_per_process;

    // compute
    for (int time = 0; time < steps; time++)
    {
        float cache_value;

        for (int row = start_row; row <= end_row; row++)
        {
            cache_value = input_matrix[row][0];

            for (int col = 1; col < matrix_size - 1; col++)
            {
                float new_y = k * input_matrix[row - 1][col] + 2 * (1 - k) * input_matrix[row][col] + k * input_matrix[row + 1][col];
                float new_x = k * input_matrix[row][col - 1] + 2 * (1 - k) * input_matrix[row][col] + k * input_matrix[row][col + 1];
                float new_value = (new_x + new_y) / 4;

                cache_value = input_matrix[row][col];
                input_matrix[row][col] = new_value;
            }
        }

        // exchange top boundary
        if (mpi_rank > 0)
        {
            MPI_Send(input_matrix[start_row], matrix_size, MPI_FLOAT, mpi_rank - 1, 1, MPI_COMM_WORLD);
            MPI_Recv(input_matrix[start_row - 1], matrix_size, MPI_FLOAT, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // exchange bottom boundary
        if (mpi_rank < mpi_size - 1)
        {
            MPI_Send(input_matrix[end_row], matrix_size, MPI_FLOAT, mpi_rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(input_matrix[end_row + 1], matrix_size, MPI_FLOAT, mpi_rank + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    // collect
    if (mpi_rank != 0)
    {
        for (int row = start_row; row <= end_row; row++)
        {
            MPI_Send(input_matrix[row], matrix_size, MPI_FLOAT, 0, row, MPI_COMM_WORLD);
        }
    }
    else
    {
        // process 0
        for (int row = start_row; row < end_row; row++)
        {
            copy_row(input_matrix[row], input_matrix[row], matrix_size);
        }

        // other processes
        for (int process = 1; process < mpi_size; process++)
        {
            int target_start_row = process * rows_per_process + 1;
            int target_end_row = (process + 1) * rows_per_process;

            for (int row = target_start_row; row <= target_end_row; row++)
            {
                MPI_Recv(input_matrix[row], matrix_size, MPI_FLOAT, process, row, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        // END
        clock_t end_time = clock();
        double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        printf("\nExecution time: %f seconds\n", time_taken);

        // export
        export_image("parallel_end", matrix_size, input_matrix);
    }

    MPI_Finalize();

    return 0;
}