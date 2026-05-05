#include <catch.hpp>
#include <cstdio>
#include <fstream>
#include <stdexcept>

#include "../Bmp.h"

// Вспомогательная функция для создания временного файла
static std::string TempFileName() {
    static int counter = 0;
    return "test_" + std::to_string(counter++) + ".bmp";
}

// Запись произвольных данных во временный файл
template <size_t N>
static void WriteFile(const std::string& filename, const std::array<uint8_t, N>& data) {
    std::ofstream f(filename, std::ios::binary);
    f.write(reinterpret_cast<const char*>(data.data()), data.size());
    f.close();
}

static std::vector<uint8_t> ReadFile(const std::string& filename) {
    std::ifstream f(filename, std::ios::binary | std::ios::ate);
    if (!f.is_open()) {
        throw std::runtime_error("Cannot open file");
    }
    std::streamsize size = f.tellg();
    f.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(size);
    if (!f.read(reinterpret_cast<char*>(data.data()), size)) {
        throw std::runtime_error("Cannot read file");
    }
    return data;
}

TEST_CASE("Bmp: create and access pixels") {
    Bmp img;
    img.Create(3, 4);
    REQUIRE(img.GetWidth() == 3);
    REQUIRE(img.GetHeight() == 4);

    img.GetPixel(0, 0) = Pixel{1.0, 0.0, 0.0};
    img.GetPixel(2, 3) = Pixel{0.0, 1.0, 0.0};

    Pixel p = img.GetPixel(0, 0);
    REQUIRE(p.r == Approx(1.0));
    REQUIRE(p.g == Approx(0.0));
    REQUIRE(p.b == Approx(0.0));

    p = img.GetPixel(2, 3);
    REQUIRE(p.r == Approx(0.0));
    REQUIRE(p.g == Approx(1.0));
    REQUIRE(p.b == Approx(0.0));
}

