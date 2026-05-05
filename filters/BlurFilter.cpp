#include "BlurFilter.h"

Bmp& BlurFilter::Apply(Bmp& pic) const {
    const int width = pic.GetWidth();
    const int height = pic.GetHeight();

    Bmp tmp;
    tmp.Create(width, height);

    Bmp* current = &pic;
    Bmp* next = &tmp;

    for (int i = 0; i < BlurBoxIterations; ++i) {
        BoxBlurH(*current, *next, width, height, radius_);
        std::swap(current, next);
        BoxBlurV(*current, *next, width, height, radius_);
        std::swap(current, next);
    }

    return pic;
}

void BlurFilter::BoxBlurH(const Bmp& src, Bmp& dst, int width, int height, int radius) {
    double normalize_coef = 1.0 / (radius + radius + 1);

    for (int y = 0; y < height; ++y) {
        Pixel fv = src.GetPixel(0, y);
        Pixel lv = src.GetPixel(width - 1, y);

        double sum_r = (radius + 1) * fv.r;
        double sum_g = (radius + 1) * fv.g;
        double sum_b = (radius + 1) * fv.b;

        for (int j = 0; j < radius; ++j) {
            Pixel p = src.GetPixel(j, y);
            sum_r += p.r;
            sum_g += p.g;
            sum_b += p.b;
        }

        // Обработка левого края
        int ri = radius;
        for (int j = 0; j <= radius; ++j) {
            if (ri < width) {
                Pixel p_right = src.GetPixel(ri, y);
                sum_r += p_right.r;
                sum_g += p_right.g;
                sum_b += p_right.b;
            }
            sum_r -= fv.r;
            sum_g -= fv.g;
            sum_b -= fv.b;

            Pixel result;
            result.SetRGB(sum_r * normalize_coef, sum_g * normalize_coef, sum_b * normalize_coef);
            dst.GetPixel(j, y) = result;

            ri++;
        }

        // Обработка центра
        int li = 0;
        for (int j = radius + 1; j < width - radius; ++j) {
            Pixel p_right = src.GetPixel(ri, y);
            sum_r += p_right.r;
            sum_g += p_right.g;
            sum_b += p_right.b;

            Pixel p_left = src.GetPixel(li, y);
            sum_r -= p_left.r;
            sum_g -= p_left.g;
            sum_b -= p_left.b;

            Pixel result;
            result.SetRGB(sum_r * normalize_coef, sum_g * normalize_coef, sum_b * normalize_coef);
            dst.GetPixel(j, y) = result;

            li++;
            ri++;
        }

        // Обработка правого края
        for (int j = width - radius; j < width; ++j) {
            sum_r += lv.r;
            sum_g += lv.g;
            sum_b += lv.b;

            Pixel p_left = src.GetPixel(li, y);
            sum_r -= p_left.r;
            sum_g -= p_left.g;
            sum_b -= p_left.b;

            Pixel result;
            result.SetRGB(sum_r * normalize_coef, sum_g * normalize_coef, sum_b * normalize_coef);
            dst.GetPixel(j, y) = result;

            li++;
        }
    }
}

void BlurFilter::BoxBlurV(const Bmp& src, Bmp& dst, int width, int height, int radius) {
    double normalize_coef = 1.0 / (radius + radius + 1);

    for (int x = 0; x < width; ++x) {
        Pixel fv = src.GetPixel(x, 0);
        Pixel lv = src.GetPixel(x, height - 1);

        double sum_r = (radius + 1) * fv.r;
        double sum_g = (radius + 1) * fv.g;
        double sum_b = (radius + 1) * fv.b;

        for (int j = 0; j < radius; ++j) {
            Pixel p = src.GetPixel(x, j);
            sum_r += p.r;
            sum_g += p.g;
            sum_b += p.b;
        }

        // Обработка низа
        int ti = radius;
        for (int j = 0; j <= radius; ++j) {
            if (ti < height) {
                Pixel p_top = src.GetPixel(x, ti);
                sum_r += p_top.r;
                sum_g += p_top.g;
                sum_b += p_top.b;
            }
            sum_r -= fv.r;
            sum_g -= fv.g;
            sum_b -= fv.b;

            Pixel result;
            result.SetRGB(sum_r * normalize_coef, sum_g * normalize_coef, sum_b * normalize_coef);
            dst.GetPixel(x, j) = result;

            ti++;
        }

        // Обработка центра
        int bi = 0;
        for (int j = radius + 1; j < height - radius; ++j) {
            Pixel p_top = src.GetPixel(x, ti);
            sum_r += p_top.r;
            sum_g += p_top.g;
            sum_b += p_top.b;

            Pixel p_bottom = src.GetPixel(x, bi);
            sum_r -= p_bottom.r;
            sum_g -= p_bottom.g;
            sum_b -= p_bottom.b;

            Pixel result;
            result.SetRGB(sum_r * normalize_coef, sum_g * normalize_coef, sum_b * normalize_coef);
            dst.GetPixel(x, j) = result;

            bi++;
            ti++;
        }

        // Обработка правого края
        for (int j = height - radius; j < height; ++j) {
            sum_r += lv.r;
            sum_g += lv.g;
            sum_b += lv.b;

            Pixel p_bottom = src.GetPixel(x, bi);
            sum_r -= p_bottom.r;
            sum_g -= p_bottom.g;
            sum_b -= p_bottom.b;

            Pixel result;
            result.SetRGB(sum_r * normalize_coef, sum_g * normalize_coef, sum_b * normalize_coef);
            dst.GetPixel(x, j) = result;

            bi++;
        }
    }
}