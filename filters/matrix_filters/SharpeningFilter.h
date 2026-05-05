#ifndef CPP_HSE_SHARPFILTER_H
#define CPP_HSE_SHARPFILTER_H

#include "MatrixFilter.h"

class SharpeningFilter : public MatrixFilter {
public:
    SharpeningFilter() : MatrixFilter(SHARPENING_KERNEL) {
    }

    ~SharpeningFilter() override = default;

private:
    inline static const std::vector<double> SHARPENING_KERNEL = {0, -1, 0, -1, 5, -1, 0, -1, 0};
};

#endif  // CPP_HSE_SHARPFILTER_H