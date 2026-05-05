#include <iostream>

#include "CropFilter.h"

Bmp& CropFilter::Apply(Bmp& pic) const {
    const int old_width = pic.GetWidth();
    const int old_height = pic.GetHeight();

    const int new_width = std::min(width_, old_width);
    const int new_height = std::min(height_, old_height);

    if (width_ > old_width || height_ > old_height) {
        std::wcerr << "\nWARNING: Requested crop dimensions exceed image size.\n";
        std::wcerr << "   Original: " << old_width << "x" << old_height << "\n";
        std::wcerr << "   Requested: " << width_ << "x" << height_ << "\n";
        std::wcerr << "   Image will be cropped to: " << new_width << "x" << new_height << "\n\n";
    }

    Bmp new_bmp;
    new_bmp.Create(new_width, new_height);
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            Pixel& src_pixel = pic.GetPixel(x, y);
            Pixel& dst_pixel = new_bmp.GetPixel(x, y);
            dst_pixel = src_pixel;
        }
    }

    pic = std::move(new_bmp);
    return pic;
}