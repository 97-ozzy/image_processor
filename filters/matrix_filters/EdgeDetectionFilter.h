#ifndef CPP_HSE_EDGEDETECTIONFILTER_H
#define CPP_HSE_EDGEDETECTIONFILTER_H

#include "MatrixFilter.h"
#include "../GrayscaleFilter.h"

class EdgeDetectionFilter : public MatrixFilter {
public:
    explicit EdgeDetectionFilter(double threshold) : MatrixFilter(EDGE_DETECTION_KERNEL), threshold_(threshold) {
    }

    Bmp& Apply(Bmp& pic) const override;

    ~EdgeDetectionFilter() override = default;

private:
    inline static const std::vector<double> EDGE_DETECTION_KERNEL = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    double threshold_;
};

#endif  // CPP_HSE_EDGEDETECTIONFILTER_H
