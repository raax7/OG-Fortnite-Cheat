#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

#include "Error.h"
#include "../Globals.h"

// Only log if the log level is not LOG_NO
#if LOG_LEVEL > LOG_NONE
class Logger {
private:
    static std::ofstream File;

    static std::string GetTimestamp() {
        time_t Now = time(0);
        struct tm TimeInfo;
        localtime_s(&TimeInfo, &Now);
        char Buffer[80];
        strftime(Buffer, sizeof(Buffer), skCrypt("%Y-%m-%d %H:%M:%S").decrypt(), &TimeInfo);
        return Buffer;
    }

    static std::string GetFileName(const char* FilePath) {
        if (!FilePath) {
            return skCrypt("").decrypt();
        }

        size_t LastSlash = std::string(FilePath).find_last_of(skCrypt("/\\").decrypt());
        return (LastSlash != std::string::npos) ?
            std::string(FilePath).substr(LastSlash + 1) :
            std::string(FilePath);
    }

public:
    static void InitLogger(const std::string& FileNameWithPath) {
        File.open(FileNameWithPath, std::ios::out | std::ios::app);
        if (!File.is_open()) {
            // Display error
        }
        else {
            File.seekp(0, std::ios::end);
            if (!File.tellp() == 0) {
                File << std::endl << std::endl << std::endl;
            }

            Log<std::string>(0, skCrypt("--------------------- LOGGER STARTED ---------------------").decrypt(), nullptr, 0);
        }
    }

    template <typename T>
    static void Log(unsigned __int8 LogLevel, const T& Message, const char* FilePath, int Line) {
        if (LogLevel > LOG_LEVEL) {
			return;
		}

        std::ostringstream LogStream;

        std::string FileName = GetFileName(FilePath);

        if (!FileName.empty()) {
            LogStream << skCrypt("[").decrypt() << GetTimestamp() << skCrypt("] ").decrypt()
                << skCrypt("[").decrypt() << FileName << skCrypt(":").decrypt() << Line << skCrypt("] ").decrypt()
                << Message;
        }
        else {
            LogStream << skCrypt("[").decrypt() << GetTimestamp() << skCrypt("] ").decrypt() << Message;
        }

        File << LogStream.str() << std::endl;
    }
};

inline std::ofstream Logger::File;

#define DEBUG_LOG(LogLevel, Message) Logger::Log(LogLevel, Message, __FILE__, __LINE__)
#else
    #define DEBUG_LOG(LogLevel, Message) ((void)0)
#endif