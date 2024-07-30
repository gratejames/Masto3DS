#include "ui.hpp"

string getUTF8Char(string text, uint &index) {
    u8 byte1 = text[index];
    if (byte1 < 0b11000000) {
        return text.substr(index, 1);
    }
    if (byte1 < 0b11100000) {
        return text.substr(index, 2);
    }
    if (byte1 < 0b11110000) {
        return text.substr(index, 3);
    }
    return text.substr(index, 4);
}

text::text(string dispText, u32 color, float scale) {
    this->dispText = dispText;
    this->color = color;
    this->scale = scale;
}

void text::Draw(float origin_x, float origin_y, float &width, float &height) {
    float t_width = 0, t_height = 0;
    width = 0, height = 0;

    C2D_TextBuf textBuf = C2D_TextBufNew(400);
    C2D_Text c2text;
    uint i = 0;
    while (i < dispText.length()) {
        string utf8Char = getUTF8Char(dispText, i);
        i += utf8Char.length();
        C2D_TextParse(&c2text, textBuf, utf8Char.c_str());
        C2D_TextOptimize(&c2text);
        C2D_TextGetDimensions(&c2text, scale, scale, &t_width, &t_height);
        C2D_DrawText(&c2text, C2D_WithColor, origin_x + width, origin_y, 0, scale, scale, color);
        width += t_width;
        height = std::max(t_height, height);
    }
    C2D_TextBufDelete(textBuf);


    if (uiDebug_textOutlines) {
        C2D_DrawLine(origin_x, origin_y, color_debug, origin_x + width, origin_y, color_debug, 1, 0);
        C2D_DrawLine(origin_x, origin_y, color_debug, origin_x, origin_y + height, color_debug, 1, 0);
        C2D_DrawLine(origin_x + width, origin_y + height, color_debug, origin_x + width, origin_y, color_debug, 1, 0);
        C2D_DrawLine(origin_x + width, origin_y + height, color_debug, origin_x, origin_y + height, color_debug, 1, 0);
    }
}

// htmltext::htmltext(string dispText, u32 color, float scale) {
//     this->dispText = dispText;
//     this->color = color;
//     this->scale = scale;
// }

// void htmltext::Draw(float origin_x, float origin_y, float &width, float &height) {

// }

uiStatus::uiStatus(string display_name, string acct, string content) {
    this->display_name = text(display_name, color_text, 0.5);
    this->acct = text(acct, color_text, 0.5);
    this->content = text(content, color_text, 0.4);
}

void uiStatus::Draw(float origin_x, float origin_y, float &width, float &height) {
    float t_width, t_height;

    // Padding
    origin_y += uiPad;
    origin_x += uiPad;

    // Draw display_name
    display_name.Draw(origin_x, origin_y, t_width, t_height);

    float acct_x = origin_x + t_width + uiPad;
    float name_height = t_height;

    // Draw acct
    acct.Draw(acct_x, origin_y, t_width, t_height);

    // Pick max of display_name height and acct height
    name_height = std::max(name_height, t_height);

    // Draw content
    content.Draw(origin_x, origin_y + name_height, t_width, t_height);

    // Set height of this item: padding + content_y + content_height + padding
    height = uiPad + name_height + t_height + uiPad;

    // std::cout << origin_y << ":" << height << std::endl;
}