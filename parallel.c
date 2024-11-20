#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "qr.c"
#include "image.c"
#include "util.c"

// mpicc parallel.c -o out/parallel -I/opt/homebrew/include -L/opt/homebrew/lib -lpng && mpirun -np 4 ./out/parallel

int main(int argc, char *argv[])
{
    float k = 0.3;
    int steps = 4;

    int matrix_size = 134;
    float input_matrix[matrix_size][matrix_size];

    int pixel_multiplier = matrix_size / qr_size;
    int offset = (matrix_size - (pixel_multiplier * qr_size)) / 2;

    // TODO: extract
    // fill input_matrix
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            if (i < offset || j < offset)
            {
                input_matrix[i][j] = 0.0;
            }
            else
            {
                int qr_i = (i - offset) / pixel_multiplier;
                int qr_j = (j - offset) / pixel_multiplier;

                if (qr_i >= (qr_size) || qr_j >= (qr_size))
                {
                    input_matrix[i][j] = 0.0;
                }
                else
                {
                    input_matrix[i][j] = qr[qr_i][qr_j];
                }
            }
        }
    }

    int mpi_rank, mpi_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    int rows_per_process = (matrix_size - 2) / mpi_size;
    // printf("rows per process %d, %d/%d\n", rows_per_process, rows_per_process * mpi_size, matrix_size);

    float cache_value;

    int process_cache_size = rows_per_process + 2;
    float process_cache[process_cache_size][matrix_size];

    for (int i = 0; i < process_cache_size; i++)
    {
        copy_row(input_matrix[i + mpi_rank], process_cache[i], matrix_size);
    }

    for (int time = 0; time < steps; time++)
    {
        // printf("start step %d on process %d\n", time, mpi_rank);

        for (int row = 1; row < process_cache_size; row++)
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
            MPI_Send(process_cache[0], matrix_size, MPI_FLOAT, mpi_rank - 1, 1, MPI_COMM_WORLD);
            MPI_Recv(process_cache[0], matrix_size, MPI_FLOAT, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // exchange bottom boundary
        if (mpi_rank < mpi_size - 1)
        {
            MPI_Send(process_cache[process_cache_size - 1], matrix_size, MPI_FLOAT, mpi_rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(process_cache[process_cache_size - 1], matrix_size, MPI_FLOAT, mpi_rank + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if (mpi_rank == 0)
    {
        // own content
        for (int i = 1; i < process_cache_size - 1; i++)
        {
            copy_row(process_cache[i], input_matrix[i], matrix_size);
        }

        // other processes
        float finalize_cache[matrix_size];

        for (int process = 1; process < mpi_size; process++)
        {
            for (int row = 1; row < rows_per_process - 2; row++)
            {
                MPI_Recv(finalize_cache, matrix_size, MPI_FLOAT, process, process + row, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                copy_row(finalize_cache, input_matrix[process * rows_per_process + row], matrix_size);
            }
        }

        // export
        export_image("parallel_end", matrix_size, input_matrix);
    }
    else
    {
        for (int row = 1; row < process_cache_size - 1; row++)
        {
            MPI_Send(process_cache[row], matrix_size, MPI_FLOAT, 0, mpi_rank + row, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}