#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iostream>

#include "MatrixFilter.h"

MatrixFilter::MatrixFilter(const std::vector<double>& kernel) : kernel_(kernel) {
    if (kernel_.empty()) {
        throw std::invalid_argument("Kernel cannot be empty");
    }

    size_t size = static_cast<int>(std::sqrt(kernel_.size()));
    if (size * size != kernel_.size()) {
        throw std::invalid_argument("Kernel must be square");
    }

    if (size % 2 == 0) {
        throw std::invalid_argument("Kernel size must be odd");
    }

    radius_ = static_cast<int>(size / 2);
}

Bmp& MatrixFilter::Apply(Bmp& pic) const {
    const int width = pic.GetWidth();
    const int height = pic.GetHeight();

    Bmp old = pic;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            double r = 0.0;
            double g = 0.0;
            double b = 0.0;
            int k = 0;

            for (int dy = -radius_; dy <= radius_; ++dy) {
                const int sy = ClampIndex(y + dy, height);

                for (int dx = -radius_; dx <= radius_; ++dx) {
                    const int sx = ClampIndex(x + dx, width);

                    const Pixel& p = old.GetPixel(sx, sy);
                    const double w = kernel_[k++];

                    r += p.r * w;
                    g += p.g * w;
                    b += p.b * w;
                }
            }

            Pixel& dst = pic.GetPixel(x, y);
            dst.SetRGB(std::clamp(r, 0.0, 1.0), std::clamp(g, 0.0, 1.0), std::clamp(b, 0.0, 1.0));
        }
    }
    return pic;
}