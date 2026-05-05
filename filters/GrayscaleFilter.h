#ifndef CPP_HSE_GRAYSCALEFILTER_H
#define CPP_HSE_GRAYSCALEFILTER_H

#include "Filter.h"

class GrayscaleFilter : public Filter {
public:
    Bmp& Apply(Bmp& pic) const override;

    ~GrayscaleFilter() override = default;

private:
    constexpr static double RGsCoef = 0.299;
    constexpr static double GGsCoef = 0.587;
    constexpr static double BGsCoef = 0.114;
};

#endif  // CPP_HSE_GRAYSCALEFILTER_H