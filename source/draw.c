// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "font.h"
#include "draw.h"

int current_y = 0;

void ClearScreen(unsigned char *screen, int color)
{
    int i;
    unsigned char *screenPos = screen;
    for (i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH); i++) {
        *(screenPos++) = color >> 16;  // B
        *(screenPos++) = color >> 8;   // G
        *(screenPos++) = color & 0xFF; // R
    }
}

void DrawCharacter(unsigned char *screen, int character, int x, int y, int color, int bgcolor)
{
    int yy, xx;
    for (yy = 0; yy < 8; yy++) {
        int xDisplacement = (x * BYTES_PER_PIXEL * SCREEN_WIDTH);
        int yDisplacement = ((SCREEN_WIDTH - (y + yy) - 1) * BYTES_PER_PIXEL);
        unsigned char *screenPos = screen + xDisplacement + yDisplacement;

        unsigned char charPos = font[character * 8 + yy];
        for (xx = 7; xx >= 0; xx--) {
            if ((charPos >> xx) & 1) {
                *(screenPos + 0) = color >> 16;  // B
                *(screenPos + 1) = color >> 8;   // G
                *(screenPos + 2) = color & 0xFF; // R
            } else {
                *(screenPos + 0) = bgcolor >> 16;  // B
                *(screenPos + 1) = bgcolor >> 8;   // G
                *(screenPos + 2) = bgcolor & 0xFF; // R
            }
            screenPos += BYTES_PER_PIXEL * SCREEN_WIDTH;
        }
    }
}

void DrawString(unsigned char *screen, const char *str, int x, int y, int color, int bgcolor)
{
    const size_t string_len = strlen(str);

    for (size_t i = 0; i < string_len; i++)
        DrawCharacter(screen, str[i], x + i * 8, y, color, bgcolor);
}

void DrawStringF(int x, int y, const char *format, ...)
{
    char str[256];
    va_list va;

    va_start(va, format);
    vsnprintf(str, 256, format, va);
    va_end(va);

    DrawString(TOP_SCREEN0, str, x, y, RGB(255, 255, 255), RGB(0, 0, 0));
}

void Debug(const char *format, ...)
{
    char str[50];
    va_list va;

    va_start(va, format);
    vsnprintf(str, sizeof(str), format, va);
    va_end(va);

    DrawString(TOP_SCREEN0, str, 0, current_y, RGB(255, 255, 255), RGB(0, 0, 0));

    current_y += 10;
    if (current_y >= 220) {
        current_y = 0;
    }
}

int xy_to_offset(int x, int y) {
    if (x > 400 || y > 240) return 0; // Whoa, not okay.

    int xDisplacement = (x * BYTES_PER_PIXEL * SCREEN_WIDTH);
    int yDisplacement = ((SCREEN_WIDTH - y - 1) * BYTES_PER_PIXEL);
    return xDisplacement + yDisplacement;
}

void RewindLines(int lines) {
	current_y -= 10 * lines;
	if (current_y < 0 || current_y >= 220)
		current_y = 0;
}

void line(u8* screen, int line, int color) {
    int xDis = (line * BYTES_PER_PIXEL * SCREEN_WIDTH);
    int yDis = ((SCREEN_WIDTH - 239) * BYTES_PER_PIXEL);

	u8* at = screen + (xDis + yDis);
	for(int j=0; j<8; j++) {
		*at++ = (color >> 16) & 0xff;
		*at++ = (color >> 8) & 0xff;
		*at++ = color & 0xff;
	}
}

u32 last_current = 0, last_max = 0, last_pix = 0;
void ProgressBar(u32 current, u32 max) {
	if (last_current == current && last_max == max)
		return 0;

 	u32 cur_pix = current * 400 / max;
	if (cur_pix > 400) cur_pix = 400;

	if (cur_pix == last_pix) return 0;

	last_current = current;
	last_max = max;
	last_pix = cur_pix;

	for(int i=0; i < 400; i++) {
		int col = 0;
		if (i < cur_pix)
			col = RGB(255, 255, 255);
		else
			col = RGB(50, 50, 50);

		line(TOP_SCREEN0, i, col);
	}
}
