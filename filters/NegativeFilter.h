#ifndef CPP_HSE_NEGATIVEFILTER_H
#define CPP_HSE_NEGATIVEFILTER_H

#include "Filter.h"

class NegativeFilter : public Filter {
public:
    Bmp& Apply(Bmp& pic) const override;

    ~NegativeFilter() override = default;

private:
    constexpr static double MaxBrightness = 1.0;
};

#endif  // CPP_HSE_NEGATIVEFILTER_H
