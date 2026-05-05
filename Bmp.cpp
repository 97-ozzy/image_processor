//
// Created by Huawei on 10-Mar-26.
//

#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cmath>
#include <iostream>

#include "Bmp.h"

void Bmp::Load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open() || !file.good()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    // reinterpret_cast<char*>(&file_header_) преобразует указатель на структуру BMPFileHeader в указатель
    // на char. При этом само значение указателя (адрес) не меняется, меняется только тип,
    // под которым компилятор интерпретирует память по этому адресу.
    // Так как структура плотно упакована (pack(1)),
    // байты из файла лягут точно в поля структуры в соответствии с порядком их объявления

    file.read(reinterpret_cast<char*>(&file_header_), sizeof(file_header_));
    if (!file.good()) {
        throw std::runtime_error("Failed to read file header");
    }

    if (file_header_.Type != BMPFileHeader::BmpSignature) {
        throw std::runtime_error("Unsupported file type:");
    }

    file.read(reinterpret_cast<char*>(&deep_header_), sizeof(deep_header_));
    if (!file.good()) {
        throw std::runtime_error("Failed to read info header");
    }

    if (deep_header_.Size != BMPDeepInfoHeader::HeaderSize) {
        throw std::runtime_error("Unsupported DIB header size: " + std::to_string(deep_header_.Size) +
                                 " bytes. Only 40-byte BITMAPINFOHEADER is supported.");
    }

    if (deep_header_.Height < 0) {
        throw std::runtime_error("Unsupported file type. Height must be positive. Bottom up storage only.");
    }

    if (deep_header_.BitCount != BMPDeepInfoHeader::BitsPerPixel) {
        throw std::runtime_error("Unsupported file type");
    }

    ReadPaddedData(file);

    file.close();
}

void Bmp::Save(const std::string& filename) const {
    // Создаем родительские директории, если они не существуют
    std::filesystem::path file_path(filename);
    auto parent_path = file_path.parent_path();

    if (!parent_path.empty() && !std::filesystem::exists(parent_path)) {
        std::filesystem::create_directories(parent_path);
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }

    file.write(reinterpret_cast<const char*>(&file_header_), sizeof(file_header_));
    if (!file.good()) {
        throw std::runtime_error("Failed to write file header");
    }

    file.write(reinterpret_cast<const char*>(&deep_header_), sizeof(deep_header_));
    if (!file.good()) {
        throw std::runtime_error("Failed to write info header");
    }

    WritePaddedData(file);

    if (!file.good()) {
        throw std::runtime_error("Failed to write image data");
    }
}

void Bmp::ReadPaddedData(std::ifstream& file) {
    int width = deep_header_.Width;
    int height = deep_header_.Height;
    int stride = GetRowStride();

    data_.resize(width * height);

    std::vector<uint8_t> row(stride);

    constexpr double MaxVal = Pixel::PixelComponentMaxVal;

    for (int y = 0; y < height; ++y) {
        file.read(reinterpret_cast<char*>(row.data()), stride);
        if (!file.good()) {
            throw std::runtime_error("Failed to read image data at row " + std::to_string(y));
        }

        for (int x = 0; x < width; ++x) {
            Pixel& pixel = data_[y * width + x];
            pixel.b = static_cast<double>(row[x * 3]) / MaxVal;
            pixel.g = static_cast<double>(row[x * 3 + 1]) / MaxVal;
            pixel.r = static_cast<double>(row[x * 3 + 2]) / MaxVal;
        }
    }
}

void Bmp::WritePaddedData(std::ofstream& file) const {
    int width = deep_header_.Width;
    int height = std::abs(deep_header_.Height);
    int stride = GetRowStride();
    constexpr double MaxVal = Pixel::PixelComponentMaxVal;

    std::vector<uint8_t> row(stride, 0);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const auto& [r, g, b] = data_[y * width + x];

            row[x * 3] = static_cast<uint8_t>(std::clamp(std::round(b * MaxVal), 0.0, MaxVal));
            row[x * 3 + 1] = static_cast<uint8_t>(std::clamp(std::round(g * MaxVal), 0.0, MaxVal));
            row[x * 3 + 2] = static_cast<uint8_t>(std::clamp(std::round(r * MaxVal), 0.0, MaxVal));
        }

        file.write(reinterpret_cast<const char*>(row.data()), stride);
        if (!file.good()) {
            throw std::runtime_error("Failed to write image data at row " + std::to_string(y));
        }
    }
}

void Bmp::Create(int width, int height) {
    if (width <= 0 || height == 0) {
        throw std::runtime_error("Invalid dimensions for BMP creation");
    }

    deep_header_.Width = width;
    deep_header_.Height = height;

    int stride = GetRowStride();
    deep_header_.SizeImage = stride * height;

    file_header_.OffBits = sizeof(file_header_) + sizeof(deep_header_);
    file_header_.Size = file_header_.OffBits + deep_header_.SizeImage;
    data_.resize(height * width);
}
