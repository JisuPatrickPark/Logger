#include "pch.h"
#include "framework.h"

#include "Logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

namespace MyLib 
{
    std::shared_ptr<spdlog::logger> Logger::s_logger;
    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::s_moduleLoggers;

    static std::shared_ptr<spdlog::logger> createLogger(const std::string& name, const std::string& filepath, bool daily, spdlog::level::level_enum level) 
    {
        auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        std::shared_ptr<spdlog::sinks::sink> file_sink;

        if (daily)
            file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filepath, 0, 0);
        else
            file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath, true);
        
        std::vector<spdlog::sink_ptr> sinks{ debug_sink, file_sink };
        auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());

        logger->set_level(level);
        logger->flush_on(spdlog::level::info);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [T%t] [%^%l%$] [%n] %v");

        spdlog::register_logger(logger);
        return logger;
    }

    void Logger::Init(const std::string& name, const std::string& filepath, bool daily) 
    {
        try 
        {
            s_logger = createLogger(name, filepath, daily, spdlog::level::debug);
        }
        catch (const spdlog::spdlog_ex& ex) 
        {
            ::OutputDebugStringA(("Logger init failed: " + std::string(ex.what()) + "\n").c_str());
        }
    }

    void Logger::CreateModuleLogger(const std::string& moduleName, const std::string& filepath, bool daily) 
    {
        try 
        {
            auto logger = createLogger(moduleName, filepath, daily, spdlog::level::debug);
            s_moduleLoggers[moduleName] = logger;
        }
        catch (const spdlog::spdlog_ex& ex) 
        {
            ::OutputDebugStringA(("Module logger init failed: " + std::string(ex.what()) + "\n").c_str());
        }
    }

    std::shared_ptr<spdlog::logger> Logger::GetLogger(const std::string& moduleName) 
    {
        auto it = s_moduleLoggers.find(moduleName);
        if (it != s_moduleLoggers.end())
            return it->second;
        return nullptr;
    }

    void Logger::SetLevel(spdlog::level::level_enum level) 
    {
        if (s_logger)
            s_logger->set_level(level);
        
        for (auto& kv : s_moduleLoggers) 
        {
            kv.second->set_level(level);
        }
    }

    void Logger::Trace(const std::string& msg) { if (s_logger) s_logger->trace(msg); }
    void Logger::Debug(const std::string& msg) { if (s_logger) s_logger->debug(msg); }
    void Logger::Info(const std::string& msg) { if (s_logger) s_logger->info(msg); }
    void Logger::Warn(const std::string& msg) { if (s_logger) s_logger->warn(msg); }
    void Logger::Error(const std::string& msg) { if (s_logger) s_logger->error(msg); }
    void Logger::Critical(const std::string& msg) { if (s_logger) s_logger->critical(msg); }
}