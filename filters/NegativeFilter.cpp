#include "NegativeFilter.h"

Bmp& NegativeFilter::Apply(Bmp& pic) const {
    const int width = pic.GetWidth();
    const int height = pic.GetHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel& pixel = pic.GetPixel(x, y);
            auto [r, g, b] = pixel;
            pixel.SetRGB(MaxBrightness - r, MaxBrightness - g, MaxBrightness - b);
        }
    }
    return pic;
}