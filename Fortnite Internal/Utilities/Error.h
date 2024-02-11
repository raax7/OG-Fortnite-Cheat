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
    static void error(std::string message, bool close, const char* filePath, int line) {
#if SHOW_MESSAGE_BOX
        std::string errorMessageInfo;
        std::string errorMessage;

        std::string fileName = GetFileName(filePath);

        if (!fileName.empty()) {
            errorMessageInfo = close ? skCrypt("A fatal error has occurred!\n").decrypt() : skCrypt("An error has occurred!\n").decrypt() + fileName + skCrypt(":").decrypt() + std::to_string(line) + skCrypt("\n\n").decrypt();
        }
        else {
            errorMessageInfo = close ? skCrypt("A fatal error has occurred!\n\n").decrypt() : skCrypt("An error has occurred!\n\n").decrypt();
        }

        errorMessage = errorMessageInfo + message;

        LI_FN(MessageBoxA).safe()(NULL, errorMessage.c_str(), skCrypt("Error!").decrypt(), MB_ICONERROR);
#endif
        errorMessage.clear();
        errorMessageInfo.clear();
        fileName.clear();
        message.clear();

        if (close) {
            exit(0);
        }
    }
};

#define THROW_ERROR(message, close) ErrorManager::error(message, close, __FILE__, __LINE__)