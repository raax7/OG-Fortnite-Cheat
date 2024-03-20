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
        strftime(Buffer, sizeof(Buffer), skCrypt("%Y-%m-%d %H:%M:%S"), &TimeInfo);
        return Buffer;
    }

    static std::string GetFileName(const char* FilePath) {
        if (!FilePath) {
            return "";
        }

        size_t LastSlash = std::string(FilePath).find_last_of(skCrypt("/\\"));
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

            Log(0, skCrypt("--------------------- LOGGER STARTED ---------------------"), nullptr, 0);
        }
    }

    static void Log(unsigned __int8 LogLevel, const char* Message, const char* FilePath, int Line) {
        if (LogLevel > LOG_LEVEL) {
            return;
        }

        std::ostringstream LogStream;

        std::string FileName = GetFileName(FilePath);

        if (!FileName.empty()) {
            LogStream << skCrypt("[")<< GetTimestamp() << skCrypt("] ")
                << skCrypt("[") << FileName << skCrypt(":") << Line << skCrypt("] ")
                << Message;
        }
        else {
            LogStream << skCrypt("[") << GetTimestamp() << skCrypt("] ") << Message;
        }

        File << LogStream.str() << std::endl;
    }

    static void Log(unsigned __int8 LogLevel, std::string Message, const char* FilePath, int Line) {
        Log(LogLevel, Message.c_str(), FilePath, Line);
    }

    static void Log(unsigned __int8 LogLevel, const wchar_t* Message, const char* FilePath, int Line) {
		Log(LogLevel, std::wstring(Message), FilePath, Line);
	}

    static void Log(unsigned __int8 LogLevel, std::wstring Message, const char* FilePath, int Line) {
        Log(LogLevel, std::string(Message.begin(), Message.end()).c_str(), FilePath, Line);
	}
};

inline std::ofstream Logger::File;

#define DEBUG_LOG(LogLevel, Message) Logger::Log(LogLevel, Message, __FILE__, __LINE__)
#else
    #define DEBUG_LOG(LogLevel, Message) ((void)0)
#endif