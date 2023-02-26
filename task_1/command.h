#pragma once
#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand {
public:
    ConsoleLogCommand() = default;
    void print(const std::string& message) override {
        std::cout << message << std::endl;
    }

    ~ConsoleLogCommand() = default;

private:
    std::string msg_;
};

class FileLogCommand : public LogCommand {
public:

    FileLogCommand() = delete;
    explicit FileLogCommand(const std::string fileName) : fileName_(fileName) {}

    void print(const std::string& message) override {
        std::ofstream fout(fileName_, std::ios::app);
        if (fout) {
            fout << message << '\n';
        }
        else {
            std::cerr << "file opening error " << std::endl;
        }
    }
    ~FileLogCommand() = default;

private:
    std::string fileName_;
};

