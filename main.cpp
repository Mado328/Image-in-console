#include <iostream>

#include "Image.h"


int main() {
    std::string path;
    std::cout << "Enter the path to the file\n";
    std::cin >> path;
    Image image(0, 0);
    image.Read(path.c_str());
    image.Export("image1.bmp");
    image.WriteBMP();

    return 0;
}