#pragma once

#include "pd_api.h"

void initializeMemoryTools(PlaydateAPI* playdate);

void* pd_malloc(size_t size);
void* pd_calloc(size_t count, size_t size);
void* pd_realloc(void* ptr, size_t size);
void  pd_free(void* ptr);