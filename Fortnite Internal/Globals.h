#pragma once
#include <Windows.h>

// ALL POTENTIAL DETECTION VECTORS ARE MARKED WITH A COMMENT!
// IF YOU WANT TO AVOID DETECTION, YOU SHOULD DISABLE THEM!



#define LOG_NONE 0					// No logs
#define LOG_ERROR 1					// Only error logs
#define LOG_OFFSET 2				// Error logs and offset logs
#define LOG_INFO 3					// Error logs, offset logs and general info logs
#define LOG_ALL 3					// Log everything
#define LOG_LEVEL_MAX 4



// Level of DEBUG_LOG to display
#ifdef _DEBUG
	#define LOG_LEVEL		LOG_INFO
#endif // _DEBUG
#ifdef NODEBUG
	#define LOG_LEVEL		LOG_NONE// No logs in release mode by default
#endif // NODEBUG

// Only enable this if you are sure your injector supports SEH
#define USING_SEH			TRUE	// Enables the use oh SEH (Structured Exception Handler) for verifying if a pointer is valid
#define SEASON_20_PLUS		FALSE	// REQUIRED ON SEASON 20 AND FORWARD! Enables the use of doubles instead of floats on structures like FVector, FRotator etc. Changes a few other structures too

#define SHOW_MESSAGE_BOX	TRUE	// Enables the display of error message boxes
#define CRASH_ON_NOT_FOUND	FALSE	// Crashes the game when an offset/VFT index is not found

#define NAME_DUMP			FALSE	// Dumps all FNames to a log
#define OBJECT_DUMP			FALSE	// Dumps all UObjects to a log

// RISKY SETTINGS! ADVISED TO KEEP THEM DISABLED!
// Creating a thread can add another major detection vector depending on how you inject, and how the Anti-Cheat scans for suspicous threads
#define INIT_THREAD			TRUE	// Starts the cheat intialization in a new thread
#define UNLOAD_THREAD		TRUE	// Allows the cheat to be unloaded (will only work if DLL is injected as valid module)
// Both of these settings will require a thread to be created, and will be a major detection vector (most of the time)!

inline HMODULE ThisModule = nullptr;// The current module handle



// Compile-Time Asserts
static_assert(LOG_LEVEL < LOG_LEVEL_MAX && LOG_LEVEL >= LOG_NONE, "Invalid log level");