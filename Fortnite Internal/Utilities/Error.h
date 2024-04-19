#pragma once
#include <Windows.h>
#include <string>

#include "../Globals.h"

#include "../External-Libs/LazyImporter.h"
#include "../External-Libs/skCrypter.h"

#include "Logger.h"

class ErrorManager {
private:
    static std::string GetFileName(const char* FilePath) {
        if (FilePath == nullptr) {
            return "";
        }

        size_t LastSlash = std::string(FilePath).find_last_of(skCrypt("/\\"));
        return (LastSlash != std::string::npos) ?
            std::string(FilePath).substr(LastSlash + 1) :
            std::string(FilePath);
    }
public:
#if _DEBUG // If we are in debug mode, we will show the file name and line number
    static void ThrowError(std::string Message, bool Close, const char* FilePath, int Line) {
#else
    static void ThrowError(std::string Message, bool Close) {
#endif // _DEBUG
        std::string ErrorMessageInfo = "";
        std::string ErrorMessage = "";

#ifdef _DEBUG // If we are in debug mode, we will show the file name and line number
        std::string FileName = GetFileName(FilePath);

        if (FileName.empty() == false) {
            ErrorMessageInfo = Close ? std::string(skCrypt("A fatal error has occurred!\n")) : std::string(skCrypt("An error has occurred!\n")) + FileName + std::string(skCrypt(":")) + std::to_string(Line) + std::string(skCrypt("\n\n"));
        }
        else {
            ErrorMessageInfo = Close ? std::string(skCrypt("A fatal error has occurred!\n\n")) : std::string(skCrypt("An error has occurred!\n\n"));
        }
#else
        ErrorMessageInfo = Close ? std::string(skCrypt("A fatal error has occurred!\n")) : std::string(skCrypt("An error has occurred!\n\n"));
#endif // _DEBUG

        ErrorMessage = ErrorMessageInfo + Message;

        LI_FN(MessageBoxA).safe()(NULL, ErrorMessage.c_str(), Close ? skCrypt("Fatal Error! (closing)") : skCrypt("Error!"), MB_ICONERROR);

#if LOG_LEVEL > LOG_NONE // If logging is enabled, we will log the error
        DEBUG_LOG(LOG_ERROR, Message.c_str());
#endif // LOG_LEVEL > LOG_NONE

        if (Close) {
            exit(0);
        }
    }
};

#if _DEBUG
    #define THROW_ERROR(message, close) ErrorManager::ThrowError(message, close, __FILE__, __LINE__)
#else
    #define THROW_ERROR(message, close) ErrorManager::ThrowError(message, close)
#endif // _DEBUG