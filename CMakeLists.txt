cmake_minimum_required(VERSION 3.20)

project(image_processor_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(image_processor_lib STATIC
        Pipeline.cpp
        Pipeline.h
        CmdArgsParser.cpp
        CmdArgsParser.h
        filters/Filter.h
        FilterFactory.cpp
        FilterFactory.h
        filters/CropFilter.cpp
        filters/CropFilter.h
        filters/GrayscaleFilter.cpp
        filters/GrayscaleFilter.h
        filters/BlurFilter.cpp
        filters/BlurFilter.h
        Bmp.cpp
        Bmp.h
        filters/NegativeFilter.cpp
        filters/NegativeFilter.h
        filters/matrix_filters/SharpeningFilter.h
        filters/matrix_filters/MatrixFilter.cpp
        filters/matrix_filters/MatrixFilter.h
        filters/matrix_filters/EdgeDetectionFilter.h
        Utils.h
        filters/matrix_filters/EdgeDetectionFilter.cpp
        filters/FisheyeFilter.cpp
        filters/FisheyeFilter.h
)

target_include_directories(image_processor_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(image_processor image_processor.cpp)
target_link_libraries(image_processor PRIVATE image_processor_lib)