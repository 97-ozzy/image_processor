#ifndef CPP_HSE_FILTERFACTORY_H
#define CPP_HSE_FILTERFACTORY_H

#include <unordered_map>
#include <functional>

#include "Pipeline.h"
#include "CmdArgsParser.h"

class FilterFactory {
public:
    FilterFactory();

    FilterFactory(const FilterFactory&) = delete;
    FilterFactory& operator=(const FilterFactory&) = delete;

    Pipeline* CreatePipeline(const std::vector<FilterDescriptor>& filters);

private:
    static Filter* CreateFisheyeFilter(const FilterDescriptor& descriptor);
    static Filter* CreateCropFilter(const FilterDescriptor& descriptor);
    static Filter* CreateGrayscaleFilter(const FilterDescriptor& descriptor);
    static Filter* CreateBlurFilter(const FilterDescriptor& descriptor);
    static Filter* CreateNegativeFilter(const FilterDescriptor& descriptor);
    static Filter* CreateSharpeningFilter(const FilterDescriptor& descriptor);
    static Filter* CreateEdgeDetectionFilter(const FilterDescriptor& descriptor);
    static int ParseIntParam(const std::string& param, const std::string& filter_name);
    static double ParseDoubleParam(const std::string& param, const std::string& filter_name);

    std::unordered_map<std::string, std::function<Filter*(const FilterDescriptor&)>> map_;
};

#endif  // CPP_HSE_FILTERFACTORY_H
