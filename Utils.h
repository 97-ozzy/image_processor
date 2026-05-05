#ifndef CPP_HSE_UTILS_H
#define CPP_HSE_UTILS_H

#include <string>
#include <iostream>

class Utils {
public:
    static void PrintHelp() {
        std::cout << "Image Processor - Apply filters to BMP images\n";
        std::cout << "================================================\n";
        std::cout << "Usage: <input.bmp> <output.bmp> [filters...]\n\n";

        std::cout << "Available filters:\n";
        std::cout << "  -crop <width> <height>      Crop image to specified dimensions\n";
        std::cout << "  -gs                         Convert to grayscale\n";
        std::cout << "  -neg                        Apply negative effect\n";
        std::cout << "  -sharp                      Sharpen image\n";
        std::cout << "  -edge <threshold>           Edge detection with threshold (0-1)\n";
        std::cout << "  -blur <sigma>               Gaussian blur with given sigma\n";
        std::cout << "  -fisheye <strength>         Fisheye effect with strength\n\n";

        std::cout << "Example:\n";
        std::cout << "input.bmp output.bmp -crop 800 600 -gs -blur 0.5\n";
    }
    static void PrintError() {
        std::cerr << "Error: Invalid command line arguments" << std::endl;
        std::cerr << "Usage: <input.bmp> <output.bmp> [-filter1 params] [-filter2 params] ..." << std::endl;
    }
};

#endif  // CPP_HSE_UTILS_H
