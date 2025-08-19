#pragma once

#include <string>
#include <memory>           // shared memory
#include <unordered_map>    // 

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>

namespace MyLib 
{
    class Logger 
    {
    public:
        static void Init(const std::string& name, const std::string& filepath, bool daily = false);
        static void CreateModuleLogger(const std::string& moduleName, const std::string& filepath, bool daily = false);
        static std::shared_ptr<spdlog::logger> GetLogger(const std::string& moduleName);
        static void SetLevel(spdlog::level::level_enum level);

        static void Trace(const std::string& msg);
        static void Debug(const std::string& msg);
        static void Info(const std::string& msg);
        static void Warn(const std::string& msg);
        static void Error(const std::string& msg);
        static void Critical(const std::string& msg);

        // json - nlohmann , ini - mini or simpleini
        //static void InitFromJson(const std::string& configPath);
        //static void InitFromIni(const std::string& configPath);

    private:
        static std::shared_ptr<class spdlog::logger> s_logger;
        static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> s_moduleLoggers;
    };
}