//===============================================================================
// Description:    Logic for linking the Auto GCAS model to the surrounding 
//                 software environment
// Author:         Zack Kirkendoll
// Date:           01/06/2018
//===============================================================================

#include "gcas_environment.h"
#include "gcas_main.h"

//===============================================================================
// Definitions
//===============================================================================

#define PI              (3.141592653589793f)
#define DEG_TO_RAD      (PI / 180.0f)
#define RAD_TO_DEG      (180.0f / PI)

//===============================================================================
// Local memory
//===============================================================================

static gcasLib_ownshipState global_ownshipState;

//===============================================================================
// Global routines
//===============================================================================

void gcasLib_EnvReadOwnshipStates(gcasLib_ownshipState ardupilot_ownshipState)
{
    // pass the autopilot object to the gcas module
    global_ownshipState = ardupilot_ownshipState;
    gcasLib_EnvSignalConditioning();
}

void gcasLib_EnvRunMain(void)
{
    gcasLib_Main();
}

gcasLib_ownshipState gcasLib_EnvGetOwnshipStates(void)
{
    return global_ownshipState;
}

//===============================================================================
// Local routines
//===============================================================================

static GCAS_LIB_RETURN gcasLib_EnvSignalConditioning(void)
{
    // Handles converting or manipulating the data for standardization
    global_ownshipState.tgt_air_spd_cm = global_ownshipState.tgt_air_spd_cm / 100;
    global_ownshipState.roll = global_ownshipState.roll * RAD_TO_DEG;
    global_ownshipState.roll_cmd = global_ownshipState.roll_cmd / 100;
    global_ownshipState.alt = global_ownshipState.alt / 100;

    return GCAS_LIB_RETURN_SUCCESS;
}