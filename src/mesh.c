#include "mesh.h"
#include "memory.h"  /* Include our custom memory functions */
#include "logging.h" 

/* Creates a new mesh object with the given vertices and faces. */
mesh_t* mesh_create(const vec3_t vertices[], size_t num_vertices, const face_t faces[], size_t num_faces)
{
    // Input Validation (already done in mesh.h)

    // Memory Allocation & Error Checking
    mesh_t* mesh = (mesh_t*)pd_malloc(sizeof(mesh_t));
    if (mesh == NULL)
    {
        LOG_ERROR("Failed to allocate memory for mesh");
        return NULL;
    }

    mesh->vertices = (vec3_t*)pd_malloc(sizeof(vec3_t) * num_vertices);
    mesh->faces = (face_t*)pd_malloc(sizeof(face_t) * num_faces);

    if (mesh->vertices == NULL)
    {
        LOG_ERROR("Failed to allocate memory for vertices");
        pd_free(mesh->faces);
        pd_free(mesh);
        return NULL;
    }
    if (mesh->faces == NULL)
    {
        LOG_ERROR("Failed to allocate memory for faces");
        pd_free(mesh->vertices);
        pd_free(mesh);
        return NULL;
    }

    // Copy Vertex and Face Data 
    mesh->numVertices = num_vertices;
    for (int i = 0; i < num_vertices; i++)
    {
        mesh->vertices[i] = vertices[i];
    }

    mesh->numFaces = num_faces;
    for (int i = 0; i < num_faces; i++)
    {
        mesh->faces[i] = faces[i];
    }

    return mesh;
}


/* Destroys a mesh object, freeing all associated memory. */
void mesh_destroy(mesh_t* mesh)
{
    if (mesh != NULL)
    {
        pd_free(mesh->vertices);
        pd_free(mesh->faces);
        pd_free(mesh);
    }
}