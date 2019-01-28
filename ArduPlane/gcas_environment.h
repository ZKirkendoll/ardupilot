
#ifndef gcas_environment_h
#define gcas_environment_h

#include <stdint.h>

// Includes from the environment
#include "Plane.h"

//===============================================================================
// Definitions
//===============================================================================

#ifdef GCAS_LIB_SELF_TEST
    #define debugPrint printf
    typedef enum MAV_SEVERITY
    {
        MAV_SEVERITY_EMERGENCY=0, /* System is unusable. This is a "panic" condition. | */
        MAV_SEVERITY_ALERT=1, /* Action should be taken immediately. Indicates error in non-critical systems. | */
        MAV_SEVERITY_CRITICAL=2, /* Action must be taken immediately. Indicates failure in a primary system. | */
        MAV_SEVERITY_ERROR=3, /* Indicates an error in secondary/redundant systems. | */
        MAV_SEVERITY_WARNING=4, /* Indicates about a possible future error if this is not resolved within a given timeframe. Example would be a low battery warning. | */
        MAV_SEVERITY_NOTICE=5, /* An unusual event has occured, though not an error condition. This should be investigated for the root cause. | */
        MAV_SEVERITY_INFO=6, /* Normal operational messages. Useful for logging. No action is required for these messages. | */
        MAV_SEVERITY_DEBUG=7, /* Useful non-operational messages that can assist in debugging. These should not occur during normal operation. | */
        MAV_SEVERITY_ENUM_END=8, /*  | */
    } MAV_SEVERITY;
#else
    #define debugPrint gcs().send_text
#endif

typedef struct gcasLib_ownshipState
{
    // commanded
    int32_t roll_cmd;
    int32_t pitch_cmd;
    int32_t yaw_cmd;

    int32_t tgt_alt_amsl_cm; // target altitude above mean sea level [cm]
    int32_t tgt_air_spd_cm; // target airspeed
    int32_t climb_rate_cmd;

    // current states
    int32_t lat; // position north
    int32_t lon; // position east
    float alt; // altitude (m)
    int32_t u; // body velocity x (m/s)
    int32_t v;
    int32_t w;
    float roll;
    float pitch;
    float yaw;
    int32_t p; // angular rate x (deg/s)
    int32_t q;
    int32_t r;
    float va; // airspeed (m/s)
    int32_t alpha;
    int32_t beta;
    int32_t wn; // wind north
    int32_t we;
    int32_t ed;
    int32_t delta_e; // elevator angle (deg)
    int32_t delta_a;
    int32_t delta_r;
    int32_t delta_t; // throttle setting
    int32_t time; // simulation time
    float climb_rate;
    float dive_angle;
    
    // Miscellaneous
    bool auto_gcas_engaged;
    int32_t alt_error;
    int32_t roll_limit;
    int32_t max_load_factor;
    int32_t aero_load_factor;

    // estimated

} gcasLib_ownshipState;

// add structure for input output, then only use specific structures through the env

typedef enum GCAS_LIB_RETURN
{
    GCAS_LIB_RETURN_SUCCESS                  = 0,
    GCAS_LIB_RETURN_FAIL                     = 1,
    GCAS_LIB_RETURN_INVALID_ARGUMENTS        = 2,
    GCAS_LIB_RETURN_FAILED_SUBROUTINE_CALL   = 3,
} GCAS_LIB_RETURN;

//===============================================================================
// Prototypes
//===============================================================================

void gcasLib_EnvReadOwnshipStates(gcasLib_ownshipState ardupilot_ownshipState);
void gcasLib_EnvRunMain(void);
gcasLib_ownshipState gcasLib_EnvGetOwnshipStates(void);
static GCAS_LIB_RETURN gcasLib_EnvSignalConditioning(void);

#endif //End include guard
