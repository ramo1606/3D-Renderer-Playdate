#ifndef DISPLAY_H  /* Header guard to prevent multiple inclusion */
#define DISPLAY_H

#include "pd_api.h" /* Include Playdate API header for drawing functions */

/* Function Declarations */

/* Initializes the display module within the Playdate environment. */
void display_init(PlaydateAPI* playdate);

/* Drawing Functions */

/* Draws a single pixel at the specified coordinates with the given color. */
void display_draw_pixel(int x, int y, LCDSolidColor color); // Use Playdate's LCDSolidColor enum

/* Draws a line between two points with the given color. */
void display_draw_line(int x0, int y0, int x1, int y1, LCDSolidColor color); // Use Playdate's LCDSolidColor enum

/* Draws a filled triangle with the given vertices and color. */
void display_draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, LCDSolidColor color); // Use Playdate's LCDSolidColor enum

/* Draws a filled rectangle with the given position, dimensions, and color. */
void display_draw_rect(int x, int y, int width, int height, LCDSolidColor color); // Use Playdate's LCDSolidColor enum

/* Draws a grid on the screen with the given color. */
void display_draw_grid(LCDSolidColor color); // Use Playdate's LCDSolidColor enum


/* Rendering Functions */

/* Clears the framebuffer with the specified color. */
void display_clear_framebuffer(LCDSolidColor color); // Use Playdate's LCDSolidColor enum

/* Renders the contents of the color buffer to the Playdate display. */
void display_render_color_buffer(void);

#endif /* DISPLAY_H */
