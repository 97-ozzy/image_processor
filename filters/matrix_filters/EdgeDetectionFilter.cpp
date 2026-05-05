#include "EdgeDetectionFilter.h"

Bmp& EdgeDetectionFilter::Apply(Bmp& pic) const {

    GrayscaleFilter gs;
    gs.Apply(pic);

    MatrixFilter::Apply(pic);

    const int width = pic.GetWidth();
    const int height = pic.GetHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            Pixel& pixel = pic.GetPixel(x, y);

            if (pixel.r > threshold_) {
                pixel.SetRGB(1.0, 1.0, 1.0);
            } else {
                pixel.SetRGB(0.0, 0.0, 0.0);
            }
        }
    }
    return pic;
}