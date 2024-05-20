#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include "logging.h" 

/* Type Definitions */

typedef struct
{
    float x;
    float y;
} vec2_t;

typedef struct
{
    float x;
    float y;
    float z;
} vec3_t;

/* Function Declarations */

/* Rotates a 3D vector around the X-axis by the given angle (in radians). */
static inline vec3_t vec3_rotate_x(const vec3_t* v, float angle)
{
    if (v == NULL)
    {
        LOG_ERROR("NULL vector passed to vec3_rotate_x");
        return (vec3_t) { 0 };
    }

    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    vec3_t rotated;
    rotated.x = v->x;
    rotated.y = v->y * cos_angle - v->z * sin_angle;
    rotated.z = v->y * sin_angle + v->z * cos_angle;
    return rotated;
}

/* Rotates a 3D vector around the Y-axis by the given angle (in radians). */
static inline vec3_t vec3_rotate_y(const vec3_t* v, float angle)
{
    if (v == NULL)
    {
        LOG_ERROR("NULL vector passed to vec3_rotate_y");
        return (vec3_t) { 0 };
    }

    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    vec3_t rotated;
    rotated.x = v->x * cos_angle + v->z * sin_angle;
    rotated.y = v->y;
    rotated.z = -v->x * sin_angle + v->z * cos_angle;
    return rotated;
}

/* Rotates a 3D vector around the Z-axis by the given angle (in radians). */
static inline vec3_t vec3_rotate_z(const vec3_t* v, float angle)
{
    if (v == NULL)
    {
        LOG_ERROR("NULL vector passed to vec3_rotate_z");
        return (vec3_t) { 0 };
    }

    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    vec3_t rotated;
    rotated.x = v->x * cos_angle - v->y * sin_angle;
    rotated.y = v->x * sin_angle + v->y * cos_angle;
    rotated.z = v->z;
    return rotated;
}

// ... (Add other vec2_t and vec3_t operations as needed)

#endif /* VECTOR_H */
