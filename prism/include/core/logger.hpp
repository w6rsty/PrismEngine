#pragma once

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string_view> 
#include <utility>
#include <map>
#include <memory>
#include <vector>

namespace logger {

#define TRACECOL    "\x1b[94;1m"
#define INFOCOL     "\x1b[92;1m"
#define DEBUGCOL    "\x1b[95;1m"
#define WARNCOL     "\x1b[93;1m"
#define ERRORCOL    "\x1b[91;1m"
#define FATALCOL    "\x1b[91;1m"
#define TAGCOL      "\x1b[96;3m"
#define RESETCOL    "\x1b[0m"

enum Level {
    All = 0,
    Trace,
    Info,
    Debug,
    Warn,
    Error,
    Fatal,
};

class Logger final  {
public:
    Logger(std::ostream& stream) : m_Stream(stream), m_Level(All) {}
    void SetLevel(Level level) { m_Level = level; }

    template<typename... Args>
    void Trace(std::string_view funcName, std::string_view fileName, unsigned int line, Args&&... args) {
        logImpl(Level::Trace, funcName, fileName, line, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Info(std::string_view funcName, std::string_view fileName, unsigned int line, Args&&... args) {
        logImpl(Level::Info, funcName, fileName, line, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Debug(std::string_view funcName, std::string_view fileName, unsigned int line, Args&&... args) {
        logImpl(Level::Debug, funcName, fileName, line, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Warn(std::string_view funcName, std::string_view fileName, unsigned int line, Args&&... args) {
        logImpl(Level::Warn, funcName, fileName, line, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Error(std::string_view funcName, std::string_view fileName, unsigned int line, Args&&... args) {
        logImpl(Level::Error, funcName, fileName, line, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Fatal(std::string_view funcName, std::string_view fileName, unsigned int line, Args&&... args) {
        logImpl(Level::Fatal, funcName, fileName, line, std::forward<Args>(args)...);
    }

private:
    std::ostream& m_Stream; // output target
    Level m_Level;

    template <typename... Args>
    void logImpl(Level level, std::string_view funcName, std::string_view fileName, unsigned int line, Args&&... args) {
        // get time 

        printf("[%s] [%s] [%s:%u] ", Level2Str(level).data(), funcName.data(), fileName.data(), line);
        doLogImpl(std::forward<Args>(args)...);
        m_Stream << std::endl;
    }

    template <typename Param, typename... Params>
    void doLogImpl(Param&& param, Params&&... params) {
        m_Stream << param;
        doLogImpl(std::forward<Params>(params)...);
    }

    template <typename Param>
    void doLogImpl(Param&& param) {
        m_Stream << param;
    }

    std::string_view Level2Str(Level level) const {
        switch(level) {
            case Level::Trace:  return TRACECOL "TRACE" RESETCOL;
            case Level::Info:   return INFOCOL  "INFO"  RESETCOL;
            case Level::Debug:  return DEBUGCOL "DEBUG" RESETCOL;
            case Level::Warn:   return WARNCOL  "WARN"  RESETCOL;
            case Level::Error:  return ERRORCOL "ERROR" RESETCOL;
            case Level::Fatal:  return FATALCOL "FATAL" RESETCOL;
            default: return "";
        }
    }
};

class LoggerManager final {
public:
    // optional
    static void Init() {
        Instance();
    }
    static LoggerManager& Instance() {
        static std::unique_ptr<LoggerManager> instance;
        if (!instance) {
            instance.reset(new LoggerManager());
        }
        return *instance;
    }

    // not fully implemented
    static Logger& ToFile(const std::string& name, const std::string& filename, bool append = false) {
        auto& instance = Instance();
        instance.m_Files.emplace_back(filename, append ? std::ios::app : std::ios::trunc);
        return instance.m_Loggers.emplace(name, Logger(instance.m_Files.back())).first->second;
    }

    // not fully implemented
    static Logger& ToStream(const std::string& name, std::ostream& stream) {
        auto& instance = Instance();
        return instance.m_Loggers.emplace(name, Logger(stream)).first->second;
    }

    Logger& GetDefault() const { return *m_DefaultLogger; }
private:
    std::unique_ptr<Logger> m_DefaultLogger;
    std::map<std::string, Logger> m_Loggers;
    std::vector<std::ofstream> m_Files;

    LoggerManager() {
        m_DefaultLogger.reset(new Logger(std::cout));
    }
};

} // namespace logger

#ifdef ENABLE_LOG
    #define LOG_TRACE(tag, ...)    logger::LoggerManager::Instance().GetDefault().Trace(   __FUNCTION__, __FILE__, __LINE__, "[" TAGCOL, tag, RESETCOL "]: ", ## __VA_ARGS__)
    #define LOG_INFO(tag, ...)     logger::LoggerManager::Instance().GetDefault().Info(    __FUNCTION__, __FILE__, __LINE__, "[" TAGCOL, tag, RESETCOL "]: ", ## __VA_ARGS__)
    #define LOG_DEBUG(tag, ...)    logger::LoggerManager::Instance().GetDefault().Debug(   __FUNCTION__, __FILE__, __LINE__, "[" TAGCOL, tag, RESETCOL "]: ", ## __VA_ARGS__)
    #define LOG_WARN(tag, ...)     logger::LoggerManager::Instance().GetDefault().Warn(    __FUNCTION__, __FILE__, __LINE__, "[" TAGCOL, tag, RESETCOL "]: ", ## __VA_ARGS__)
    #define LOG_ERROR(tag, ...)    logger::LoggerManager::Instance().GetDefault().Error(   __FUNCTION__, __FILE__, __LINE__, "[" TAGCOL, tag, RESETCOL "]: ", ## __VA_ARGS__)
    #define LOG_FATAL(tag, ...)    logger::LoggerManager::Instance().GetDefault().Fatal(   __FUNCTION__, __FILE__, __LINE__, "[" TAGCOL, tag, RESETCOL "]: ", ## __VA_ARGS__)
#else
    #define LOG_TRACE(tag, ...)
    #define LOG_INFO(tag, ...)
    #define LOG_DEBUG(tag, ...)
    #define LOG_WARN(tag, ...)
    #define LOG_ERROR(tag, ...)
    #define LOG_FATAL(tag, ...)
#endif
