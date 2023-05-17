//
//  main.c
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pd_api.h"
#include "memory.h"

uint8_t* colorBuffer = NULL;
LCDBitmap* bitMap = NULL;

PlaydateAPI* playDate = NULL;

static int update(void* userdata);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

void drawGrid(void)
{
    // TODO:
}

void renderColorBuffer(void)
{
    int bitMapWidth = 0;
    int bitMapHeight = 0;
    int bitMapRowBytes = 0;
    uint8_t* bitMapMask = NULL;
    uint8_t* bitMapData = NULL;
    playDate->graphics->getBitmapData(bitMap, &bitMapWidth, &bitMapHeight, &bitMapRowBytes, &bitMapMask, &bitMapData);

    memcpy(bitMapData, colorBuffer, playDate->display->getWidth() * playDate->display->getHeight() * sizeof(uint8_t));

    playDate->graphics->drawBitmap(bitMap, 0, 0, kBitmapUnflipped);
}

void clearColorBuffer(uint8_t color)
{
    for (int y = 0; y < playDate->display->getHeight(); y++)
    {
        for (int x = 0; x < playDate->display->getWidth(); x++)
        {
            colorBuffer[(playDate->display->getWidth() * y) + x] = color;
        }
    }
}

void setup(void)
{
    initializeMemoryTools(playDate);

    // Allocate the required memory in bytes to hold the color buffer
    colorBuffer = (uint8_t*)pd_calloc(playDate->display->getWidth() * playDate->display->getHeight(), sizeof(uint8_t));

    // Creating a SDL texture that is used to display the color buffer
    bitMap = playDate->graphics->newBitmap(playDate->display->getWidth(), playDate->display->getHeight(), kColorWhite);
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
    clearColorBuffer(0x00);
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