#pragma once

#include "pd_api.h"

void initDisplay(PlaydateAPI* pd);

/* Drawing Functions */
void drawPixel(int x, int y, LCDSolidColor color);
void drawRect(int x, int y, int width, int height, LCDSolidColor color);
void drawGrid(LCDSolidColor color);

/* Rendering Functions */
void clearFrameBuffer(LCDSolidColor color);
void renderColorBuffer(void);