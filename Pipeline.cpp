#include "Pipeline.h"

void Pipeline::Apply(Bmp& pic) const {
    for (Filter* filter : filters_) {
        if (filter) {  // nullptr exc
            filter->Apply(pic);
        }
    }
}
void Pipeline::Clear() {
    for (Filter* filter : filters_) {
        delete filter;
    }
    filters_.clear();
}