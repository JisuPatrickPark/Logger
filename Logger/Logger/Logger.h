#pragma once

#include <string>
#include <memory>           // shared memory
#include <unordered_map>    // 

#define FMT_UNICODE 0       // spdlog¿«fmt UNICODE ∏∑±‚ 
#include <spdlog/spdlog.h>

using namespace std;

namespace MyLib 
{
    class Logger 
    {
    public:
        static void Init(const string& name, const string& filepath, bool daily = false);
        static void CreateModuleLogger(const string& moduleName, const string& filepath, bool daily = false);
        static shared_ptr<spdlog::logger> GetLogger(const string& moduleName);
        static void SetLevel(spdlog::level::level_enum level);

        static void Trace(const string& msg);
        static void Debug(const string& msg);
        static void Info(const string& msg);
        static void Warn(const string& msg);
        static void Error(const string& msg);
        static void Critical(const string& msg);

    private:
        static shared_ptr<class spdlog::logger> s_logger;
        static unordered_map<string, shared_ptr<spdlog::logger>> s_moduleLoggers;
    };
}