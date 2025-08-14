#include <iostream>
#include <fstream>

#include "Image.h"

Color::Color() : r(0), g(0), b(0)
{
}
Color::Color(const float r, const float g, const float b)
    : r(r), g(g), b(b)
{
}
Image::Image(const int width, const int height)
    : m_width(width), m_height(height), m_colors(std::vector<Color>(width * height))
{
}
Image::Image() : m_width(0), m_height(0)
{
}
Color Image::GetColor(const int x, const int y) const{
    return m_colors[y * m_width + x];
}
void Image::SetColor(const Color& color, const int x, const int y) {
    m_colors[y * m_width + x].r = color.r;
    m_colors[y * m_width + x].g = color.g;
    m_colors[y * m_width + x].b = color.b;
}
void Image::Export(const char* path) const {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        std::cout << "File could not be opened\n";
        return;
    }
    constexpr unsigned char bmpPad[3] = { 0, 0, 0 };
    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

    f.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    f.write(reinterpret_cast<const char*>(&fileInfoHeader), sizeof(fileInfoHeader));

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            const unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
            const unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
            const unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);

            const unsigned char color[] = {b, g, r};

            f.write(reinterpret_cast<const char*>(color), 3);
        }
        f.write(reinterpret_cast<const char*>(bmpPad), paddingAmount);
    }
    if (!f.good()) {
        std::cerr << "Error occurred during writing\n";
        return;
    }

    f.close();

    std::cout << "File created\n";
}

void Image::Read(const char* path) {
    std::ifstream f;
    f.open(path, std::ios::binary);
    if (!f.is_open()) {
        std::cout << "File not open\n";
        f.close();
        return;
    }

    f.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    if (fileHeader.fileType != 19778) {
        std::cout << "File is not BMP format\n";
        f.close();
        return;
    }

    f.read(reinterpret_cast<char*>(&fileInfoHeader), sizeof(fileInfoHeader));

    m_width = fileInfoHeader.width;
    m_height = fileInfoHeader.height;

    m_colors.resize(m_width * m_height);

    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {

            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);

            m_colors[y * m_width + x].r = static_cast<float>(color[2]) / 255.0f;
            m_colors[y * m_width + x].g = static_cast<float>(color[1]) / 255.0f;
            m_colors[y * m_width + x].b = static_cast<float>(color[0]) / 255.0f;
        }
        f.ignore(paddingAmount);
    }
    f.close();

    std::cout << "File read\n";
    convertToBlackAndWhite();
}
void Image::convertToBlackAndWhite() {
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                const float blue = m_colors[y * m_width + x].b;
                const float green = m_colors[y * m_width + x].g;
                const float red = m_colors[y * m_width + x].r;

                if (blue > green && blue > red) {
                    m_colors[y * m_width + x].b = blue;
                    m_colors[y * m_width + x].g = blue;
                    m_colors[y * m_width + x].r = blue;
                }
                else if (green > blue && green > red) {
                    m_colors[y * m_width + x].b = green;
                    m_colors[y * m_width + x].g = green;
                    m_colors[y * m_width + x].r = green;
                }
                else {
                    m_colors[y * m_width + x].b = red;
                    m_colors[y * m_width + x].g = red;
                    m_colors[y * m_width + x].r = red;
                }
            }
        }
    };

void Image::WriteBMP() const{
    std::cout << m_width << ":" << m_height << "\n";
    for (int y = m_height - 1; y >= 0; y -= 2) {
        for (int x = 0; x < m_width; ++x) {
            const float blue = m_colors[y * m_width + x].b;
            const float green = m_colors[y * m_width + x].g;
            const float red = m_colors[y * m_width + x].r;

            const double brightness = (red + green + blue) / 3.0;

            if (brightness < 0.25) {
                std::cout << " ";
            }
            else if (brightness < 0.5) {
                std::cout << "*";
            }
            else if (brightness < 0.75) {
                std::cout << "#";
            }
            else {
                std::cout << "@";
            }
        }
        std::cout << std::endl;
    }
}


