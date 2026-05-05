#ifndef CPP_HSE_BLURFILTER_H
#define CPP_HSE_BLURFILTER_H

#include "Filter.h"

class BlurFilter : public Filter {
public:
    explicit BlurFilter(double sigma) : radius_(static_cast<int>(sigma)) {
    }

    Bmp& Apply(Bmp& pic) const override;

    ~BlurFilter() override = default;

private:
    int radius_;
    static constexpr size_t BlurBoxIterations = 3;

    static void BoxBlurH(const Bmp& src, Bmp& dst, int width, int height, int radius);
    static void BoxBlurV(const Bmp& src, Bmp& dst, int width, int height, int radius);
};

#endif  // CPP_HSE_BLURFILTER_H