#define SFML_STATIC

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>

#include <perlin.hpp>
#include <frame.hpp>

using std::cout;

#define WIDTH 1280
#define HEIGHT 720

double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

class MyFrame : public Frame {
    public:
        MyFrame(string name, const uint32_t width, const uint32_t height) : Frame(name, width, height) {};

        Color getPixel(double x, double y) {
            return Color::White;
        }
};

int ct = 0;
int main()
{
    const int checkerSize = 100;
    MyFrame *frame = new MyFrame((string) "SFML", WIDTH, HEIGHT);

    Timer end(1000, [frame]() {
        ct++;
        if(ct < 10) {
            return -1;
        }
        cout << "fps: " << (1000 / frame->stopWatch1->getAverage()) << "    \n";
        return 0;
    });

    int returnCode = runAll();
    delete frame;
    return returnCode;
}