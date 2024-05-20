#include "display.h"
#include <stdlib.h>
#include "logging.h" 

/* Static variables for display information */
static int displayRowBytes = 0;
static int displayWidth = 0;
static int displayHeight = 0;

/* Pointer to the PlaydateAPI instance */
static PlaydateAPI* playdate = NULL;
static uint8_t* frameBuffer = NULL;

/* Initializes the display module and gets display information */
void display_init(PlaydateAPI* pd)
{
    playdate = pd;
    uint8_t* bitMapMask = NULL;

    // Get display buffer bitmap information and framebuffer pointer
    playdate->graphics->getBitmapData(
        playdate->graphics->getDisplayBufferBitmap(),
        &displayWidth,
        &displayHeight,
        &displayRowBytes,
        &bitMapMask,
        &frameBuffer
    );
}

/* Draws a single pixel at the specified coordinates with the given color */
void display_draw_pixel(int x, int y, LCDSolidColor color)
{
    // Get a pointer to the current framebuffer 
    uint8_t* frameBuffer = playdate->graphics->getFrame();

    // Bounds checking
    if (x < 0 || x >= displayWidth || y < 0 || y >= displayHeight)
    {
        LOG_WARNING("display_draw_pixel: Coordinates out of bounds (%d, %d)", x, y);
        return;
    }

    uint8_t* block = frameBuffer + (y * displayRowBytes) + (x / 8);
    uint8_t data = 0x80 >> (x % 8);
    *block = color ? *block | data : *block & ~data;
}


/* Draws a line between two points with the given color. */
void display_draw_line(int x0, int y0, int x1, int y1, LCDSolidColor color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    {  /* loop */
        display_draw_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}


/* Draws a filled triangle with the given vertices and color. */
void display_draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, LCDSolidColor color)
{
    display_draw_line(x0, y0, x1, y1, color);
    display_draw_line(x0, y0, x2, y2, color);
    display_draw_line(x1, y1, x2, y2, color);
}


/* Draws a filled rectangle with the given position, dimensions, and color. */
void display_draw_rect(int x, int y, int width, int height, LCDSolidColor color)
{
    for (int j = y; j < y + height; j++)
    {
        for (int i = x; i < x + width; i++)
        {
            display_draw_pixel(i, j, color);
        }
    }
}


/* Draws a grid on the screen with the given color. */
void display_draw_grid(LCDSolidColor color)
{
    // Bounds checking
    int x = 5, y = 5;

    while (y < displayHeight)
    {
        while (x < displayWidth)
        {
            display_draw_pixel(x, y, color);
            x += 10;
        }
        x = 5;
        y += 10;
    }
}

/* Renders the framebuffer to the display by marking updated rows */
void display_render_color_buffer(void)
{
    playdate->graphics->markUpdatedRows(0, displayHeight - 1);
}


/* Clears the framebuffer with the specified color */
void display_clear_framebuffer(LCDSolidColor color)
{
    // Get a pointer to the current framebuffer 
    frameBuffer = playdate->graphics->getFrame();

    memset(frameBuffer, color ? 0xFF : 0x00, displayRowBytes * displayHeight);
}

