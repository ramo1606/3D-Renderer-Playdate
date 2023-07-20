//
//  main.c
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pd_api.h"
#include "memory.h"
#include "display.h"
#include "vector.h"

#define N_POINTS (9*9*9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = { .x = 0.f, .y = 0.f, .z = -5.f };
vec3_t cube_rotation = { .x = 0.f, .y = 0.f, .z = 0.f };

float fov_factor = 256;

static PlaydateAPI* playDate = NULL;

static int update(void* userdata);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

void setup(void)
{
    initializeMemoryTools(playDate);
	initDisplay(playDate);

    int point_count = 0;

    // start loading my array of vectors
    // form -1 to 1
    for (float x = -1.f; x <= 1.f; x += 0.25f)
    {
        for (float y = -1.f; y <= 1.f; y += 0.25f)
        {
            for (float z = -1.f; z <= 1.f; z += 0.25f)
            {
                vec3_t new_point = { .x = x, .y = y, .z = z };
                cube_points[point_count++] = new_point;
            }
        }
    }
}

void processInput(void)
{
    //TODO
}

vec2_t project(vec3_t point)
{
    vec2_t projected_point = { .x = (fov_factor * point.x) / point.z, .y = (fov_factor * point.y) / point.z };
    return projected_point;
}

void gameUpdate(void)
{
    cube_rotation.y += 0.02f;
    cube_rotation.z += 0.02f;
    cube_rotation.x += 0.02f;

    for (int i = 0; i < N_POINTS; ++i)
    {
        vec3_t point = cube_points[i];
        
        vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
        transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

        // Translate the points away from the camera
        transformed_point.z -= camera_position.z;
        projected_points[i] = project(transformed_point);
    }
}

void render(void)
{
    clearFrameBuffer(kColorBlack);
    drawGrid(kColorWhite);
    for (int i = 0; i < N_POINTS; ++i)
    {
        drawRect(projected_points[i].x + (playDate->display->getWidth() / 2), projected_points[i].y + (playDate->display->getHeight() / 2), 4, 4, kColorWhite);
    }
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
		font = playDate->graphics->loadFont(fontpath, &err);

		if (font == NULL)
            playDate->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
        playDate->system->setUpdateCallback(update, pd);
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