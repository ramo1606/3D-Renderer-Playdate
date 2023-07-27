#include "display.h"

#include "memory.h"
#include <stdlib.h>

static int displayRowBytes = 0;
static int displayWidth = 0;
static int displayHeight = 0;

static uint8_t* frameBuffer = NULL;
static LCDBitmap* bufferBitmap = NULL;

static PlaydateAPI* playDate = NULL;

void initDisplay(PlaydateAPI* pd)
{
    playDate = pd;

    uint8_t* bitMapMask = NULL;
    uint8_t* bitMapData = NULL;

    // Get display buffer Bitmap and its info
    playDate->graphics->getBitmapData(playDate->graphics->getDisplayBufferBitmap(), &displayWidth, &displayHeight, &displayRowBytes, &bitMapMask, &bitMapData);

    // Allocate the required memory in bytes to hold the frame buffer
    // Allocate a frame buffer of height * rowBytes
    // Each display row hast 52 bytes = 416 pixels, the las two bytes are not used.
    // since each display row are 32bits aligned we need at leas 52 bytes
    frameBuffer = (uint8_t*)pd_calloc(displayRowBytes * displayHeight, sizeof(uint8_t));
    bufferBitmap = playDate->graphics->newBitmap(displayWidth, displayHeight, kColorWhite);
}

void drawPixel(int x, int y, LCDSolidColor color)
{
    //Logic to set a specific pixel in the screen
    uint8_t* block = frameBuffer + (y * displayRowBytes) + (x / 8);
    uint8_t data = 0x80 >> (x % 8);
    *block = color ? *block | data : *block & ~data;
}

//void drawLine (int x0, int y0, int x1, int y1, LCDSolidColor color)
//{
//    // DDA Algorithm
//    int delta_x = (x1 - x0);
//    int delta_y = (y1 - y0);
//
//    int longest_side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);
//
//    // Find how much we should increment in both x and y each step
//    float x_inc = delta_x / (float)longest_side_length;
//    float y_inc = delta_y / (float)longest_side_length;
//
//    float current_x = x0;
//    float current_y = y0;
//
//    for(int i = 0; i <= longest_side_length; i++)
//    {
//        drawPixel(round(current_x), round(current_y), color);
//        current_x += x_inc;
//        current_y += y_inc;
//    }
//}

void drawLine (int x0, int y0, int x1, int y1, LCDSolidColor color)
{
    // Bresenham Algorithms
    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2; /* error value e_xy */
    
    for (;;) /* loop */
    {
        drawPixel(x0,y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, LCDSolidColor color)
{
    drawLine (x0, y0, x1, y1, color);
    drawLine (x0, y0, x2, y2, color);
    drawLine (x1, y1, x2, y2, color);
}

void drawGrid(LCDSolidColor color)
{
    for (int y = 5; y < displayHeight; y += 10)
    {
        for (int x = 5; x < displayWidth; x += 10)
        {
            drawPixel(x, y, color);
        }
    }
}

void drawRect(int x, int y, int width, int height, LCDSolidColor color)
{
    for (int j = y; j < y + height; j++)
    {
        for (int i = x; i < x + width; i++)
        {
            drawPixel(i, j, color);
        }
    }
}

void renderColorBuffer(void)
{
    // TODO: can I just draw into the frame buffer directly??
    // Try to use playdate->graphics->getFrame(void) as unique buffer

    // Get the info of our buffer BitMap to get its raw data
    uint8_t* bitMapData = NULL;
    playDate->graphics->getBitmapData(bufferBitmap, NULL, NULL, NULL, NULL, &bitMapData);

    // Move our frameBuffer data to our buffer BitMap
    memmove((void*)bitMapData, (void*)frameBuffer, displayRowBytes * displayHeight);

    // Draw our buffer BitMap to screen
    playDate->graphics->drawBitmap(bufferBitmap, 0, 0, kBitmapUnflipped);
}

void clearFrameBuffer(LCDSolidColor color)
{
    for (int y = 0; y < displayHeight; y++)
    {
        for (int x = 0; x < displayWidth; x++)
        {
            drawPixel(x, y, kColorBlack);
        }
    }
}
