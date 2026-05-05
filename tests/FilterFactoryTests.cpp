#include <catch.hpp>
#include <stdexcept>

#include "FilterFactory.h"
#include "Pipeline.h"

static FilterDescriptor MakeDescriptor(const std::string& name, std::initializer_list<std::string> params) {
    return {name, std::vector<std::string>(params)};
}

TEST_CASE("FilterFactory: create pipeline with no filters") {
    FilterFactory factory;
    std::vector<FilterDescriptor> empty;
    std::unique_ptr<Pipeline> pipeline(factory.CreatePipeline(empty));
    REQUIRE(pipeline != nullptr);
    REQUIRE(pipeline->GetFilters().empty());
}

TEST_CASE("FilterFactory: create pipeline with valid filters") {
    FilterFactory factory;
    constexpr size_t FiltersCount = 7;
    std::vector<FilterDescriptor> descrs = {MakeDescriptor("crop", {"100", "200"}),
                                            MakeDescriptor("gs", {}),
                                            MakeDescriptor("blur", {"0.5"}),
                                            MakeDescriptor("neg", {}),
                                            MakeDescriptor("sharp", {}),
                                            MakeDescriptor("edge", {"0.3"}),
                                            MakeDescriptor("fisheye", {"0.5"})};

    std::unique_ptr<Pipeline> pipeline(factory.CreatePipeline(descrs));
    REQUIRE(pipeline != nullptr);
    REQUIRE(pipeline->GetFilters().size() == FiltersCount);
}

TEST_CASE("FilterFactory: unknown filter throws") {
    FilterFactory factory;
    std::vector<FilterDescriptor> descrs = {MakeDescriptor("unknown", {})};
    REQUIRE_THROWS_AS(factory.CreatePipeline(descrs), std::runtime_error);
}

// ---------- Crop filter tests ----------
TEST_CASE("FilterFactory: crop filter with valid params") {
    FilterFactory factory;
    auto desc = MakeDescriptor("crop", {"800", "600"});
    std::unique_ptr<Pipeline> pipeline(factory.CreatePipeline({desc}));
    REQUIRE(pipeline->GetFilters().size() == 1);
}

TEST_CASE("FilterFactory: crop filter with wrong number of params") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("crop", {"800"})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("crop", {"800", "600", "700"})}), std::runtime_error);
}

TEST_CASE("FilterFactory: crop filter with non-positive dimensions") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("crop", {"0", "600"})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("crop", {"-100", "600"})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("crop", {"800", "0"})}), std::runtime_error);
}

TEST_CASE("FilterFactory: crop filter with invalid integer params") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("crop", {"abc", "600"})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("crop", {"800", "12.5"})}), std::runtime_error);
}

// ---------- Grayscale filter tests ----------
TEST_CASE("FilterFactory: grayscale filter with no params") {
    FilterFactory factory;
    std::unique_ptr<Pipeline> pipeline(factory.CreatePipeline({MakeDescriptor("gs", {})}));
    REQUIRE(pipeline->GetFilters().size() == 1);
}

TEST_CASE("FilterFactory: grayscale filter rejects params") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("gs", {"0"})}), std::runtime_error);
}

// ---------- Blur filter tests ----------
TEST_CASE("FilterFactory: blur filter with valid sigma") {
    FilterFactory factory;
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("blur", {"0.5"})}));
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("blur", {"0"})}));
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("blur", {"2.5"})}));
}

TEST_CASE("FilterFactory: blur filter with wrong param count") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("blur", {})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("blur", {"0.5", "1.0"})}), std::runtime_error);
}

TEST_CASE("FilterFactory: blur filter with negative sigma") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("blur", {"-0.1"})}), std::runtime_error);
}

TEST_CASE("FilterFactory: blur filter with invalid sigma string") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("blur", {"abc"})}), std::runtime_error);
}

// ---------- Negative filter tests ----------
TEST_CASE("FilterFactory: negative filter with no params") {
    FilterFactory factory;
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("neg", {})}));
}

TEST_CASE("FilterFactory: negative filter rejects params") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("neg", {"0"})}), std::runtime_error);
}

// ---------- Sharpening filter tests ----------
TEST_CASE("FilterFactory: sharpening filter with no params") {
    FilterFactory factory;
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("sharp", {})}));
}

TEST_CASE("FilterFactory: sharpening filter rejects params") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("sharp", {"0.5"})}), std::runtime_error);
}

// ---------- Edge Detection filter tests ----------
TEST_CASE("FilterFactory: edge detection filter with valid threshold") {
    FilterFactory factory;
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("edge", {"0.1"})}));
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("edge", {"0.5"})}));
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("edge", {"0.99"})}));
}

TEST_CASE("FilterFactory: edge detection filter with wrong param count") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("edge", {})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("edge", {"0.5", "0.6"})}), std::runtime_error);
}

TEST_CASE("FilterFactory: edge detection filter with out-of-range threshold") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("edge", {"0.0"})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("edge", {"1.0"})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("edge", {"-0.1"})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("edge", {"1.1"})}), std::runtime_error);
}

TEST_CASE("FilterFactory: edge detection filter with invalid threshold string") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("edge", {"abc"})}), std::runtime_error);
}

// ---------- Fisheye filter tests ----------
TEST_CASE("FilterFactory: fisheye filter with valid strength") {
    FilterFactory factory;
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("fisheye", {"0.5"})}));
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("fisheye", {"0.0"})}));
    REQUIRE_NOTHROW(factory.CreatePipeline({MakeDescriptor("fisheye", {"1.0"})}));
}

TEST_CASE("FilterFactory: fisheye filter with wrong param count") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("fisheye", {})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("fisheye", {"0.5", "0.6"})}), std::runtime_error);
}

TEST_CASE("FilterFactory: fisheye filter with out-of-range strength") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("fisheye", {"-0.1"})}), std::runtime_error);
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("fisheye", {"1.1"})}), std::runtime_error);
}

TEST_CASE("FilterFactory: fisheye filter with invalid strength string") {
    FilterFactory factory;
    REQUIRE_THROWS_AS(factory.CreatePipeline({MakeDescriptor("fisheye", {"abc"})}), std::runtime_error);
}