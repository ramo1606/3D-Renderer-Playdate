#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

/* Type Definition for Mesh */

typedef struct mesh_t
{
    vec3_t* vertices;  /* Dynamic array of vertex positions */
    size_t  numVertices;   /* Number of vertices */

    face_t* faces;      /* Dynamic array of faces (vertex indices) */
    size_t  numFaces;     /* Number of faces */
} mesh_t;


/* Function Declarations */

/* Creates a new mesh object with the given vertices and faces. */
mesh_t* mesh_create(const vec3_t vertices[], size_t numVertices, const face_t faces[], size_t numFaces);

/* Destroys a mesh object, freeing all associated memory. */
void mesh_destroy(mesh_t* mesh);

// ... (You can add other mesh-related functions here as needed)

#endif /* MESH_H */
