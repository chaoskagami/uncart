// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common.h"

#define BYTES_PER_PIXEL 3
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 400

#define SCREEN_SIZE (BYTES_PER_PIXEL * SCREEN_WIDTH * SCREEN_HEIGHT)

#define RGB(r,g,b) (r<<24|b<<16|g<<8|r)

// These offsets have a pointer to the FBs actual location.
#define TOP_SCREEN0 (u8*)(*(u32*)0x23FFFE00)
#define TOP_SCREEN1 (u8*)(*(u32*)0x23FFFE00)
#define BOT_SCREEN0 (u8*)(*(u32*)0x23FFFE08)
#define BOT_SCREEN1 (u8*)(*(u32*)0x23FFFE08)

extern int current_y;

void ClearScreen(unsigned char *screen, int color);
void DrawCharacter(unsigned char *screen, int character, int x, int y, int color, int bgcolor);
void DrawHex(unsigned char *screen, unsigned int hex, int x, int y, int color, int bgcolor);
void DrawString(unsigned char *screen, const char *str, int x, int y, int color, int bgcolor);
void DrawStringF(int x, int y, const char *format, ...);
void DrawHexWithName(unsigned char *screen, const char *str, unsigned int hex, int x, int y, int color, int bgcolor);

void Debug(const char *format, ...);

void ProgressBar(u32 current, u32 max);
void RewindLines(int lines);
