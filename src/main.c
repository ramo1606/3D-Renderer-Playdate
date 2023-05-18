//
//  main.c
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pd_api.h"
#include "memory.h"

int displayRowBytes = 0;
int displayWidth = 0;
int displayHeight = 0;

uint8_t* frameBuffer = NULL;
LCDBitmap* bufferBitmap = NULL;

PlaydateAPI* playDate = NULL;

static int update(void* userdata);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

void putPixel(int x, int y, LCDSolidColor color)
{
    uint8_t* block = frameBuffer + (y*displayRowBytes) + (x / 8);
    uint8_t data = 0x80 >> (x % 8);
    *block = color ? *block | data : *block & ~data;
}

void drawGrid(LCDSolidColor color)
{
    for (int y = 5; y < displayHeight; y += 10)
    {
        for (int x = 5; x < displayWidth; x += 10)
        {
            putPixel(x, y, color);
        }
    }
}

void renderColorBuffer(void)
{
    uint8_t* bitMapData = NULL;
    playDate->graphics->getBitmapData(bufferBitmap, NULL, NULL, NULL, NULL, &bitMapData);
    
    memmove((void*)bitMapData, (void*)frameBuffer, displayRowBytes * displayHeight);

    playDate->graphics->drawBitmap(bufferBitmap, 0, 0, kBitmapUnflipped);
}

void clearFrameBuffer(LCDSolidColor color)
{
    for (int y = 0; y < displayHeight; y++)
    {
        for (int x = 0; x < displayWidth; x++)
        {
            putPixel(x, y, kColorBlack);
        }
    }
}

void setup(void)
{
    initializeMemoryTools(playDate);

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

void processInput(void)
{
    //TODO
}

void gameUpdate(void)
{
    //TODO
}

void render(void)
{
    clearFrameBuffer(kColorBlack);
    drawGrid(kColorWhite);
    renderColorBuffer();
}


#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if (event == kEventInit)
	{
        playDate = pd;
        setup();

		const char* err;
		font = pd->graphics->loadFont(fontpath, &err);

		if (font == NULL)
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
	}

	return 0;
}

static int update(void* userdata)
{
    playDate = userdata;
    processInput();
    gameUpdate();
    render();

	//playDate->system->drawFPS(0, 0);

	return 1;
}