#include "images.hpp"

// https://github.com/devkitPro/citro2d/issues/16
void createImage(C2D_Image* image) {
	u32 w=32, h=32;
	C3D_Tex *tex = new C3D_Tex;
	if (!C3D_TexInit(tex,w,h,GPU_RGBA8))
        return;

	u32 s = w * h;
    u32 *b = (u32*)linearAlloc(s * 4);
    u8 n=0;
	for (u32 ofs=0; ofs<s; ofs++) {
        b[ofs] = C2D_Color32(255,n,n,n);
        n++;
    }

	C3D_TexUpload(tex,b);
    Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture;
	subtex->left = subtex->top = 0;
	subtex->right = subtex->bottom = 1;
    subtex->width = w;
    subtex->height = h;
    image->tex = tex;
    image->subtex = subtex;
}

vector<uint> PNGimage::getPixIndex(uint x, uint y, uint w, uint h) {
    vector<uint> out {};
    for (uint col = 0; col < h; col++) {
        for (uint row = 0; row < w; row++) {
    // for (uint col = h; col > 0; col--) {
    //     for (uint row = w; row > 0; row--) {
            out.push_back((x + row) + (y + col)*width);
            // out.push_back((x + row) + (y + col)*width);
        }
    }
	return out;
}

uint genSwizzleNumbers(uint n, bool genX) {
	uint bitPos = 0;
	uint x = 0;
	while (n != 0) {
		x += (n%2) << ((2 * bitPos) + genX);
		n = n >> 1;
		bitPos += 1;
    }
	return x;
}

int parsePNG(downloaded PNGdata, PNGimage &image, bool debugMode) {
    for (uint i = 0; i < PNGdata.size; i++) {
        image.data.push_back(PNGdata.response[i]);
    }

    vector<unsigned char> out;

    int errorCode = lodepng::decode(out, image.width, image.height, image.data);
    if (errorCode != 0) {
        return errorCode;
    }
    for (uint i = 0; i < out.size(); i+=1) {
        if (debugMode) {
            // HAHAHAHA DESWIZZLE IT'S BEEN DAYS HELP ME
            uint x = 0, y = 0;
            uint bitPos = 0;
            uint i2 = i;
            while ( i2 != 0) {
                if (bitPos%2 == 0)
                    // Even bits
                    y += (i2%2) << (bitPos/2);
                else
                    // Odd bits
                    x += (i2%2) << (bitPos/2);
                bitPos++;
                i2 = i2 >> 1;
            }
                
            x = image.width - 1 - x;
            uint finalIndex = (y*image.width + x)*4;
            image.pixels.push_back(C2D_Color32(out[finalIndex+3],out[finalIndex+2],out[finalIndex+1],out[finalIndex+0]));
        } else {
            image.pixels.push_back(C2D_Color32(out[i*4+3],out[i*4+2],out[i*4+1],out[i*4+0]));
        }
    }
    return 0;
}

int PNGtoImage(PNGimage png, C2D_Image *image) {
    std::cout << "W" << png.width << " H" << png.height << " S" << png.pixels.size() << std::endl;
    
	C3D_Tex *tex = new C3D_Tex;
	if (!C3D_TexInit(tex, png.width, png.height,GPU_RGBA8))
        return -1;

	u32 area = png.width * png.height;
    u32 *byteData = (u32*)linearAlloc(area * 4);
    u8 n=0;
	for (u32 i=0; i<area; i++) {
        byteData[i] = png.pixels[i];//C2D_Color32(png.pixels[i*4+3],png.pixels[i*4+2],png.pixels[i*4+1],png.pixels[i*4+0]);
        n++;
    }

	C3D_TexUpload(tex,byteData);
    Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture;
	subtex->left = subtex->top = 0;
	subtex->right = subtex->bottom = 1;
    subtex->width = png.width;//2;
    subtex->height = png.height;//2;
    image->tex = tex;
    image->subtex = subtex;
    return 0;
}