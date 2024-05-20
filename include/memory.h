#ifndef MEMORY_H
#define MEMORY_H

#include "pd_api.h"
#include "logging.h" 

/* Extern declaration for the PlaydateAPI singleton */
extern PlaydateAPI* playdate;

/* Function Declarations */

/* Initializes memory tools and stores the PlaydateAPI instance */
void initialize_memory_tools(PlaydateAPI* pd);

/* Allocates a block of memory (or allocates if ptr is NULL) */
static inline void* pd_realloc(void* ptr, size_t size)
{
    void* new_ptr = playdate->system->realloc(ptr, size);
    if (new_ptr == NULL && size != 0)
    {
        LOG_ERROR("pd_realloc: Failed to reallocate memory (size: %zu)", size);
    }
    return new_ptr;
}

/* Allocates a block of memory */
static inline void* pd_malloc(size_t size)
{
    return pd_realloc(NULL, size);
}

/* Allocates and initializes a block of memory to zeros */
static inline void* pd_calloc(size_t count, size_t size)
{
    void* ptr = pd_malloc(count * size);
    if (ptr != NULL)
    {
        memset(ptr, 0, count * size);
    }
    return ptr;
}

/* Frees a block of memory */
static inline void pd_free(void* ptr)
{
    playdate->system->realloc(ptr, 0);
}

#endif /* MEMORY_H */