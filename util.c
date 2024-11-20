// TODO: Util generally a bad pattern

void copy_row(const float *from, float *to, int width)
{
    for (int i = 0; i < width; i++)
    {
        to[i] = from[i];
    }
}
