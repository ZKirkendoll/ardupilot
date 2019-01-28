//===============================================================================
// Description:    Main processing for the Auto GCAS model
// Author:         Zack Kirkendoll
// Date:           01/06/2018
//===============================================================================

#include "gcas_environment.h"
#include "gcas_chronometer.h"
#include "gcas_main.h"

#include <iostream>
#include <fstream>

//===============================================================================
// Local memory
//===============================================================================

static gcasLib_ownshipState gcas_ownshipState;
static bool gcasMainInitialized = false;

static char buffer [80];

//===============================================================================
// Global routines
//===============================================================================

int gcasLib_Init(void)
{
    /* to do
        - setup initialization
        - init the header
        - init the structure
    */
    gcasLib_InitChronometer();
    gcasLib_SetChronometer(0, 0, 0); // set time 0

    // set log file name
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"data/gcaslog_%Y-%m-%d_%H-%M-%S.txt",timeinfo);

    // add header to log file

    gcasMainInitialized = true;

    debugPrint(MAV_SEVERITY_INFO, "__Auto GCAS initialization complete__");

    return 0;
}

int gcasLib_Main(void)
{
    /*
        - abstract the logging
        - move things to local routines
    */

    // Main GCAS chronometer time
    int chronometerHours = 0;
    int chronometerMinutes = 0;
    int chronometerSeconds = 0;
    int chronometerMilliseconds = 0;
    int chronometerSlots = 0;
    
    if(!gcasMainInitialized)
    {
        gcasLib_Init();
        return GCAS_LIB_RETURN_SUCCESS;
    }
    
    // Read the most recent clock values
    gcasLib_GetChronometer(&chronometerHours, &chronometerMinutes, &chronometerSeconds, &chronometerMilliseconds, &chronometerSlots);

    // Read the most recent autopilot states
    gcas_ownshipState = gcasLib_EnvGetOwnshipStates();
    
    if(gcas_ownshipState.auto_gcas_engaged)
    {
        // GCS terminal output
        /*debugPrint(MAV_SEVERITY_INFO, "----GCAS Status Update----");
        debugPrint(MAV_SEVERITY_INFO, "GCAS roll cmd: %d", gcas_ownshipState.roll_cmd);
        debugPrint(MAV_SEVERITY_INFO, "GCAS pitch cmd: %d", gcas_ownshipState.pitch_cmd);
        debugPrint(MAV_SEVERITY_INFO, "GCAS target spd: %d", gcas_ownshipState.tgt_air_spd_cm);
        debugPrint(MAV_SEVERITY_INFO, "GCAS alt error: %d", gcas_ownshipState.alt_error);*/

        // log file
        std::string data = std::to_string( float((chronometerMinutes*60)+chronometerSeconds)+((float)chronometerMilliseconds/1000.0) );
        //data.append(".");
        //data.append(std::to_string(chronometerMilliseconds));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.lat));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.lon));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.alt));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.roll_cmd));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.roll));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.climb_rate_cmd));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.climb_rate));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.tgt_air_spd_cm));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.va));
        data.append(", ");
        data.append(std::to_string(gcas_ownshipState.aero_load_factor));

        gcasLib_Log(data);
    }
    
    return GCAS_LIB_RETURN_SUCCESS;
}

int gcasLib_Shutdown(void)
{
    return GCAS_LIB_RETURN_SUCCESS;
}

int gcasLib_Log(const std::string input)
{
    /* make this more streamlined
        - on init, add headers to file
        - test with df, python
        - may need circular buffer, then write at slower rate from the buffer
    */
    std::ofstream out(buffer, std::ios_base::app | std::ios_base::out);

    // Get the rdbuf of clog.
    // We need it to reset the value before exiting.
    auto old_rdbuf = std::clog.rdbuf();

    // Set the rdbuf of clog.
    std::clog.rdbuf(out.rdbuf());

    // Write to clog.
    std::clog << input << "\n";

    // Reset the rdbuf of clog.
    std::clog.rdbuf(old_rdbuf);

    return GCAS_LIB_RETURN_SUCCESS;
}
