// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "font.h"
#include "draw.h"

u8 *TOP_SCREEN0;
u8 *TOP_SCREEN1;
u8 *BOT_SCREEN0;
u8 *BOT_SCREEN1;

size_t current_y = 0;

void DrawInit(void)
{
#ifdef BRAHMA
    TOP_SCREEN0 = (u8*)(0x20000000);
    TOP_SCREEN1 = (u8*)(0x20046500);
    BOT_SCREEN0 = (u8*)(0x2008CA00);
    BOT_SCREEN1 = (u8*)(0x200C4E00);
#elif A9LH
    TOP_SCREEN0 = (u8*)(*(u32*)0x23FFFE00);
    TOP_SCREEN1 = (u8*)(*(u32*)0x23FFFE00);
    BOT_SCREEN0 = (u8*)(*(u32*)0x23FFFE08);
    BOT_SCREEN1 = (u8*)(*(u32*)0x23FFFE08);
#else
	#error "BRAHMA or A9LH must be defined!"
#endif
}

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

void DrawCharacter(unsigned char *screen, int character, size_t x, size_t y, int color, int bgcolor)
{
    for (size_t yy = 0; yy < 8; yy++) {
        size_t xDisplacement = (x * BYTES_PER_PIXEL * SCREEN_WIDTH);
        size_t yDisplacement = ((SCREEN_WIDTH - (y + yy) - 1) * BYTES_PER_PIXEL);

        unsigned char *screenPos = screen + xDisplacement + yDisplacement;
        unsigned char charPos = font[(size_t)character * 8 + yy];
        for (int xx = 7; xx >= 0; xx--) {
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

void DrawString(unsigned char *screen, const char *str, size_t x, size_t y, int color, int bgcolor)
{
    const size_t string_len = strlen(str);

    for (size_t i = 0; i < string_len; i++)
        DrawCharacter(screen, str[i], x + i * 8, y, color, bgcolor);
}

void DrawStringF(size_t x, size_t y, const char *format, ...)
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
		return;

 	u32 cur_pix = current * 400 / max;
	if (cur_pix > 400) cur_pix = 400;

	if (cur_pix == last_pix) return;

	last_current = current;
	last_max = max;
	last_pix = cur_pix;

	for(u32 i=0; i < 400; i++) {
		int col = 0;
		if (i < cur_pix)
			col = RGB(255, 255, 255);
		else
			col = RGB(50, 50, 50);

		line(TOP_SCREEN0, i, col);
	}
}
