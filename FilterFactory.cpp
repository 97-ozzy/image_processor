#include <stdexcept>
#include <memory>

#include "FilterFactory.h"
#include "filters/CropFilter.h"
#include "filters/GrayscaleFilter.h"
#include "filters/BlurFilter.h"
#include "filters/FisheyeFilter.h"
#include "filters/NegativeFilter.h"
#include "filters/matrix_filters/EdgeDetectionFilter.h"
#include "filters/matrix_filters/SharpeningFilter.h"

FilterFactory::FilterFactory() {
    map_.insert({"crop", CreateCropFilter});
    map_.insert({"gs", CreateGrayscaleFilter});
    map_.insert({"blur", CreateBlurFilter});
    map_.insert({"neg", CreateNegativeFilter});
    map_.insert({"sharp", CreateSharpeningFilter});
    map_.insert({"edge", CreateEdgeDetectionFilter});
    map_.insert({"fisheye", CreateFisheyeFilter});
}

Pipeline* FilterFactory::CreatePipeline(const std::vector<FilterDescriptor>& filters) {
    Pipeline* pipeline = new Pipeline;

    for (const auto& fd : filters) {
        auto it = map_.find(fd.name);  // итератор на найденный элемент
        if (it == map_.end()) {
            throw std::runtime_error("Unknown filter: " +
                                     fd.name);  // в const std::string& передаем std::string (rvalue)
        }

        if (it->second == nullptr) {
            throw std::logic_error("Creator function for filter " + fd.name + " is nullptr");
        }

        Filter* filter = it->second(fd);

        if (filter == nullptr) {
            throw std::logic_error("Filter creator " + fd.name + " returned nullptr");
        }

        pipeline->Add(filter);
    }

    return pipeline;
}
Filter* FilterFactory::CreateFisheyeFilter(const FilterDescriptor& descriptor) {
    if (descriptor.params.size() != 1) {
        throw std::runtime_error("Fisheye filter requires exactly 1 parameter: strength");
    }

    try {
        double strength = ParseDoubleParam(descriptor.params[0], descriptor.name);

        if (strength < 0 || strength > 1) {
            throw std::runtime_error("Strength must be between 0 and 1");
        }
        return new FisheyeFilter(strength);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error in fisheye filter: " + std::string(e.what()));
    }
}

Filter* FilterFactory::CreateNegativeFilter(const FilterDescriptor& descriptor) {
    if (!descriptor.params.empty()) {
        throw std::runtime_error("Negative filter takes no parameters");
    }

    return new NegativeFilter();
}
Filter* FilterFactory::CreateSharpeningFilter(const FilterDescriptor& descriptor) {
    if (!descriptor.params.empty()) {
        throw std::runtime_error("Sharpening filter takes no parameters");
    }

    return new SharpeningFilter();
}
Filter* FilterFactory::CreateEdgeDetectionFilter(const FilterDescriptor& descriptor) {
    if (descriptor.params.size() != 1) {
        throw std::runtime_error("Edge filter requires exactly 1 parameter: threshold");
    }

    try {
        double threshold = ParseDoubleParam(descriptor.params[0], "edge");

        if (threshold <= 0 || threshold >= 1) {
            throw std::runtime_error("Threshold must be between 0 and 1");
        }

        return new EdgeDetectionFilter(threshold);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error in threshold filter: " + std::string(e.what()));
    }
}

Filter* FilterFactory::CreateCropFilter(const FilterDescriptor& descriptor) {
    if (descriptor.params.size() != 2) {
        throw std::runtime_error("Crop filter requires exactly 2 parameters: width and height");
    }

    try {
        int width = ParseIntParam(descriptor.params[0], "crop");
        int height = ParseIntParam(descriptor.params[1], "crop");

        if (width <= 0 || height <= 0) {
            throw std::runtime_error("Crop dimensions must be positive");
        }

        return new CropFilter(width, height);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error in crop filter: " + std::string(e.what()));
    }
}

Filter* FilterFactory::CreateGrayscaleFilter(const FilterDescriptor& descriptor) {
    if (!descriptor.params.empty()) {
        throw std::runtime_error("Grayscale filter takes no parameters");
    }

    return new GrayscaleFilter();
}

Filter* FilterFactory::CreateBlurFilter(const FilterDescriptor& descriptor) {
    if (descriptor.params.size() != 1) {
        throw std::runtime_error("Blur filter requires exactly 1 parameter: sigma");
    }

    try {
        double sigma = ParseDoubleParam(descriptor.params[0], "blur");

        if (sigma < 0) {
            throw std::runtime_error("Sigma must be non-negative");
        }

        return new BlurFilter(sigma);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error in blur filter: " + std::string(e.what()));
    }
}

int FilterFactory::ParseIntParam(const std::string& param, const std::string& filter_name) {
    try {
        size_t pos = 0;
        int result = std::stoi(param, &pos);

        if (pos != param.length()) {
            throw std::runtime_error("Invalid integer format");
        }

        return result;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse integer parameter for " + filter_name + " filter: " + param);
    }
}

double FilterFactory::ParseDoubleParam(const std::string& param, const std::string& filter_name) {
    try {
        size_t pos = 0;
        double result = std::stod(param, &pos);
        if (pos != param.length()) {
            throw std::runtime_error("Invalid double format");
        }

        return result;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse double parameter for " + filter_name + " filter: " + param);
    }
}