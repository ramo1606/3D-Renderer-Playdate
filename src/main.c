//
//  main.c
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pd_api.h"
#include "memory.h"
#include "display.h"

static PlaydateAPI* playDate = NULL;

static int update(void* userdata);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

void setup(void)
{
    initializeMemoryTools(playDate);
	initDisplay(playDate);
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