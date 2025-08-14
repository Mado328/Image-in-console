#ifndef UNTITLED_IMAGE_H
#define UNTITLED_IMAGE_H

#include <cstdint>
#include <vector>

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t fileType{};
    uint32_t fileSize{};
    uint16_t reserved1{};
    uint16_t reserved2{};
    uint32_t offsetData{};
};
struct CIEXYZ { int32_t X, Y, Z; };
struct BMPInfoHeader {
    uint32_t size{};                // Размер структуры (40, 108 или 124)
    int32_t width{};                // Ширина изображения (в пикселях)
    int32_t height{};               // Высота (может быть отрицательной для top-down)
    uint16_t planes{1};            // Должно быть 1
    uint16_t bitCount{};            // Бит на пиксель (1, 4, 8, 16, 24, 32)
    uint32_t compression{};         // Тип сжатия (0 = BI_RGB, 1 = BI_RLE8, ...)
    uint32_t imageSize{};           // Размер данных пикселей (может быть 0 для BI_RGB)
    int32_t xPixelsPerMeter{};      // Горизонтальное разрешение (пикселей/метр)
    int32_t yPixelsPerMeter{};      // Вертикальное разрешение
    uint32_t colorsUsed{};          // Количество используемых цветов (0 = все)
    uint32_t colorsImportant{};     // Количество важных цветов (0 = все)

    // Только для V4/V5 (BITMAPV4HEADER / BITMAPV5HEADER)
    uint32_t redMask{};             // Маска красного канала (для BI_BITFIELDS)
    uint32_t greenMask{};           // Маска зелёного канала
    uint32_t blueMask{};            // Маска синего канала
    uint32_t alphaMask{};           // Маска альфа-канала
    uint32_t colorSpaceType{};      // Тип цветового пространства (LCS_sRGB = 0x73524742)

    // CIEXYZTRIPLE (9 × int32_t = 36 байт)
    CIEXYZ colorSpaceEndpoints[3]{}; // Точки цветового пространства

    uint32_t gammaRed{};            // Гамма для красного (фиксированная точка 16.16)
    uint32_t gammaGreen{};          // Гамма для зелёного
    uint32_t gammaBlue{};           // Гамма для синего

    // Только для V5
    uint32_t intent{};              // Rendering intent (LCS_GM_ABS_COLORIMETRIC и др.)
    uint32_t profileData{};         // Смещение до ICC-профиля
    uint32_t profileSize{};         // Размер ICC-профиля
    uint32_t reserved{};            // Должно быть 0
};

#pragma pack(pop)
struct Color {
    float r, g, b;

    Color();
    Color(float r, float g, float b);
    ~Color() = default;
};

class Image {
public:
    Image(int width, int height);
    Image();
    ~Image() = default;

    [[nodiscard]] Color GetColor(int x, int y) const;
    void SetColor(const Color& color, int x, int y);

    void Export(const char * path) const;
    void Read(const char* path);
    void WriteBMP() const;
    void convertToBlackAndWhite();

private:
    int m_width;
    int m_height;
    std::vector<Color> m_colors;
    BMPFileHeader fileHeader;
    BMPInfoHeader fileInfoHeader;
};


#endif //UNTITLED_IMAGE_H


