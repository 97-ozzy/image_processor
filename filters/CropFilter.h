#ifndef CPP_HSE_CROPFILTER_H
#define CPP_HSE_CROPFILTER_H

#include "Filter.h"

class CropFilter : public Filter {
public:
    CropFilter(int width, int height) : width_(width), height_(height) {  // с левого верхнего
    }

    Bmp& Apply(Bmp& pic) const override;

    ~CropFilter() override = default;

private:
    int width_;
    int height_;
};

#endif  // CPP_HSE_CROPFILTER_H