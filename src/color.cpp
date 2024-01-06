#include <color.hpp>

void cap(uint8_t &val, uint8_t max)
{
    val = min(max(val, 0), max);
}

void wrapAngle(int16_t &ang)
{
    while (ang > 360)
        ang -= 360;
    while (ang < 0)
        ang += 360;
}

uint8_t *getRGB(uint32_t rgb)
{
    uint8_t *output;
    output = new uint8_t[3];
    output[0] = rgb >> 24;
    output[1] = rgb >> 16;
    output[2] = rgb >> 8;
    return output;
}

uint8_t *getRGBA(uint32_t rgba)
{
    uint8_t *output;
    output = new uint8_t[4];
    output[0] = rgba >> 24;
    output[1] = rgba >> 16;
    output[2] = rgba >> 8;
    output[3] = rgba;
    return output;
}

void getRGB(uint32_t rgb, uint8_t &r, uint8_t &g, uint8_t &b)
{
    r = (rgb >> 24);
    g = (rgb >> 16);
    b = (rgb >> 8);
}

void getRGBA(uint32_t rgba, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t& a)
{
    r = (rgba >> 24);
    g = (rgba >> 16);
    b = (rgba >> 8);
    a = (rgba);
}

uint32_t getRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((r << 24) + (g << 16) + (b << 8) + a);
}

uint32_t getRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return getRGBA(r, g, b, 0xff);
}

uint32_t getHSV(int16_t h, uint8_t s, uint8_t v)
{
    wrapAngle(h);
    cap(s, 100);
    cap(v, 100);

    uint8_t r, g, b;

    r = (17 * v * (s * (max(min(abs(mod(h + 360, 360) - 180) - 60, 60), 0) - 60) + 6000)) / 40000;
    g = (17 * v * (s * (max(min(abs(mod(h + 240, 360) - 180) - 60, 60), 0) - 60) + 6000)) / 40000;
    b = (17 * v * (s * (max(min(abs(mod(h + 120, 360) - 180) - 60, 60), 0) - 60) + 6000)) / 40000;
    // std::cout << "hsv(" << std::to_string(h) << ", " << std::to_string(s) << ", " << std::to_string(v) << ") = rgb(" << std::to_string(r) << ", " << std::to_string(g) << ", " << std::to_string(b) << ")        \r";
    // Sleep(10);

    return getRGB(r, g, b);
}