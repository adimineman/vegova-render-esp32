#include "tools.h"
#include <MatrixMath.h>

void project(mtx_type *a, float dist)
{
    mtx_type tmp2[3];
    mtx_type tmp[3][3] = {
        {dist / a[2], 0, 0},
        {0, dist / a[2], 0},
        {0, 0, 1}};
    Matrix.Multiply((mtx_type *)tmp, a, 3, 3, 1, (mtx_type *)tmp2);
    Matrix.Copy((mtx_type *)tmp2, 3, 1, a);
}

void rotX(mtx_type *a, float ang)
{
    mtx_type tmp2[3];

    mtx_type tmp[3][3] = {
        {1, 0, 0},
        {0, cos(ang), sin(ang)},
        {0, -sin(ang), cos(ang)}};
    Matrix.Multiply((mtx_type *)tmp, a, 3, 3, 1, (mtx_type *)tmp2);
    Matrix.Copy((mtx_type *)tmp2, 3, 1, a);
}

void rotY(mtx_type *a, float ang)
{
    mtx_type tmp2[3];

    mtx_type tmp[3][3] = {
        {cos(ang), 0, -sin(ang)},
        {0, 1, 0},
        {sin(ang), 0, cos(ang)}};
    Matrix.Multiply((mtx_type *)tmp, a, 3, 3, 1, (mtx_type *)tmp2);
    Matrix.Copy((mtx_type *)tmp2, 3, 1, a);
}

void rotZ(mtx_type *a, float ang)
{
    mtx_type tmp2[3];

    mtx_type tmp[3][3] = {
        {cos(ang), sin(ang), 0},
        {-sin(ang), cos(ang), 0},
        {0, 0, 1}};
    Matrix.Multiply((mtx_type *)tmp, a, 3, 3, 1, (mtx_type *)tmp2);
    Matrix.Copy((mtx_type *)tmp2, 3, 1, a);
}

void transform(mtx_type *a, float x, float y, float z)
{
    mtx_type tmp2[3];

    mtx_type tmp[3] = {x, y, z};
    Matrix.Add((mtx_type *)tmp, a, 1, 3, (mtx_type *)tmp2);
    Matrix.Copy((mtx_type *)tmp2, 3, 1, a);
}

/*int zsort(const void *a, const void *b)
{
    return *(int *)b - *(int *)a;
}*/

#define WIDTH 128
#define HEIGHT 64

float zbuff[WIDTH][HEIGHT] = {0};

void initzbuff()
{
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
            zbuff[i][j] = -3E+38;
}

void draw(uint8_t *buffer, mtx_type *a, int color)
{
    int x = int(a[0] * 32) + 64, y = int(a[1] * 32) + 32;
    if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT) && (a[2] < 0))
    {

        if (color == 2)
            buffer[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
        else
        {
            if (zbuff[x][y] < a[2])
            {
                zbuff[x][y] = a[2];
                switch (color)
                {
                case 1:
                    buffer[x + (y / 8) * WIDTH] |= (1 << (y & 7));
                    break;
                case 0:
                    buffer[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
                    break;
                }
            }
        }
    }
}
