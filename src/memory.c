#include "memory.h"

static PlaydateAPI* pd = NULL;
void initializeMemoryTools(PlaydateAPI* playdate)
{
    pd = playdate;
}

void* pd_realloc(void* ptr, size_t size) 
{
    return pd->system->realloc(ptr, size);
}

void* pd_malloc(size_t size) 
{
    return pd_realloc(NULL, size);
}

void* pd_calloc(size_t count, size_t size) 
{
    return memset(pd_malloc(count * size), 0, count * size);
}

void pd_free(void* ptr) 
{
    pd_realloc(ptr, 0);
}