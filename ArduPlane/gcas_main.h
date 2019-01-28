
#ifndef gcas_main_h
#define gcas_main_h

//===============================================================================
// Prototypes
//===============================================================================

#include <string>

int gcasLib_Init(void);
int gcasLib_Main(void);
int gcasLib_Shutdown(void);
int gcasLib_Log(const std::string input);

#endif //End include guard
