#pragma once
#include <string>
#include <Windows.h>
#include "Utilities/skCrypter.h"

#define EXTRA_DEBUG_INFO	FALSE	// Enables writing of EXTRA_DEBUG_LOG messages
#define SHOW_MESSAGE_BOX	TRUE	// Enables the display of error message boxes

#define NAME_DUMP			FALSE	// Dumps all FNames to a log
#define OBJECT_DUMP			FALSE	// Dumps all UObjects to a log

// RISKY SETTINGS! ADVISED TO KEEP THEM DISABLED!
// Creating a thread can add another major detection vector depending on how you inject, and how the Anti-Cheat scans for suspicous threads
#define CREATE_THREAD		FALSE	// Starts the cheat intialization in a new thread
#define UNLOAD_THREAD		FALSE	// Allows the cheat to be unloaded (will only work if DLL is injected as valid module)
// Both of these settings will require a thread to be created, and will be a major detection vector (most of the time)!

inline HMODULE ThisModule = nullptr;// The current module handle