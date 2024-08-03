#pragma once
#include <iostream>
using std::string;
#include <vector>
using std::vector;
#include <citro2d.h>
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#include "downloadedstruct.hpp"
#include "crc.hpp"
// #include "pngstruct.hpp"
#include "lodepng.hpp"

class PNGimage {
public:
    unsigned int width;
    unsigned int height;
    vector<unsigned char> data;
    vector<u32> pixels;
    vector<uint> getPixIndex(uint x, uint y, uint w, uint h);
private:
};

int parsePNG(downloaded PNGdata, PNGimage &image, bool debugMode);
int PNGtoImage(PNGimage png, C2D_Image *image);
