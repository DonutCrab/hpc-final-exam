#include "output.h"
#include <png.h>

void export_image(char *file, int size, float matrix[size][size])
{
    char name[strlen(file) + 9];
    sprintf(name, "out/%s.png", file);

    FILE *fp = fopen(name, "wb");
    png_structp png_out = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_out = png_create_info_struct(png_out);

    png_init_io(png_out, fp);
    png_set_IHDR(png_out, info_out, size, size, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_out, info_out);

    png_bytep row = (png_bytep)malloc(1 * size);
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            row[y] = (png_byte)(matrix[x][y] * 255);
        }
        png_write_row(png_out, row);
    }

    png_write_end(png_out, NULL);
    free(row);
    png_destroy_write_struct(&png_out, &info_out);
    fclose(fp);
}
