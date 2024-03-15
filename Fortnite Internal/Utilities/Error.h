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

        size_t LastSlash = std::string(FilePath).find_last_of(skCrypt("/\\").decrypt());
        return (LastSlash != std::string::npos) ?
            std::string(FilePath).substr(LastSlash + 1) :
            std::string(FilePath);
    }
public:
    static void error(std::string Message, bool Close, const char* FilePath, int Line) {
#if SHOW_MESSAGE_BOX
        std::string ErrorMessageInfo = "";
        std::string ErrorMessage = "";

#if _DEBUG
        std::string FileName = GetFileName(FilePath);

        if (!FileName.empty()) {
            ErrorMessageInfo = Close ? skCrypt("A fatal error has occurred!\n").decrypt() : skCrypt("An error has occurred!\n").decrypt() + FileName + skCrypt(":").decrypt() + std::to_string(Line) + skCrypt("\n\n").decrypt();
        }
        else {
            ErrorMessageInfo = Close ? skCrypt("A fatal error has occurred!\n\n").decrypt() : skCrypt("An error has occurred!\n\n").decrypt();
        }
#else
        ErrorMessageInfo = Close ? skCrypt("A fatal error has occurred!\n").decrypt() : skCrypt("An error has occurred!\n\n").decrypt();
#endif

        ErrorMessage = ErrorMessageInfo + Message;

        LI_FN(MessageBoxA).safe()(NULL, ErrorMessage.c_str(), skCrypt("Error!").decrypt(), MB_ICONERROR);
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
    #define THROW_ERROR(message, close) ErrorManager::error(message, close, __FILE__, __LINE__)
#else
    #define THROW_ERROR(message, close) ErrorManager::error(message, close, "", 0)
#endif