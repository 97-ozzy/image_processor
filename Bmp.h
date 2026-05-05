#ifndef CPP_HSE_BMP_H
#define CPP_HSE_BMP_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

#pragma pack(push, 1)  // Выравнивание, чтоб убрать padding

// Структура заголовка BMP файла - 14 байт
struct BMPFileHeader {
    static constexpr uint16_t BmpSignature = 0x4D42;  //

    uint16_t Type = BmpSignature;
    uint32_t Size = 0;
    uint16_t Reserved1 = 0;
    uint16_t Reserved2 = 0;
    uint32_t OffBits = 0;
};

// Структура информационного заголовка - 40 байт
struct BMPDeepInfoHeader {
    static constexpr uint32_t HeaderSize = 40;
    static constexpr uint16_t BitsPerPixel = 24;

    uint32_t Size = HeaderSize;
    int32_t Width = 0;
    int32_t Height = 0;
    uint16_t Planes = 1;
    uint16_t BitCount = BitsPerPixel;
    uint32_t Compression = 0;
    uint32_t SizeImage = 0;

    int32_t XPixelsPerMeter = 0;
    int32_t YPixelsPerMeter = 0;
    uint32_t ClrUsed = 0;
    uint32_t ClrImportant = 0;
};

#pragma pack(pop)

#pragma pack(push, 1)
// CIEXYZ structure (3 DWORDs: x, y, z)
struct CIEXYZ {
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z = 0;
};

// CIEXYZTRIPLE structure (3 CIEXYZ: red, green, blue)
struct CIEXYZTRIPLE {
    CIEXYZ red;
    CIEXYZ green;
    CIEXYZ blue;
};

// 124-byte DIB header (BITMAPV5HEADER)
struct BMPV5Header {
    static constexpr uint32_t HeaderSize = 124;  // total size of this structure

    uint32_t Size = HeaderSize;   // must be 124
    int32_t Width = 0;            // image width in pixels
    int32_t Height = 0;           // image height in pixels (positive = bottom-up)
    uint16_t Planes = 1;          // number of planes (always 1)
    uint16_t BitCount = 0;        // bits per pixel (e.g., 1, 4, 8, 16, 24, 32)
    uint32_t Compression = 0;     // compression method
    uint32_t SizeImage = 0;       // size of raw bitmap data (can be 0 for uncompressed)
    int32_t XPixelsPerMeter = 0;  // horizontal resolution (pixels per meter)
    int32_t YPixelsPerMeter = 0;  // vertical resolution (pixels per meter)
    uint32_t ClrUsed = 0;         // number of color indices used
    uint32_t ClrImportant = 0;    // number of important colors

    // V4 / V5 fields
    uint32_t RedMask = 0;    // mask for red channel
    uint32_t GreenMask = 0;  // mask for green channel
    uint32_t BlueMask = 0;   // mask for blue channel
    uint32_t AlphaMask = 0;  // mask for alpha channel

    uint32_t CSType = 0;      // color space type (e.g., LCS_sRGB)
    CIEXYZTRIPLE Endpoints;   // endpoints for the color space (36 bytes)
    uint32_t GammaRed = 0;    // gamma for red channel (scaled)
    uint32_t GammaGreen = 0;  // gamma for green channel
    uint32_t GammaBlue = 0;   // gamma for blue channel

    // V5 specific fields
    uint32_t Intent = 0;       // rendering intent
    uint32_t ProfileData = 0;  // offset to profile data (from start of file)
    uint32_t ProfileSize = 0;  // size of profile data
    uint32_t Reserved = 0;     // reserved, must be 0
};
#pragma pack(pop)

struct Pixel {
    static constexpr double PixelComponentMaxVal = 255.0;
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    void SetRGB(double nr, double ng, double nb) {
        r = nr;
        g = ng;
        b = nb;
    }

    bool operator==(const Pixel& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
    bool operator!=(const Pixel& other) const {
        return !(*this == other);
    }
};

class Bmp {
public:
    Bmp() = default;

    Bmp(const Bmp& other) : file_header_(other.file_header_), deep_header_(other.deep_header_), data_(other.data_) {
    }
    Bmp& operator=(const Bmp& other) {
        if (this != &other) {
            file_header_ = other.file_header_;
            deep_header_ = other.deep_header_;
            data_ = other.data_;
        }
        return *this;
    }

    Bmp(Bmp&& other) noexcept
        : file_header_(std::move(other.file_header_)),
          deep_header_(std::move(other.deep_header_)),
          data_(std::move(other.data_)) {
    }
    Bmp& operator=(Bmp&& other) noexcept {
        if (this != &other) {
            file_header_ = std::move(other.file_header_);
            deep_header_ = std::move(other.deep_header_);
            data_ = std::move(other.data_);
        }
        return *this;
    }

    void Load(const std::string& filename);

    void Save(const std::string& filename) const;

    void Create(int width, int height);

    int GetWidth() const {
        return deep_header_.Width;
    }
    int GetHeight() const {
        return deep_header_.Height;
    }

    bool IsPointInBounds(int x, int y) const {
        return x >= 0 && x < deep_header_.Width && y >= 0 && y < deep_header_.Height;
    }

    Pixel& GetPixel(int x, int y) {
        return data_[GetIndex(x, y)];
    }

    const Pixel& GetPixel(int x, int y) const {
        return data_[GetIndex(x, y)];
    }

private:
    BMPFileHeader file_header_;
    BMPDeepInfoHeader deep_header_;
    std::vector<Pixel> data_;

    int GetRowStride() const {
        int width_bytes = deep_header_.Width * 3;
        int padding = (4 - (width_bytes % 4)) % 4;
        return width_bytes + padding;
    }

    size_t GetIndex(int x, int y) const {
        if (!IsPointInBounds(x, y)) {
            throw std::out_of_range("Pixel coordinates out of bounds");
        }

        return (deep_header_.Height - 1 - y) * deep_header_.Width + x;
    }

    void ReadPaddedData(std::ifstream& file);
    void WritePaddedData(std::ofstream& file) const;
};

#endif  // CPP_HSE_BMP_H