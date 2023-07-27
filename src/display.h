#pragma once

#include "pd_api.h"

void initDisplay(PlaydateAPI* pd);

/* Drawing Functions */
void drawPixel(int x, int y, LCDSolidColor color);
void drawLine (int x0, int y0, int x1, int y1, LCDSolidColor color);
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, LCDSolidColor color);
void drawRect(int x, int y, int width, int height, LCDSolidColor color);
void drawGrid(LCDSolidColor color);

/* Rendering Functions */
void clearFrameBuffer(LCDSolidColor color);
void renderColorBuffer(void);
