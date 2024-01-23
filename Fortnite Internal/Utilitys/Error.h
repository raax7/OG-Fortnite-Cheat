#ifndef ERROR_H
#define ERROR_H

#include <Windows.h>
#include <iostream>
#include <string>

#include "LazyImporter.h"
#include "skCrypter.h"
#include "../Globals.h"

class ErrorManager {
private:
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
    void error(std::string message, bool close, const char* filePath, int line) {
#if SHOW_MESSAGE_BOX
        std::string errorMessageInfo;
        std::string errorMessage;

        std::string fileName = getFileName(filePath);

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

inline ErrorManager g_ErrorManager;

#define THROW_ERROR(message, close) g_ErrorManager.error(message, close, __FILE__, __LINE__)

#endif