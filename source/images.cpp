#include "images.hpp"


// https://github.com/Universal-Team/Universal-Updater/blob/015120a88218f430ac65466f93e352f5d66908bf/source/utils/screenshot.cpp#L31-L91
C2D_Image EmojiFromDownload(downloaded PNGdata) {
    std::vector<u8> buffer = {};
    for (uint i = 0; i < PNGdata.size; i++) {
        buffer.push_back(PNGdata.response[i]);
    }
	std::vector<u8> ImageBuffer;
	unsigned width, height;
	C2D_Image img;
	lodepng::decode(ImageBuffer, width, height, buffer);

	img.tex = new C3D_Tex;
	img.subtex = new Tex3DS_SubTexture({(u16)width, (u16)height, 0.0f, 1.0f, width / 512.0f, 1.0f - (height / 512.0f)});

	C3D_TexInit(img.tex, 512, 512, GPU_RGBA8);
	C3D_TexSetFilter(img.tex, GPU_NEAREST, GPU_NEAREST);
	img.tex->border = 0xFFFFFFFF;
	C3D_TexSetWrap(img.tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);

	for (u32 x = 0; x < width && x < 512; x++) {
		for (u32 y = 0; y < height && y < 512; y++) {
			const u32 dstPos = ((((y >> 3) * (512 >> 3) + (x >> 3)) << 6) +
								((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) |
								((x & 4) << 2) | ((y & 4) << 3))) * 4;

			const u32 srcPos = (y * width + x) * 4;
			((uint8_t *)img.tex->data)[dstPos + 0] = ImageBuffer.data()[srcPos + 3];
			((uint8_t *)img.tex->data)[dstPos + 1] = ImageBuffer.data()[srcPos + 2];
			((uint8_t *)img.tex->data)[dstPos + 2] = ImageBuffer.data()[srcPos + 1];
			((uint8_t *)img.tex->data)[dstPos + 3] = ImageBuffer.data()[srcPos + 0];
		}
	}

	return img;
}