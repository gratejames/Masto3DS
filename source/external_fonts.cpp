#include "external_fonts.hpp"

struct fontDataItem {
    string name;
    u32 start;
    u32 end;
    C2D_SpriteSheet fontSheet;
    C2D_Image *glyphs;
};

vector<fontDataItem> fontData = {
    {
        "basic_latin",
        0x000,
        0x07F,
        0,
        new C2D_Image[0x07F - 0x000],
    },
    {
        "latin_1_supplement",
        0x080,
        // 0x0FF,
        0x0DF,
        0,
        // new C2D_Image[0x0FF - 0x080],
        new C2D_Image[0x0DF - 0x080],
    },
    // {
    //     "ipa_extensions",
    //     0x0250,
    //     0x02AF,
    //     0,
    //     new C2D_Image[0x02AF - 0x0250],
    // },
    // {
    //     "spacing_modifier_letters",
    //     0x02B0,
    //     0x02FF,
    //     0,
    //     new C2D_Image[0x02FF - 0x02B0],
    // },
    // {
    //     "combining_diacritical_marks",
    //     0x0300,
    //     0x036F,
    //     0,
    //     new C2D_Image[0x036F - 0x0300],
    // },
    // {
    //     "greek_and_coptic",
    //     0x0370,
    //     0x03FF,
    //     0,
    //     new C2D_Image[0x03FF - 0x0370],
    // },
    // {
    //     "cyrillic",
    //     0x0400,
    //     0x04FF,
    //     0,
    //     new C2D_Image[0x04FF - 0x0400],
    // },
    // {
    //     "cjk_symbol_and_punctuation",
    //     0x3000,
    //     0x303F,
    //     0,
    //     new C2D_Image[0x303F - 0x3000],
    // },
    {
        "emoticons",
        0x1F600,
        0x1F64F,
        0,
        new C2D_Image[0x1F64F - 0x1F600],
    },
};

u32 utf8_to_codepoint(string utf8) {
    uint len = utf8.size();
    u32 codepoint = 0;

    if (len == 1) {
        codepoint = utf8[0];
    } else if (len == 2) {
        codepoint = ((utf8[0] & 0x1F) << 6) | (utf8[1] & 0x3F);
    } else if (len == 3) {
        codepoint = ((utf8[0] & 0x0F) << 12) | ((utf8[1] & 0x3F) << 6) | (utf8[2] & 0x3F);
    } else if (len == 4) {
        codepoint = ((utf8[0] & 0x07) << 18) | ((utf8[1] & 0x3F) << 12) | ((utf8[2] & 0x3F) << 6) | (utf8[3] & 0x3F);
    } else {
        return 0;
    }
    return codepoint;
}


void load_efonts() {
    for (uint i = 0; i < fontData.size(); i++) {
        fontDataItem font = fontData[i];
        string fileName = "romfs:/fonts/" + font.name + "_font.t3x";
        C2D_SpriteSheet fontSheet = C2D_SpriteSheetLoad(fileName.c_str());
        if (fontSheet == NULL) {
            std::cout << "BadUnicode failed to load fontsheet: " << font.name << std::endl;
            continue;
        }
        uint num_of_images = C2D_SpriteSheetCount(fontSheet);
        uint len = font.end - font.start;
        if (num_of_images-1 != len) {
            std::cout << "Error loading font (" << font.name << "): expected " << len << " glyphs but saw " << num_of_images-1 << std::endl;
            continue;
        }
        for (uint g = 0; g < len; g++) {
            C2D_Image img = C2D_SpriteSheetGetImage(fontSheet, g);
            C3D_TexSetFilter(img.tex, GPU_LINEAR, GPU_LINEAR);
            C3D_TexSetWrap(img.tex, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
            font.glyphs[g] = img;
        }
    }
}

void draw_texture(C2D_Image image, float x, float y, float x_size, float y_size, u32 color) {
	C2D_ImageTint tint;
	C2D_DrawParams c2d_parameter = { {x,y,x_size,y_size}, {0,0}, 0.0f, 0.0f };

	if (!(image.tex == NULL))
	{
        C2D_PlainImageTint(&tint, color, true);
        C2D_DrawImage(image, &c2d_parameter, &tint);
	}
}

void getSize_efont(string text, float scale, float &t_width, float &t_height) {
    u32 codepoint = utf8_to_codepoint(text);
    int ef = efonts_font(text);
    if (ef == -1) {
            C2D_TextBuf textBuf = C2D_TextBufNew(5);
            C2D_Text c2text;
            C2D_TextParse(&c2text, textBuf, text.c_str());
            C2D_TextOptimize(&c2text);
            C2D_TextGetDimensions(&c2text, scale*0.5, scale*0.5, &t_width, &t_height);
        //     C2D_DrawText(&c2text, C2D_WithColor, origin_x + width, origin_y+2, 0, scale*0.5, scale*0.5, color);
        //     width += t_width;
        //     height = std::max(t_height, height);
            C2D_TextBufDelete(textBuf);
        return;
    }
    fontDataItem font = fontData[ef];
    C2D_Image img = font.glyphs[codepoint - font.start];
    t_width = img.subtex->width * scale;
    t_height = img.subtex->height * scale;
}

void draw_efont(string text, float x, float y, float scale, u32 color) {
    u32 codepoint = utf8_to_codepoint(text);
    int ef = efonts_font(text);
    if (ef == -1) {
            C2D_TextBuf textBuf = C2D_TextBufNew(5);
            C2D_Text c2text;
            C2D_TextParse(&c2text, textBuf, text.c_str());
            C2D_TextOptimize(&c2text);
            // C2D_TextGetDimensions(&c2text, scale*0.5, scale*0.5, &t_width, &t_height);
            C2D_DrawText(&c2text, C2D_WithColor, x, y+2, 0, scale*0.5, scale*0.5, color);
            // width += t_width;
            // height = std::max(t_height, height);
            C2D_TextBufDelete(textBuf);


        // int ef = efonts_font(utf8Char);
        // if (ef != -1) {
            // getSize_efont(utf8Char, scale, t_width, t_height);
            // draw_efont(utf8Char, origin_x + width, origin_y, scale, color);
            // width += t_width;
            // height = std::max(t_height, height);
        // } else {
        // }
        return;
    }
    fontDataItem font = fontData[ef];

    C2D_Image img = font.glyphs[codepoint - font.start];

    float t_width = img.subtex->width * scale;
    float t_height = img.subtex->height * scale;

	draw_texture(img, x, y, t_width, t_height, color);
}

// Returns which font the character is in. -1 for default system handler or not found.
int efonts_font(string text) {
    u32 codepoint = utf8_to_codepoint(text);

    if (codepoint == 0) {
        return -1;
    }

    for (uint i = 0; i < fontData.size(); i++) {
        fontDataItem font = fontData[i];
        if (font.start <= codepoint && codepoint < font.end) {
            return i;
        }
    }
    // std::cout << "BadUnicode unkcp: " << codepoint << ", " << text.length() << (int)text[0] << std::endl;
    // std::cout << "BadUnicode unkcp: " << codepoint << std::endl;
    return -1;
}
