#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

#include "LazyImporter.h"
#include "skCrypter.h"
#include "../Globals.h"

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
        std::string ErrorMessageInfo;
        std::string ErrorMessage;

        std::string FileName = GetFileName(FilePath);

        if (!FileName.empty()) {
#if _DEBUG
            ErrorMessageInfo = Close ? skCrypt("A fatal error has occurred!\n").decrypt() : skCrypt("An error has occurred!\n\n").decrypt();
#else
            ErrorMessageInfo = Close ? skCrypt("A fatal error has occurred!\n").decrypt() : skCrypt("An error has occurred!\n").decrypt() + FileName + skCrypt(":").decrypt() + std::to_string(Line) + skCrypt("\n\n").decrypt();
#endif
        }
        else {
            ErrorMessageInfo = Close ? skCrypt("A fatal error has occurred!\n\n").decrypt() : skCrypt("An error has occurred!\n\n").decrypt();
        }

        ErrorMessage = ErrorMessageInfo + Message;

        LI_FN(MessageBoxA).safe()(NULL, ErrorMessage.c_str(), skCrypt("Error!").decrypt(), MB_ICONERROR);
#endif
        ErrorMessage.clear();
        ErrorMessageInfo.clear();
        FileName.clear();
        Message.clear();

        if (Close) {
            exit(0);
        }
    }
};

#define THROW_ERROR(message, close) ErrorManager::error(message, close, __FILE__, __LINE__)