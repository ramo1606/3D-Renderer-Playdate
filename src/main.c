#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pd_api.h"
#include "memory.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"

vec3_t mesh_vertices[8] =
{
    {.x = -1, .y = -1, .z = -1 },
    {.x = -1, .y = 1, .z = -1 },
    {.x = 1, .y = 1, .z = -1 },
    {.x = 1, .y = -1, .z = -1 },
    {.x = 1, .y = 1, .z = 1 },
    {.x = 1, .y = -1, .z = 1 },
    {.x = -1, .y = 1, .z = 1 },
    {.x = -1, .y = -1, .z = 1 }
};

face_t mesh_faces[12] =
{
    /* Front */
    {.a = 0, .b = 1, .c = 2 },
    {.a = 0, .b = 2, .c = 3 },
    /* Right */
    {.a = 3, .b = 2, .c = 4 },
    {.a = 3, .b = 4, .c = 5 },
    /* Back */
    {.a = 5, .b = 4, .c = 6 },
    {.a = 5, .b = 6, .c = 7 },
    /* Left */
    {.a = 7, .b = 6, .c = 1 },
    {.a = 7, .b = 1, .c = 0 },
    /* Top */
    {.a = 1, .b = 6, .c = 4 },
    {.a = 1, .b = 4, .c = 2 },
    /* Bottom */
    {.a = 5, .b = 7, .c = 0 },
    {.a = 5, .b = 0, .c = 3 }
};

// Number of faces in the cube mesh
#define N_MESH_FACES (sizeof(mesh_faces) / sizeof(mesh_faces[0]))

/* Global variables */
triangle_t triangles_to_render[N_MESH_FACES];
vec3_t camera_position = { .x = 0.f, .y = 0.f, .z = -5.f };
vec3_t cube_rotation = { .x = 0.f, .y = 0.f, .z = 0.f };
float fov_factor = 256;
LCDFont* font = NULL;

/* Function prototypes */
static int update(void* userdata);
void setup(void);
void process_input(void);
vec2_t project(vec3_t point);
void game_update(void);
void render(void);

/* Playdate API instance */
PlaydateAPI* playdate = NULL;  // Declare here, initialize in eventHandler


/* Update callback function */
int update(void* userdata)
{
    playdate = userdata;

    process_input();
    game_update();
    render();

    playdate->system->drawFPS(0, 0);

    return 1; // Continue updating
}

/* Main Playdate event handler */
#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
    if (event == kEventInit)
    {
        playdate = pd;
        setup();

        // Load font (with error handling)
        const char* err;
        const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
        font = playdate->graphics->loadFont(fontpath, &err);

        if (font == NULL)
        {
            playdate->system->logToConsole(playdate, "ERROR: Couldn't load font %s: %s", fontpath, err);
            return 1; // Indicate an error occurred
        }

        playdate->system->setUpdateCallback(update, pd);
    }

    return 0;
}

/* Game setup function */
void setup(void)
{
    initialize_memory_tools(playdate);
    display_init(playdate);
}

/* Function to process user input (empty for now) */
void process_input(void)
{
    // TODO: Implement input handling logic here
}

/* Projects a 3D point to 2D screen coordinates */
vec2_t project(vec3_t point)
{
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z
    };
    return projected_point;
}

/* Updates the game state (cube rotation) */
void game_update(void)
{
    // Update cube rotation angles
    cube_rotation.x += 0.02f;
    cube_rotation.y += 0.02f;
    cube_rotation.z += 0.02f;

    // Process each face of the mesh (cube)
    for (int i = 0; i < N_MESH_FACES; i++)
    {
        // Get the vertices that make up the current face
        face_t mesh_face = mesh_faces[i];
        vec3_t face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a]; // Note: No need to subtract 1 since arrays are 0-indexed
        face_vertices[1] = mesh_vertices[mesh_face.b];
        face_vertices[2] = mesh_vertices[mesh_face.c];

        // Create a triangle for this face
        triangle_t projected_triangle;

        // Process each vertex of the face
        for (int j = 0; j < 3; j++)
        {
            vec3_t transformed_vertex = face_vertices[j];

            // Apply rotation transformations
            transformed_vertex = vec3_rotate_x(&transformed_vertex, cube_rotation.x);
            transformed_vertex = vec3_rotate_y(&transformed_vertex, cube_rotation.y);
            transformed_vertex = vec3_rotate_z(&transformed_vertex, cube_rotation.z);

            // Translate vertex relative to camera
            transformed_vertex.z -= camera_position.z;

            // Project the vertex onto the 2D screen
            vec2_t projected_vertex = project(transformed_vertex);

            // Scale and translate to the center of the screen
            projected_vertex.x += playdate->display->getWidth() / 2;
            projected_vertex.y += playdate->display->getHeight() / 2;

            // Store the projected vertex in the triangle
            projected_triangle.points[j] = projected_vertex;
        }

        // Store the projected triangle for rendering
        triangles_to_render[i] = projected_triangle;
    }
}

/* Renders the scene */
void render(void)
{
    display_clear_framebuffer(kColorBlack);
    display_draw_grid(kColorWhite);

    for (int i = 0; i < N_MESH_FACES; i++)
    {
        // Extract vertices for the current triangle
        triangle_t triangle = triangles_to_render[i];

        // Draw the triangle
        display_draw_triangle(
            triangle.points[0].x, triangle.points[0].y,
            triangle.points[1].x, triangle.points[1].y,
            triangle.points[2].x, triangle.points[2].y,
            kColorWhite
        );
    }

    // Update the Playdate display
    display_render_color_buffer();
}
