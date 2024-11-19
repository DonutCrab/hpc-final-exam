// TODO: ...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h> // libpng

#include "qr.c"
#include "image.c"

// cc sequential.c -o out/sequential -I/opt/homebrew/include -L/opt/homebrew/lib -lpng && ./out/sequential

void copy_row(float *from, float *to, int width)
{
    for (int i = 0; i < width; i++)
    {
        to[i] = from[i];
    }
}

int main()
{
    float k = 0.3;
    int steps = 1;

    int size = 66;
    float input_array[size][size];

    // float boundary_condition = 0.0;
    // boundary_condition constant?

    int pixel_multiplier = size / qr_size;
    int offset = (size - (pixel_multiplier * qr_size)) / 2;

    // fill input_array
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i < offset || j < offset)
            {
                input_array[i][j] = 0.0;
            }
            else
            {
                int qr_i = (i - offset) / pixel_multiplier;
                int qr_j = (j - offset) / pixel_multiplier;

                if (qr_i >= (qr_size) || qr_j >= (qr_size))
                {
                    input_array[i][j] = 0.0;
                }
                else
                {
                    input_array[i][j] = qr[qr_i][qr_j];
                }
            }
        }
    }

    // print input_array
    // for (int i = 0; i < size; i++)
    // {
    //     for (int j = 0; j < size; j++)
    //     {
    //         printf("%s", input_array[i][j] > 0.5 ? " " : "\u25A0");
    //     }
    //     printf("\n");
    // }

    float cache_row[size];
    float cache_value;

    for (int time = 0; time < steps; time++)
    {
        copy_row(input_array[0], cache_row, size); // cache

        // skip borders -> boundary condition
        for (int row = 1; row < size; row++)
        {
            cache_value = input_array[row][0]; // cache

            // if (row > 0 && row < size)
            // {
            //     printf("calc_row %d\n", row);
            // }

            float new_row[size]; // new

            for (int col = 0; col <= size; col++)
            {
                // skip borders
                if (col > 0 && col < size - 1)
                {
                    float new_x = k * cache_row[col] + 2 * (1 - k) * input_array[row][col] + k * input_array[row + 1][col];
                    float new_y = k * cache_value + 2 * (1 - k) * input_array[row][col] + k * input_array[row][col + 1];
                    float new_value = (new_x + new_y) / 4;

                    cache_value = input_array[row][col]; // cache
                    new_row[col] = new_value;            // overwrite
                }
                else
                {
                    // cache_value = input_array[row][col]; // cache
                }
            }

            copy_row(input_array[row], cache_row, size); // cache
            copy_row(new_row, input_array[row], size);   // overwrite
        }
    }

    export_image("setpss", size, input_array);

    return 0;
}
