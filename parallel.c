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

    int process_cache_size = rows_per_process + 2;
    float process_cache[process_cache_size][matrix_size];

    for (int i = 0; i < process_cache_size; i++)
    {
        copy_row(input_matrix[i + mpi_rank * rows_per_process], process_cache[i], matrix_size);
    }

    // compute
    for (int time = 0; time < steps; time++)
    {
        float cache_value;

        for (int row = 1; row < process_cache_size - 1; row++)
        {
            cache_value = process_cache[row][0];

            for (int col = 1; col < matrix_size - 1; col++)
            {
                float new_y = k * process_cache[row - 1][col] + 2 * (1 - k) * process_cache[row][col] + k * process_cache[row + 1][col];
                float new_x = k * process_cache[row][col - 1] + 2 * (1 - k) * process_cache[row][col] + k * process_cache[row][col + 1];
                float new_value = (new_x + new_y) / 4;

                cache_value = process_cache[row][col];
                process_cache[row][col] = new_value;
            }
        }

        // exchange top boundary
        if (mpi_rank > 0)
        {
            MPI_Send(process_cache[1], matrix_size, MPI_FLOAT, mpi_rank - 1, 1, MPI_COMM_WORLD);
            MPI_Recv(process_cache[0], matrix_size, MPI_FLOAT, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // exchange bottom boundary
        if (mpi_rank < mpi_size - 1)
        {
            MPI_Send(process_cache[process_cache_size - 2], matrix_size, MPI_FLOAT, mpi_rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(process_cache[process_cache_size - 1], matrix_size, MPI_FLOAT, mpi_rank + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    // collect
    if (mpi_rank != 0)
    {
        for (int row = 1; row < process_cache_size - 1; row++)
        {
            MPI_Send(process_cache[row], matrix_size, MPI_FLOAT, 0, mpi_rank + row, MPI_COMM_WORLD);
        }
    }
    else
    {
        // process 0
        for (int row = 1; row < process_cache_size - 1; row++)
        {
            copy_row(process_cache[row], input_matrix[row], matrix_size);
        }

        // other processes
        for (int process = 1; process < mpi_size; process++)
        {
            for (int row = 1; row < process_cache_size - 1; row++)
            {
                float finalize_cache[matrix_size];

                MPI_Recv(finalize_cache, matrix_size, MPI_FLOAT, process, process + row, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                copy_row(finalize_cache, input_matrix[process * rows_per_process + row], matrix_size);
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