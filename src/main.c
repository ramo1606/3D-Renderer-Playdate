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
#include "mesh.h"

triangle_t triangles_to_render[N_MESH_FACES];

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

    for (int i = 0; i < N_MESH_FACES; i++)
    {
        face_t mesh_face = mesh_faces[i];
        vec3_t face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];

        triangle_t projected_triangle = { 0 };

        for (int j = 0; j < 3; j++)
        {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

            // Translate the vertex away from the camera
            transformed_vertex.z -= camera_position.z;
            
            vec2_t projected_vertex = project(transformed_vertex);

            // Scale and translate the projected points to the middle of the screen
            projected_vertex.x += playDate->display->getWidth() / 2;
            projected_vertex.y += playDate->display->getHeight() / 2;

            projected_triangle.points[j] = projected_vertex;
        }

        triangles_to_render[i] = projected_triangle;
    }
}

void render(void)
{
    clearFrameBuffer(kColorBlack);
    drawGrid(kColorWhite);
	
    for (int i = 0; i < N_MESH_FACES; i++)
    {
        //for (int j = 0; j < 3; j++)
        //{
        //    drawRect(triangles_to_render[i].points[j].x, triangles_to_render[i].points[j].y, 3, 3, kColorWhite);
        //}

        drawTriangle(triangles_to_render[i].points[0].x, triangles_to_render[i].points[0].y,
                     triangles_to_render[i].points[1].x, triangles_to_render[i].points[1].y,
                     triangles_to_render[i].points[2].x, triangles_to_render[i].points[2].y,
                     kColorWhite);
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
