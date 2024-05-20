#ifndef TRIANGLE_H /* Header guard to prevent multiple inclusion */
#define TRIANGLE_H

#include "vector.h" /* Include our vector header */

/* Type Definitions */

/* Represents a face (a set of vertex indices) in a mesh */
typedef struct
{
    int a;   /* Index of the first vertex */
    int b;   /* Index of the second vertex */
    int c;   /* Index of the third vertex */
} face_t;

/* Represents a triangle in 2D space, defined by its three vertices */
typedef struct
{
    vec2_t points[3]; /* Array of 2D vertex positions for the triangle */
} triangle_t;

#endif /* TRIANGLE_H */