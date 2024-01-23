#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

#define CREATE_THREAD		FALSE	// Creates a thread with CreateThread() at DllMain

#define EXTRA_DEBUG_INFO	FALSE	// Enables writing of EXTRA_DEBUG_LOG messages
#define SHOW_MESSAGE_BOX	TRUE	// Enables the display of message boxes

#define NAME_DUMP			FALSE	// Dumps all FNames to a log
#define OBJECT_DUMP			FALSE	// Dumps all UObjects to a log

inline const std::string LOGGING_PATH = skCrypt("C:\\Users\\raax\\Desktop\\cheat.log").decrypt(); // Replace this file path with your desired path

#endif // GLOBALS_H