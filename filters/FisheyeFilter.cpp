#include <cmath>

#include "FisheyeFilter.h"

Bmp& FisheyeFilter::Apply(Bmp& pic) const {

    const int width = pic.GetWidth();
    const int height = pic.GetHeight();

    Bmp pic_copy = pic;

    const double cx = width * TakeHalf;
    const double cy = height * TakeHalf;

    const double max_radius = std::min(cx, cy);

    if (strength_ == 0.0) {
        MakeCircle(pic, max_radius);
        return pic;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            double nx = (x - cx) / max_radius;
            double ny = (y - cy) / max_radius;

            double r = std::sqrt(nx * nx + ny * ny);

            if (r == 0) {
                continue;
            }

            double theta = r * (M_PI * TakeHalf);
            double distorted = std::tan(theta * strength_) / std::tan(M_PI * TakeHalf * strength_);

            if (distorted > 1.0) {
                pic.GetPixel(x, y).SetRGB(0, 0, 0);
                continue;
            }

            double scale = distorted / r;

            double sx = nx * scale;
            double sy = ny * scale;

            int src_x = static_cast<int>(cx + sx * max_radius);
            int src_y = static_cast<int>(cy + sy * max_radius);

            if (src_x >= 0 && src_x < width && src_y >= 0 && src_y < height) {
                pic.GetPixel(x, y) = pic_copy.GetPixel(src_x, src_y);
            } else {
                pic.GetPixel(x, y).SetRGB(0, 0, 0);
            }
        }
    }

    MakeCircle(pic, max_radius);
    return pic;
}

void FisheyeFilter::MakeCircle(Bmp& pic, double radius) {
    int height = pic.GetHeight();
    int width = pic.GetWidth();
    double cx = width * TakeHalf;
    double cy = height * TakeHalf;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (radius < std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy))) {
                pic.GetPixel(x, y).SetRGB(0, 0, 0);
            }
        }
    }
}