#ifndef FRAME
#define FRAME

#include <SFML/Graphics.hpp>
#include <functional>
#include <windows.h>
#include <iostream>
#include <CL/cl.h>
#include <memory>

// #include "perlin.hpp"
#include "stopWatch.hpp"
#include "frame.hpp"

#include <timer.hpp>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 800

using sf::Event;
using sf::Color;
using sf::Image;
using sf::RenderWindow;
using sf::Sprite;
using sf::Texture;
using sf::VideoMode;
using sf::View;
using std::cout;
using std::string;
using sf::VertexArray;
using sf::Points;
using sf::Vector2u;
using sf::Vector2f;
using std::unique_ptr;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;

typedef unique_ptr<RenderWindow> Window;
typedef function<Color(double, double)> Function;

class Frame {
public:
    Frame();
    Frame(string name);
    Frame(string name, uint32_t width, uint32_t height);
    Frame(Function getPixelsFunction);
    Frame(string name, Function getPixelsFunction);
    Frame(string name, uint32_t width, uint32_t height, Function getPixelsFunction);

    ~Frame();

    uint32_t getWidth();
    uint32_t getHeight();

    

    virtual Color getPixel(double x, double y);

public:

    Timer *timer = nullptr;
    StopWatch *stopWatch1 = nullptr;
    StopWatch *stopWatch2 = nullptr;

    VertexArray *pixels = nullptr;

    Window window = nullptr;

    uint32_t width = 0;
    uint32_t height = 0;

    cl_mem d_array;
    cl_mem d_constants;
    cl_kernel kernel;
    cl_program program;
    cl_command_queue queue;
    cl_context context;
};

#endif