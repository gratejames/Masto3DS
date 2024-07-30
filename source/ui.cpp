#include "ui.hpp"

text::text(string dispText, u32 color, float scale) {
    this->dispText = dispText;
    this->color = color;
    this->scale = scale;
}

void text::Draw(float origin_x, float origin_y, float &width, float &height) {
    C2D_TextBuf textBuf = C2D_TextBufNew(400);
    C2D_Text c2text;



	C2D_TextParse(&c2text, textBuf, dispText.c_str());
	C2D_TextOptimize(&c2text);
    C2D_TextGetDimensions(&c2text, 0.5, 0.5, &width, &height);
	C2D_DrawText(&c2text, C2D_WithColor, origin_x, origin_y, 0, scale, scale, color);


    if (uiDebug_textOutlines) {
        C2D_DrawLine(origin_x, origin_y, color_debug, origin_x + width, origin_y, color_debug, 1, 0);
        C2D_DrawLine(origin_x, origin_y, color_debug, origin_x, origin_y + height, color_debug, 1, 0);
        C2D_DrawLine(origin_x + width, origin_y + height, color_debug, origin_x + width, origin_y, color_debug, 1, 0);
        C2D_DrawLine(origin_x + width, origin_y + height, color_debug, origin_x, origin_y + height, color_debug, 1, 0);
    }
    C2D_TextBufDelete(textBuf); 
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