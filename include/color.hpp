#ifndef COLOR
#define COLOR
#include <stdint.h>
#define map(x, x1, x2, y1, y2) (((y2 - y1) / (x2 - x1)) * (x - x1) + y1)
#define floor(a) ((int64_t)(a))
#define mod(a, b) ((a) - ((b) * floor((a) / (b))))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define abs(a) ((a) < (0) ? -(a) : (a))

void cap(uint8_t &val, uint8_t max);
void wrapAngle(int16_t &ang);
uint8_t *getRGB(uint32_t rgb);
void getRGB(uint32_t rgb, uint8_t &r, uint8_t &g, uint8_t &b);
uint8_t *getRGBA(uint32_t rgba);
void getRGBA(uint32_t rgba, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);
uint32_t getRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t getRGB(uint8_t r, uint8_t g, uint8_t b);
uint32_t getHSV(int16_t h, uint8_t s, uint8_t v);

#endif