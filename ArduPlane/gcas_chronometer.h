
#ifndef gcas_chronometer_h
#define gcas_chronometer_h

#include <stdbool.h>

#include "gcas_environment.h"

//===============================================================================
// Prototypes
//===============================================================================

GCAS_LIB_RETURN gcasLib_InitChronometer();
GCAS_LIB_RETURN gcasLib_CalculateSecondsOffset(int hours1, int minutes1, int seconds1,
                                             int hours2, int minutes2, int seconds2,
                                             int* output);
GCAS_LIB_RETURN gcasLib_GetChronometer(int* hours, int* minutes, int* seconds, int* milliseconds, int* slots);
GCAS_LIB_RETURN gcasLib_SetChronometer(int hours, int minutes, int seconds);

#endif //End include guard
