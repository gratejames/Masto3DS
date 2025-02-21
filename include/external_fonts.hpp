#pragma once
#include <iostream>
#include <vector>
using std::string;
using std::vector;

#include <citro2d.h>

void load_efonts();
int efonts_font(string);
void draw_efont(string text, float x, float y, float scale, u32 color);
void getSize_efont(string text, float scale, float &t_width, float &t_height);
