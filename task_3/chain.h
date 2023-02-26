#pragma once
#include <iostream>
#include <fstream>

class LogMessage {
public:

    virtual ~LogMessage() = default;

    enum class Type {
        Warning,
        Error,
        FatalError,
        Unknown,
    };

    virtual Type type() const = 0;
    virtual const std::string message() const = 0;
};


class WarningMessage : public LogMessage {
public:
    Type type() const override { return Type::Warning; }
    const std::string message() const override {
        return "Warning";
    }
};

class ErrorMessage : public LogMessage {
public:
    Type type() const override { return Type::Error; }
    const std::string message() const override {
        return "Error";
    }
};

class FatalErrorMessage : public LogMessage {
public:
    Type type() const override { return Type::FatalError; }
    const std::string message() const override {
        return "Fatal error";
    }
};

class UnknownMessage : public LogMessage {
public:
    Type type() const override { return Type::Unknown; }
    const std::string message() const override {
        return "Unknown";
    }
};

class LogHandler {
public:
    explicit LogHandler(std::unique_ptr<LogHandler> next) : next_(std::move(next)) {}
    virtual ~LogHandler() = default;
    void receiveLog(const LogMessage& msg) {
        if (handleLog(msg)) { // Если текущий обработчик принял сообщение, то считаем его обработанным
            return;
        }
        if (!next_) { // Если обработчиков больше нет, то оповещаем о том, что сообщение осталось необработанным
            throw std::runtime_error("Error: Message should be handled!");
        }
        next_->receiveLog(msg); // Иначе передаём сообщение следующему обработчику
    }
private:
    virtual bool handleLog(const LogMessage& msg) = 0; 
    std::unique_ptr<LogHandler> next_;
};


class WarningHandler : public LogHandler {
public:
    using LogHandler::LogHandler;
    WarningHandler() = delete;
    explicit WarningHandler(std::unique_ptr<LogHandler> next) : LogHandler(std::move(next)) {}
    ~WarningHandler() override = default;
private:
    bool handleLog(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Warning) {
            return false;
        }
        std::cout << msg.message() << std::endl;
        return true;
    }
};


class ErrorHandler : public LogHandler {
public:
    using LogHandler::LogHandler;
    ErrorHandler() = delete;
    explicit ErrorHandler(std::unique_ptr<LogHandler> next, const std::string& logFileName) : LogHandler (std::move(next)), logFileName_(logFileName) {}
    ~ErrorHandler() override = default;
private:
    bool handleLog(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Error) {
            return false;
        }
        std::ofstream fout(logFileName_, std::ios::app);
        if (fout) {
            fout << msg.message() << '\n';
        }
        else {
            std::cerr << "file opening error " << std::endl;
        }
        std::cout << msg.message() << std::endl;
        return true;
    }

    std::string logFileName_;
};

class FatalErrorHandler : public LogHandler {
public:
    using LogHandler::LogHandler;
private:
    bool handleLog(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::FatalError) {
            return false;
        }
        throw std::runtime_error("Fatal error");
        return true;
    }
};

class UnknownHandler : public LogHandler {
public:
    using LogHandler::LogHandler;
private:
    bool handleLog(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Unknown) {
            return false;
        }
        throw std::runtime_error("Unknown message");
        return true;
    }
};


