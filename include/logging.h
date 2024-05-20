#ifndef LOGGING_H /* Header guard to prevent multiple inclusion */
#define LOGGING_H

#include "pd_api.h"

/* Extern declaration for the PlaydateAPI singleton */
extern PlaydateAPI* playdate;

/* Logging macros */

#define LOG_ERROR(format, ...) \
    playdate->system->logToConsole(playdate, "ERROR: %s: " format, __func__, ##__VA_ARGS__)

#define LOG_WARNING(format, ...) \
    playdate->system->logToConsole(playdate, "WARNING: %s: " format, __func__, ##__VA_ARGS__)

#endif /* LOGGING_H */
