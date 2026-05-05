#ifndef CPP_HSE_FISHEYEFILTER_H
#define CPP_HSE_FISHEYEFILTER_H

#include <cmath>
#include <algorithm>

#include "Filter.h"

class FisheyeFilter : public Filter {
public:
    explicit FisheyeFilter(double strength) {
        strength_ = std::sqrt(std::clamp(strength - StrengthAdj, 0.0, 1 - StrengthAdj));
    };

    Bmp& Apply(Bmp& pic) const override;

    ~FisheyeFilter() override = default;

private:
    double strength_;
    static constexpr double StrengthAdj = 0.001;
    static constexpr double TakeHalf = 0.5;

    static void MakeCircle(Bmp& pic, double radius);
};

#endif  // CPP_HSE_FISHEYEFILTER_H
