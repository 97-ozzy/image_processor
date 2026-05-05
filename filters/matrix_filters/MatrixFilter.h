#ifndef CPP_HSE_MATRIXFILTER_H
#define CPP_HSE_MATRIXFILTER_H

#include <vector>

#include "../Filter.h"

class MatrixFilter : public Filter {
public:
    explicit MatrixFilter(const std::vector<double>& kernel);

    Bmp& Apply(Bmp& pic) const override;

    ~MatrixFilter() override = default;

protected:
    int radius_;
    std::vector<double> kernel_;

private:
    int static ClampIndex(int v, int max) {
        if (v < 0) {
            return 0;
        }
        if (v >= max) {
            return max - 1;
        }
        return v;
    }
};

#endif  // CPP_HSE_MATRIXFILTER_H