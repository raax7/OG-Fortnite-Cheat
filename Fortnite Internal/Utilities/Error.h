#pragma once
#include <Windows.h>
#include <string>

#include "../Globals.h"

#include "../External-Libs/LazyImporter.h"
#include "../External-Libs/skCrypter.h"

class ErrorManager {
private:
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
#if _DEBUG
    static void ThrowError(std::string Message, bool Close, const char* FilePath, int Line) {
#else
    static void ThrowError(std::string Message, bool Close) {
#endif
#if SHOW_MESSAGE_BOX
        std::string ErrorMessageInfo = "";
        std::string ErrorMessage = "";

#if _DEBUG
        std::string FileName = GetFileName(FilePath);

        if (!FileName.empty()) {
            ErrorMessageInfo = Close ? std::string(skCrypt("A fatal error has occurred!\n")) : std::string(skCrypt("An error has occurred!\n")) + FileName + std::string(skCrypt(":")) + std::to_string(Line) + std::string(skCrypt("\n\n"));
        }
        else {
            ErrorMessageInfo = Close ? std::string(skCrypt("A fatal error has occurred!\n\n")) : std::string(skCrypt("An error has occurred!\n\n"));
        }
#else
        ErrorMessageInfo = Close ? std::string(skCrypt("A fatal error has occurred!\n")) : std::string(skCrypt("An error has occurred!\n\n"));
#endif

        ErrorMessage = ErrorMessageInfo + Message;

        LI_FN(MessageBoxA).safe()(NULL, ErrorMessage.c_str(), Close ? skCrypt("Fatal Error! (closing)") : skCrypt("Error!"), MB_ICONERROR);
#endif
        ErrorMessage.clear();
        ErrorMessageInfo.clear();
#if _DEBUG
        FileName.clear();
#endif
        Message.clear();

        if (Close) {
            exit(0);
        }
    }
};

#if _DEBUG
    #define THROW_ERROR(message, close) ErrorManager::ThrowError(message, close, __FILE__, __LINE__)
#else
    #define THROW_ERROR(message, close) ErrorManager::ThrowError(message, close)
#endif