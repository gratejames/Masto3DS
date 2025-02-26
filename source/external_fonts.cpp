#include "external_fonts.hpp"

struct fontDataItem {
    string name;
    u32 start;
    u32 end;
    C2D_SpriteSheet fontSheet;
    C2D_Image *glyphs;
    bool newScaling;
};

vector<fontDataItem> fontData = {
    {
        "basic_latin",
        0x000,
        0x07F,
        0,
        new C2D_Image[0x07F - 0x000],
        false,
    },
    {
        "latin_1_supplement",
        0x0A0,
        0x0FF,
        0,
        new C2D_Image[0x0FF - 0x0A0],
        false,
    },
    {
        "greek_and_coptic",
        0x370,
        0x3FF,
        0,
        new C2D_Image[0x3FF - 0x370],
        true,
    },
    {
        "cyrillic",
        0x400,
        0x4FF,
        0,
        new C2D_Image[0x4FF - 0x400],
        false,
    },
    {
        "arabic",
        0x600,
        0x6FF,
        0,
        new C2D_Image[0x6FF - 0x600],
        false,
    },
    // {
    //     "builtin", // General Punctuation
    //     0x2000,
    //     0x206F,
    //     0,
    //     0,
    //     false,
    // },
    {
        "general_punctuation",
        0x2000,
        0x206F,
        0,
        new C2D_Image[0x206F - 0x2000],
        true,
    },
    {
        "miscellaneous_symbols",
        0x2600,
        0x26FF,
        0,
        new C2D_Image[0x26FF - 0x2600],
        false,
    },
    {
        "builtin", // CJK Symbols and Punctuation
        0x3000,
        0x303F,
        0,
        0,
        false,
    },
    {
        "builtin", // Hiragana
        0x3040,
        0x309F,
        0,
        0,
        false,
    },
    {
        "builtin", // Katakana
        0x30A0,
        0x30FF,
        0,
        0,
        false,
    },
    {
        "builtin", // CJK Unified Ideographs
        0x4e00,
        0x9FFF,
        0,
        0,
        false,
    },
    {
        "builtin",
        0xFF00,
        0xFFEF,
        0,
        0,
        false,
    },
    // {
    //     "halfwidth_and_fullwidth_forms",
    //     0xFF00,
    //     0xFFEF,
    //     0,
    //     new C2D_Image[0xFFEF - 0xFF00],
    // },
    {
        "enclosed_alphanumeric_supplement",
        0x1F100,
        0x1F1FF,
        0,
        new C2D_Image[0x1F1FF - 0x1F100],
        true,
    },
    {
        "emoticons",
        0x1F600,
        0x1F64F,
        0,
        new C2D_Image[0x1F64F - 0x1F600],
        true,
    },
    {
        "miscellaneous_symbols_and_pictographs",
        0x1F300,
        0x1F5FF,
        0,
        new C2D_Image[0x1F5FF - 0x1F300],
        false,
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
        if (font.name == "builtin")
            continue;
        string fileName = "romfs:/fonts/" + font.name + "_font.t3x";
        C2D_SpriteSheet fontSheet = C2D_SpriteSheetLoad(fileName.c_str());
        if (fontSheet == NULL) {
            std::cout << "BadUnicode failed to load fontsheet: " << font.name << std::endl;
            continue;
        }
        uint num_of_images = C2D_SpriteSheetCount(fontSheet);
        uint len = font.end - font.start;
        if (num_of_images-1 != len && num_of_images != len) {
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
    if (ef == -2) {
        C2D_TextBuf textBuf = C2D_TextBufNew(5);
        C2D_Text c2text;
        C2D_TextParse(&c2text, textBuf, text.c_str());
        C2D_TextOptimize(&c2text);
        C2D_TextGetDimensions(&c2text, scale*0.5, scale*0.5, &t_width, &t_height);
        C2D_TextBufDelete(textBuf);
        return;
    } else if (ef == -1) {
        t_width = 0;
        t_height = 0;
    } else {
        fontDataItem font = fontData[ef];
        C2D_Image img = font.glyphs[codepoint - font.start];
        if (img.tex == 0 || img.subtex == 0)
            return;
        if (font.newScaling)
            scale *= 0.8;
        t_width = img.subtex->width * scale;
        t_height = img.subtex->height * scale;
    }
}

void draw_efont(string text, float x, float y, float scale, u32 color) {
    u32 codepoint = utf8_to_codepoint(text);
    int ef = efonts_font(text);
    if (ef == -2) {
        C2D_TextBuf textBuf = C2D_TextBufNew(5);
        C2D_Text c2text;
        C2D_TextParse(&c2text, textBuf, text.c_str());
        C2D_TextOptimize(&c2text);
        C2D_DrawText(&c2text, C2D_WithColor, x, y+2, 0, scale*0.5, scale*0.5, color);
        C2D_TextBufDelete(textBuf);
    } else if (ef == -1) {
        std::cout << "BadUnicode unkcp: " << codepoint << std::endl;
    } else {
        fontDataItem font = fontData[ef];
        C2D_Image img = font.glyphs[codepoint - font.start];
        if (img.tex == 0 || img.subtex == 0)
            return;
        if (font.newScaling)
            scale *= 0.8;
        float t_width = img.subtex->width * scale;
        float t_height = img.subtex->height * scale;
        draw_texture(img, x, y, t_width, t_height, color);
    }
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
            if (font.name == "builtin")
                return -2;
            C2D_Image img = font.glyphs[codepoint - font.start];
            if (img.tex == 0 || img.subtex == 0)
                return -2;
            return i;
        }
    }
    return -1;
}
