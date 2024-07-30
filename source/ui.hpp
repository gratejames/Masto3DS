#include <iostream>
#include <algorithm>
using std::string;

#include <citro2d.h>

#define uiDebug_textOutlines true
#define uiPad 5

const u32 color_background          = C2D_Color32(0x28, 0x2C, 0x37, 0xFF);
const u32 color_text                = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
const u32 color_horizontalRule      = C2D_Color32(0xAA, 0xAA, 0xAA, 0xFF);
const u32 color_debug               = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);

class text {
public:
    text(): text("", 0, 1) {};
    text(string dispText, u32 color, float scale);
    string dispText;
    u32 color;
    float scale;
    void Draw(float origin_x, float origin_y, float &width, float &height);
private:
};

// class htmltext {
// public:
//     htmltext(): htmltext("", 0, 1) {};
//     htmltext(string dispText, u32 color, float scale);
//     string dispText;
//     u32 color;
//     float scale;
//     void Draw(float origin_x, float origin_y, float &width, float &height);
// private:
// };

class uiStatus {
public:
    uiStatus(string display_name, string acct, string content);
    void Draw(float origin_x, float origin_y, float &width, float &height);
    text display_name;
    text acct;
    text content;
private:
};