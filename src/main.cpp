#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>
#include <signal.h>
#include <math.h>
#include <chrono>
#include <iomanip>
#define WIDTH 800
#define HEIGHT 800

#define map(x, x1, x2, y1, y2) (((y2 - y1) / (x2 - x1)) * (x - x1) + y1)
#define floor(a) ((int64_t)(a))
#define mod(a, b) ((a) - ((b) * floor((a) / (b))))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define abs(a) ((a) < (0) ? -(a) : (a))

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML with OpenGL");
sf::Image image;
uint8_t *pixels;

uint64_t time()
{
    return std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
}

void handeler_function(int sig)
{
    std::cout << std::to_string(window.getSize().x) << ", " << std::to_string(window.getSize().y);
    abort();
}

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

uint8_t *getColor(uint32_t rgba)
{
    uint8_t *output;
    output = new uint8_t[4];
    output[0] = rgba >> 24;
    output[1] = rgba >> 16;
    output[2] = rgba >> 8;
    output[3] = rgba;
    return output;
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

typedef struct
{
    float x, y;
} vector2;

vector2 randomGradient(int ix, int iy)
{
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    vector2 v;
    v.x = sin(random);
    v.y = cos(random);

    return v;
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y)
{
    // Get gradient from integer coordinates
    vector2 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a0, float a1, float w)
{
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

// Sample Perlin noise at coordinates x, y
float perlin(float x, float y)
{

    // Determine grid cell corner coordinates
    int x0 = (int)x;
    int y0 = (int)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // Compute Interpolation weights
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Compute and interpolate top two corners
    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    // Compute and interpolate bottom two corners
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sx);

    // Final step: interpolate between the two previously interpolated values, now in y
    float value = interpolate(ix0, ix1, sy);

    return value;
}

uint16_t getPerlin(uint16_t x, uint16_t y, uint16_t max)
{

    const int GRID_SIZE = 400;

    float val = 0;

    float freq = 1;
    float amp = 1;

    for (int i = 0; i < 12; i++)
    {
        val += perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;

        freq *= 2;
        amp /= 2;
    }

    // Contrast
    val *= 1.2;

    // Clipping
    if (val > 1.0f)
        val = 1.0f;
    else if (val < -1.0f)
        val = -1.0f;

    // Convert 1 to -1 into 255 to 0
    return (uint16_t)(((val + 1.0f) * 0.5f) * max);
}

int main()
{
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t count = 0;
    uint64_t totalTime = 0;

    signal(SIGSEGV, handeler_function);

    // OpenGL initialization
    window.setActive(true);

    while (window.isOpen())
    {
        uint64_t startTime = time();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (width != window.getSize().x || height != window.getSize().y)
        {
            width = window.getSize().x;
            height = window.getSize().y;
        }
        image.create(width, height, sf::Color::Transparent); // Set width and height

        // Get a pointer to the pixel data
        pixels = (uint8_t *)image.getPixelsPtr();
        std::cout << "start     \r";

        // Manipulate pixel data (for example, making everything red)
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                uint32_t index = 4 * (y * width + x);
                uint8_t color = getPerlin(x, y, 255);
                pixels[index + 0] = color;
                pixels[index + 1] = color;
                pixels[index + 2] = color;
                pixels[index + 3] = color;
            }
        }

        pixels = nullptr;

        // Update the texture with the modified pixel data
        sf::Texture texture;
        texture.loadFromImage(image);

        // Create a sprite and set the texture
        sf::Sprite sprite(texture);

        // Draw the sprite to the window
        window.draw(sprite);

        // Display the window contents
        window.display();
        std::cout << "done        \r";
        Sleep(500);
        uint64_t endTime = time();
        uint64_t nextTime = endTime - startTime;
        totalTime += nextTime;
        count++;
        // std::cout << std::setprecision(15) << std::to_string(1000.0 * count / totalTime) << "       \r";
    }

    return 0;
}