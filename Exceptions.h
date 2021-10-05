#pragma once
#include <exception>
#include <string>


class FileException : public std::exception {
private:
    std::string error;

public:
    FileException() {
        error.assign("Error: File has not found!");
    }

    const char* what() const noexcept override {
        return error.c_str();
    }
};

class FileFormatException : public std::exception {
private:
    std::string error;

public:
    FileFormatException() {
        error.assign("Error: Format is incorrect!");
    }
    const char* what() const noexcept override {
        return error.c_str();
    }
};
