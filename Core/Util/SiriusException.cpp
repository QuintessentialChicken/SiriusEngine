//
// Created by Leon on 15/01/2025.
//

#include "SiriusException.h"
#include <sstream>

SiriusException::SiriusException(int line, const char *file) noexcept : line{line}, file{file} {}

const char * SiriusException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << ":" << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char * SiriusException::GetType() const noexcept {
    return "SiriusException";
}

int SiriusException::GetLine() const noexcept {
    return line;
}

const std::string & SiriusException::GetFile() const noexcept {
    return file;
}

std::string SiriusException::GetOriginString() const noexcept {
    std::ostringstream oss;
    oss << "[File] " << GetFile() << ":" << GetLine();
    return oss.str();
}
