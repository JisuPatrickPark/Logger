#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

namespace MyLib {

    class Logger {
    public:
        // --------------------------
        // 기본Logger 초기화
        // --------------------------
        static void Init(const std::string& strName,
            const std::string& strFilepath,
            bool bDaily = false);

        // --------------------------
        // 모듈 Logger 생성
        // --------------------------
        static void CreateModuleLogger(const std::string& strModuleName,
            const std::string& strFilepath,
            bool bDaily = false);

        // --------------------------
        // Ini파일 기반 Logger 생성
        // --------------------------
        static void InitFromIni(const std::string& iniPath);

        // --------------------------
        // 모듈 Logger 가져오기
        // --------------------------
        static std::shared_ptr<spdlog::logger> GetLogger(const std::string& strModuleName);

        // --------------------------
        // 전체 로그 레벨 변경
        // --------------------------
        static void SetAllLoggerLevel(spdlog::level::level_enum spdLevel);

        static void SetModuleLoggerLevel(const std::string& module, spdlog::level::level_enum spdLevel);

        // --------------------------
        // 기본 Logger 함수
        // --------------------------
        template<typename... Args>
        static void Trace(const char* fmt, Args&&... args)
        {
            if (s_logger) s_logger->trace(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Debug(const char* fmt, Args&&... args)
        {
            if (s_logger) s_logger->debug(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Info(const char* fmt, Args&&... args)
        {
            if (s_logger) s_logger->info(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Warn(const char* fmt, Args&&... args)
        {
            if (s_logger) s_logger->warn(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Error(const char* fmt, Args&&... args)
        {
            if (s_logger) s_logger->error(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Critical(const char* fmt, Args&&... args)
        {
            if (s_logger) s_logger->critical(fmt, std::forward<Args>(args)...);
        }

        static void Flush()
        {
            if (s_logger) s_logger->flush();
        }

        // --------------------------
        // 모듈 Logger 함수
        // --------------------------
        template<typename... Args>
        static void ModuleTrace(const std::string& module, const char* fmt, Args&&... args)
        {
            auto logger = GetLogger(module);
            if (logger) logger->trace(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void ModuleDebug(const std::string& module, const char* fmt, Args&&... args)
        {
            auto logger = GetLogger(module);
            if (logger) logger->debug(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void ModuleInfo(const std::string& module, const char* fmt, Args&&... args)
        {
            auto logger = GetLogger(module);
            if (logger) logger->info(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void ModuleWarn(const std::string& module, const char* fmt, Args&&... args)
        {
            auto logger = GetLogger(module);
            if (logger) logger->warn(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void ModuleError(const std::string& module, const char* fmt, Args&&... args)
        {
            auto logger = GetLogger(module);
            if (logger) logger->error(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void ModuleCritical(const std::string& module, const char* fmt, Args&&... args)
        {
            auto logger = GetLogger(module);
            if (logger) logger->critical(fmt, std::forward<Args>(args)...);
        }

        static void Flush(const std::string& module)
        {
            auto logger = GetLogger(module);
            if (logger) logger->flush();
        }

    private:
        static std::shared_ptr<spdlog::logger> s_logger;
        static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> s_ModuleLoggers;
        static std::mutex s_mutex;

        static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& strName,
            const std::string& strFilepath,
            bool bDaily,
            spdlog::level::level_enum spdLevel,
            spdlog::level::level_enum spdflushLevel);

        static spdlog::level::level_enum toLevel(const std::string& strLevel);

        static void EnsureDirectory(const std::string& strPath);
    };

} // namespace MyLib