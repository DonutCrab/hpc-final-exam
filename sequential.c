#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "qr.c"
#include "image.c"
#include "util.c"

// cc sequential.c -o out/sequential -I/opt/homebrew/include -L/opt/homebrew/lib -lpng
// ./out/sequential

int main(int argc, char *argv[])
{
    printf("Steps: %s\n", argv[1]);
    printf("Size: %s\n", argv[2]);

    float k = 0.3;
    int steps = atoi(argv[1]);

    int size = atoi(argv[2]) * 33 + 2;
    float input_matrix[size][size];

    int pixel_multiplier = size / qr_size;
    int offset = (size - (pixel_multiplier * qr_size)) / 2;

    // fill input_matrix
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
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

    // START
    clock_t start_time = clock();

    float cache_row[size];
    float cache_value;

    for (int time = 0; time < steps; time++)
    {
        copy_row(input_matrix[0], cache_row, size);

        for (int row = 1; row < size - 1; row++)
        {
            cache_value = input_matrix[row][0];
            float new_row[size];

            copy_row(input_matrix[row], new_row, size);

            for (int col = 1; col < size - 1; col++)
            {
                float new_y = k * cache_row[col] + 2 * (1 - k) * input_matrix[row][col] + k * input_matrix[row + 1][col];
                float new_x = k * cache_value + 2 * (1 - k) * input_matrix[row][col] + k * input_matrix[row][col + 1];
                float new_value = (new_x + new_y) / 4;

                cache_value = input_matrix[row][col];
                new_row[col] = new_value;
            }

            copy_row(input_matrix[row], cache_row, size);
            copy_row(new_row, input_matrix[row], size);
        }
    }

    // END
    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nExecution time: %f seconds\n", time_taken);

    export_image("sequential_end", size, input_matrix);

    return 0;
}
