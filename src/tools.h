#ifndef TOOLZ
#define TOOLZ
#include <MatrixMath.h>
class point
{
public:
    mtx_type poz[3] = {0};
    int color = 0;
};

void project(mtx_type *a, float dist);

void rotX(mtx_type *a, float ang);
void rotY(mtx_type *a, float ang);
void rotZ(mtx_type *a, float ang);

void transform(mtx_type *a, float x, float y, float z);

// int zsort(const void *a, const void *b);
void initzbuff();
void draw(uint8_t *buffer, mtx_type *a, int color);
#endif