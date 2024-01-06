#define SFML_STATIC

#include <frame.hpp>

Frame::Frame() : Frame("")
{
}

Frame::Frame(string name) : Frame(name, DEFAULT_WIDTH, DEFAULT_HEIGHT)
{
}

Color Frame::getPixel(double x, double y) {
    return Color::White;
}

// primary
Frame::Frame(string name, const uint32_t WIDTH, const uint32_t HEIGHT)
{

    window = Window(new RenderWindow(VideoMode(WIDTH, HEIGHT), name));

    // window->setActive(true);

    width = window->getSize().x;
    height = window->getSize().y;
    pixels = new VertexArray(Points, width * height);
    Vector2f scaleFactor((float) WIDTH / width, (float) HEIGHT / height);

    // Platform and device initialization (Run once at the start)
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Create context and command queue (Run once at the start)
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueue(context, device, 0, NULL);

    // Load and compile kernel (Run once at the start)
    const char* kernelSource =
    "__kernel void gpu_kernel(__global int* input, __global int* output, int size) {\n"
    "    int tid = get_global_id(0);\n"
    "    if (tid < size) {\n"
    "        output[tid] = input[tid] * 2;\n"
    "    }\n"
    "}\n";
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, kernelSource, NULL);
    
    typedef struct {

    } ConstantData;

    ConstantData constantData;
    // Allocate constant memory (Run once at the start)
    d_constants = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(ConstantData), &constantData, NULL);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_constants);


    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x) {
            
            pixels->operator[](y * width + x).position = Vector2f(x * scaleFactor.x, y * scaleFactor.y);
        }
    }

    stopWatch1 = new StopWatch("frame rate");
    // stopWatch2 = new StopWatch("setColor");

    timer = new Timer(21, [this, WIDTH, HEIGHT]()
                      {
        Event event;
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window->close();
                return 0;
            }else if(event.type == Event::Resized) {
                width = event.size.width;
                height = event.size.height;
                pixels = new VertexArray(Points, width * height);
                Vector2f scaleFactor((float) WIDTH / width, (float) HEIGHT / height);

                for (uint32_t y = 0; y < height; ++y)
                {
                    for (uint32_t x = 0; x < width; ++x) {
                        pixels->operator[](y * width + x).position = Vector2f(x * scaleFactor.x, y * scaleFactor.y);
                    }
                }
            }
        }

        if(pixels == nullptr)
            return -1;

        stopWatch1->start();

        int size = 0;
        const void *h_array[size];

        // Allocate and copy dynamic memory (Run multiple times)
        d_array = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(int), NULL, NULL);
        clEnqueueWriteBuffer(queue, d_array, CL_TRUE, 0, size * sizeof(int), h_array, 0, NULL, NULL);
        
        // Set kernel arguments and execute (Run multiple times)
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_array);
        clSetKernelArg(kernel, 1, sizeof(int), &size);

        size_t globalSize = size;
        clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

        // Copy data back (if needed) (Run multiple times)
        clEnqueueReadBuffer(queue, d_array, CL_TRUE, 0, size * sizeof(int), h_array, 0, NULL, NULL);
        
        for (uint32_t y = 0; y < height; ++y)
        {
            for (uint32_t x = 0; x < width; ++x)
            {
                pixels->operator[](y * width + x).color = getPixel(x, y);
            }
        }

        window->clear();

        window->draw(*pixels);

        window->display();
        
        stopWatch1->stop();

        cout << "fps: " << (1000 / stopWatch1->getAverage()) << "    \r";

        return -1; });
}

Frame::~Frame()
{
    delete timer;
    if(stopWatch1 != nullptr) {
        delete stopWatch1;
    }
    if(stopWatch2 != nullptr)
    {
        delete stopWatch2;
    }
    // Release resources (Run after the program closes)
    clReleaseMemObject(d_constants);
    clReleaseMemObject(d_array);

    clReleaseKernel(kernel);
    clReleaseProgram(program);

    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

uint32_t Frame::getWidth()
{
    return width;
}

uint32_t Frame::getHeight()
{
    return height;
}

float map(float x, float x1, float x2, float y1, float y2)
{
    return ((y2 - y1) / (x2 - x1)) * (x - x1) + y1;
}