TEST_CASE("Bmp: out of bounds access") {
    Bmp img;
    img.Create(2, 2);
    REQUIRE_THROWS_AS(img.GetPixel(-1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(img.GetPixel(2, 0), std::out_of_range);
    REQUIRE_THROWS_AS(img.GetPixel(0, -1), std::out_of_range);
    REQUIRE_THROWS_AS(img.GetPixel(0, 2), std::out_of_range);
}

TEST_CASE("Bmp: copy and move") {
    Bmp img1;
    img1.Create(2, 2);
    img1.GetPixel(0, 0) = Pixel{1.0, 0.0, 0.0};

    Bmp img2(img1);
    REQUIRE(img2.GetWidth() == 2);
    REQUIRE(img2.GetHeight() == 2);
    REQUIRE(img2.GetPixel(0, 0).r == Approx(1.0));

    Bmp img3;
    img3 = img1;
    REQUIRE(img3.GetPixel(0, 0).r == Approx(1.0));

    Bmp img4(std::move(img1));
    REQUIRE(img4.GetPixel(0, 0).r == Approx(1.0));
}

// ----- Тесты загрузки корректных BMP -----

TEST_CASE("Bmp: load 3x2 blue-blue-red image") {
    constexpr int Width = 3;
    constexpr int Height = 2;
    // обрезал flag.bmp и скопировал hex
    constexpr int BasePicLen = 78;
    constexpr std::array<uint8_t, BasePicLen> BASEPIC = {
        0x42, 0x4D, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
        0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB, 0x00, 0x00, 0xBB, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
        0x00, 0x00, 0xBB, 0x00, 0x00, 0xBB, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};
    std::string filename = TempFileName();
    WriteFile(filename, BASEPIC);

    Bmp img;
    img.Load(filename);

    REQUIRE(img.GetWidth() == Width);
    REQUIRE(img.GetHeight() == Height);

    constexpr double BluePixelVal = 187.0 / 255.0;

    const Pixel& top_left = img.GetPixel(0, 0);
    REQUIRE(top_left.r == Approx(0.0));
    REQUIRE(top_left.g == Approx(0.0));
    REQUIRE(top_left.b == Approx(BluePixelVal));

    const Pixel& top_mid = img.GetPixel(1, 0);
    REQUIRE(top_mid.b == Approx(BluePixelVal));

    const Pixel& top_right = img.GetPixel(Width - 1, 0);
    REQUIRE(top_right.r == Approx(1.0));
    REQUIRE(top_right.g == Approx(0.0));
    REQUIRE(top_right.b == Approx(0.0));

    const Pixel& bottom_left = img.GetPixel(0, 1);
    REQUIRE(bottom_left.b == Approx(BluePixelVal));

    const Pixel& bottom_mid = img.GetPixel(1, 1);
    REQUIRE(bottom_mid.b == Approx(BluePixelVal));

    const Pixel& bottom_right = img.GetPixel(Width - 1, 1);
    REQUIRE(bottom_right.r == Approx(1.0));

    std::remove(filename.c_str());
}

// Тест: создаём изображение, сохраняем, проверяем байты с эталоном
TEST_CASE("Bmp: save and verify file content against expected bytes") {
    constexpr int Width = 2;
    constexpr int Height = 2;
    Bmp img;
    img.Create(Width, Height);
    img.GetPixel(0, 0) = Pixel{1.0, 0.0, 0.0};
    img.GetPixel(1, 0) = Pixel{0.0, 1.0, 0.0};
    img.GetPixel(0, 1) = Pixel{0.0, 0.0, 1.0};
    img.GetPixel(1, 1) = Pixel{1.0, 1.0, 1.0};

    std::string filename = TempFileName();
    img.Save(filename);

    std::vector<uint8_t> actual = ReadFile(filename);
    constexpr int ExpectedLen = 70;
    constexpr std::array<uint8_t, ExpectedLen> EXPECTED = {
        // File header (14 bytes)
        0x42, 0x4D,              // Signature "BM"
        0x46, 0x00, 0x00, 0x00,  // File size = 70 (0x46)
        0x00, 0x00,              // Reserved1 = 0
        0x00, 0x00,              // Reserved2 = 0
        0x36, 0x00, 0x00, 0x00,  // Offset to pixel data = 54 (0x36)

        // Info header (40 bytes)
        0x28, 0x00, 0x00, 0x00,  // Header size = 40
        0x02, 0x00, 0x00, 0x00,  // Width = 2
        0x02, 0x00, 0x00, 0x00,  // Height = 2
        0x01, 0x00,              // Planes = 1
        0x18, 0x00,              // Bits per pixel = 24
        0x00, 0x00, 0x00, 0x00,  // Compression = BI_RGB (0)
        0x10, 0x00, 0x00, 0x00,  // Image size = 16 bytes (data)
        0x00, 0x00, 0x00, 0x00,  // XPixelsPerMeter = 0
        0x00, 0x00, 0x00, 0x00,  // YPixelsPerMeter = 0
        0x00, 0x00, 0x00, 0x00,  // ClrUsed = 0
        0x00, 0x00, 0x00, 0x00,  // ClrImportant = 0

        // Pixel data (bottom-up, with padding)
        // Bottom row (y=1):
        // Pixel (0,1): синий (B=255, G=0, R=0) -> BGR: 0xFF, 0x00, 0x00
        0xFF, 0x00, 0x00,
        // Pixel (1,1): белый (R=255, G=255, B=255) -> BGR: 0xFF, 0xFF, 0xFF
        0xFF, 0xFF, 0xFF,
        // Padding: 2 bytes (width_bytes = 2*3 = 6, padding = (4 - (6%4)) % 4 = 2)
        0x00, 0x00,

        // Top row (y=0):
        // Pixel (0,0): красный (R=255, G=0, B=0) -> BGR: 0x00, 0x00, 0xFF
        0x00, 0x00, 0xFF,
        // Pixel (1,0): зелёный (R=0, G=255, B=0) -> BGR: 0x00, 0xFF, 0x00
        0x00, 0xFF, 0x00,
        // Padding: 2 bytes
        0x00, 0x00};

    REQUIRE(actual.size() == EXPECTED.size());

    for (size_t i = 0; i < actual.size(); ++i) {
        INFO("Byte " << i << ": expected " << std::hex << static_cast<int>(EXPECTED[i]) << ", got "
                     << static_cast<int>(actual[i]));
        REQUIRE(actual[i] == EXPECTED[i]);
    }

    std::remove(filename.c_str());
}