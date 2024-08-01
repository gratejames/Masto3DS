#pragma once
#include <iostream>
#include <algorithm>
#include <stack>
#include <utility>
using std::string;
using std::stack;
using std::pair;

#include <citro2d.h>

#include "schemas.hpp"

#define uiDebug_textOutlines false
#define uiPad 5
#define defaultFontHeight 30

const u32 color_background          = C2D_Color32(0x28, 0x2C, 0x37, 0xFF);
const u32 color_text                = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
const u32 color_textLink            = C2D_Color32(0xAA, 0xAA, 0xFF, 0xFF);
const u32 color_horizontalRule      = C2D_Color32(0xAA, 0xAA, 0xAA, 0xFF);
const u32 color_debug               = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);

class text {
public:
    text(): text("", 0, 1, {}) {};
    text(string dispText, u32 color, float scale, vector<CustomEmoji> emojis);
    string dispText;
    u32 color;
    float scale;
    vector<CustomEmoji> emojis;
    void Draw(float origin_x, float origin_y, float &width, float &height, const float maxWidth);
private:
};

class htmltext {
public:
    htmltext(): htmltext("", 1, {}) {};
    htmltext(string dispText, float scale, vector<CustomEmoji> emojis);
    string dispText;
    float scale;
    vector<CustomEmoji> emojis;
    void Draw(float origin_x, float origin_y, float &width, float &height, const float maxWidth);
private:
};

class uiStatus {
public:
    uiStatus(Status &status);
    void Draw(float origin_x, float origin_y, float &width, float &height, const float maxWidth);
    Status &internalStatus;
    text display_name;
    text acct;
    htmltext content;
private:
};