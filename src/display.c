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
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
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