#ifndef LOGGER_H
#define LOGGER_H

#define EXTRA_DEBUG_INFO 0

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

#include "Error.h"

#if _DEBUG

class Logger {
private:
    std::ofstream file;

    std::string getTimestamp() const {
        time_t now = time(0);
        struct tm timeinfo;
        localtime_s(&timeinfo, &now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), skCrypt("%Y-%m-%d %H:%M:%S").decrypt(), &timeinfo);
        return buffer;
    }

    std::string getFileName(const char* filePath) {
        if (!filePath) {
            return "";
        }

        size_t lastSlash = std::string(filePath).find_last_of(skCrypt("/\\").decrypt());
        return (lastSlash != std::string::npos) ?
            std::string(filePath).substr(lastSlash + 1) :
            std::string(filePath);
    }

public:
    Logger(const std::string& fileNameWithPath) {
        file.open(fileNameWithPath, std::ios::out | std::ios::app);
        if (!file.is_open()) {
            // Display error
        }
        else {
            file.seekp(0, std::ios::end);
            if (!file.tellp() == 0) {
                file << std::endl << std::endl << std::endl;
            }

            log<std::string>(skCrypt("--------------------- LOGGER STARTED ---------------------").decrypt(), nullptr, 0);
        }
    }

    ~Logger() {
        if (file.is_open()) {
            file.close();
        }
    }

    template <typename T>
    void log(const T& message, const char* filePath, int line) {
        std::ostringstream logStream;

        std::string fileName = getFileName(filePath);

        if (!fileName.empty()) {
            logStream << "[" << getTimestamp() << "] "
                << "[" << fileName << ":" << line << "] "
                << message;
        }
        else {
            logStream << "[" << getTimestamp() << "] " << message;
        }

        file << logStream.str() << std::endl;
    }
};

inline Logger g_logger(skCrypt("C:\\Users\\Coby\\Desktop\\cheat.log").decrypt()); // Replace this file path with your desired path

#define DEBUG_LOG(message) g_logger.log(message, __FILE__, __LINE__)
#if EXTRA_DEBUG_INFO
#define EXTRA_DEBUG_LOG(message) g_logger.log(message, __FILE__, __LINE__)
#endif
#if !EXTRA_DEBUG_INFO
#define EXTRA_DEBUG_LOG(message, file, line) ((void)0)
#endif
#endif

#if !_DEBUG
#define DEBUG_LOG(message, file, line) ((void)0)
#define EXTRA_DEBUG_LOG(message, file, line) ((void)0)
#endif

#endif