#include "GrayscaleFilter.h"

Bmp& GrayscaleFilter::Apply(Bmp& pic) const {
    const int width = pic.GetWidth();
    const int height = pic.GetHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel& pixel = pic.GetPixel(x, y);
            auto [r, g, b] = pixel;
            const double gray = RGsCoef * r + GGsCoef * g + BGsCoef * b;
            pixel.SetRGB(gray, gray, gray);
        }
    }
    return pic;
}