//===============================================================================
// Description:    GCAS internal chronometer which is always running
// Author:         Zack Kirkendoll
// Date:           01/29/2019
//===============================================================================

#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "gcas_environment.h"
#include "gcas_chronometer.h"

//===============================================================================
// Local memory
//===============================================================================

// This is the master chronometer value. It is expressed in
// terms of seconds offset from the PC clock. The value of
// the PC clock does not matter (as long as it is not
// changed midstream by third parties), since it only serves
// as a reference point.
static int chronometerSecondsOffset;

//===============================================================================
// Global routines
//===============================================================================

GCAS_LIB_RETURN gcasLib_InitChronometer()
{
    chronometerSecondsOffset = 0;

    debugPrint(MAV_SEVERITY_INFO, "----Initialized chronometer----");

    return GCAS_LIB_RETURN_SUCCESS;
}

GCAS_LIB_RETURN gcasLib_CalculateSecondsOffset(int hours1, int minutes1, int seconds1,
                                             int hours2, int minutes2, int seconds2,
                                             int* output)
{
    unsigned int fullSeconds1 = (hours1 * 60 * 60) + (minutes1 * 60) + seconds1;
    unsigned int fullSeconds2 = (hours2 * 60 * 60) + (minutes2 * 60) + seconds2;

    *output = fullSeconds2 - fullSeconds1;

    return GCAS_LIB_RETURN_SUCCESS;
}

GCAS_LIB_RETURN gcasLib_GetChronometer(int* hours, int* minutes, int* seconds, int* milliseconds, int* slots)
{
    struct timespec monotonicTime;
    unsigned long monotonicMilliseconds;
    float monotonicSlots;
    struct tm* convertedTime;

    clock_gettime(CLOCK_MONOTONIC, &monotonicTime);
    monotonicTime.tv_sec += chronometerSecondsOffset;
    monotonicMilliseconds = monotonicTime.tv_nsec / 1000000;
    monotonicSlots = (float)monotonicMilliseconds / 7.8125;
    convertedTime = gmtime(&monotonicTime.tv_sec);

    *hours = convertedTime->tm_hour;
    *minutes = convertedTime->tm_min;
    *seconds = convertedTime->tm_sec;
    *milliseconds = (int)monotonicMilliseconds;
    *slots = (int)monotonicSlots;

    return GCAS_LIB_RETURN_SUCCESS;
}

GCAS_LIB_RETURN gcasLib_SetChronometer(int hours, int minutes, int seconds)
{
    struct timespec monotonicTime;
    struct tm* convertedTime;

    clock_gettime(CLOCK_MONOTONIC, &monotonicTime);
    convertedTime = gmtime(&monotonicTime.tv_sec);

    gcasLib_CalculateSecondsOffset(convertedTime->tm_hour, convertedTime->tm_min, convertedTime->tm_sec,
                                  hours, minutes, seconds,
                                  &chronometerSecondsOffset);

    return GCAS_LIB_RETURN_SUCCESS;
}
