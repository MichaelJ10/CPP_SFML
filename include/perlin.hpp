#ifndef PERLIN
#define PERLIN
#include <stdint.h>
#include <math.h>

struct vector2
{
    float x, y;
};

vector2 randomGradient(int ix, int iy);
float dotGridGradient(int ix, int iy, float x, float y);
float interpolate(float a0, float a1, float w);
float perlin(float x, float y);
uint16_t getPerlin(uint16_t x, uint16_t y, uint16_t max);

#endif