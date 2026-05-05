//
// Created by Huawei on 10-Mar-26.
//

#ifndef CPP_HSE_PIPELINE_H
#define CPP_HSE_PIPELINE_H

#include <vector>

#include "filters/Filter.h"

class Pipeline {  // правильное копирование либо запретить, чтоб не было shallow copy
public:
    Pipeline() = default;

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    Pipeline(Pipeline&& other) noexcept : filters_(std::move(other.filters_)) {
    }
    Pipeline& operator=(Pipeline&& other) noexcept {
        if (this != &other) {
            Clear();
            filters_ = std::move(other.filters_);
        }
        return *this;
    }

    ~Pipeline() {
        this->Clear();
    }

    void Add(Filter* filter) {
        if (filter) {
            filters_.push_back(filter);
        }
    }
    const std::vector<Filter*>& GetFilters() const {
        return filters_;
    }

    void Apply(Bmp& pic) const;

    bool IsEmpty() const {
        return filters_.empty();
    }
    void Clear();

protected:
    std::vector<Filter*> filters_;
};

#endif  // CPP_HSE_PIPELINE_H
