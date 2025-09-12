#include "pch.h"

namespace MyLib
{
    std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;
    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::s_ModuleLoggers;
    std::mutex Logger::s_mutex;

    spdlog::level::level_enum Logger::toLevel(const std::string& strLevel)
    {
        if (strLevel == "trace") return spdlog::level::trace;
        if (strLevel == "debug") return spdlog::level::debug;
        if (strLevel == "info") return spdlog::level::info;
        if (strLevel == "warn") return spdlog::level::warn;
        if (strLevel == "error") return spdlog::level::err;
        if (strLevel == "critical") return spdlog::level::critical;
        return spdlog::level::info;
    }

    static std::wstring Utf8ToWide(const std::string& strUtf8)
    {
        int wideSize = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, nullptr, 0);
        if (wideSize == 0) return L"";
        std::wstring wideStr(wideSize, 0);
        MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, &wideStr[0], wideSize);
        return wideStr;
    }

    void Logger::EnsureDirectory(const std::string& strPath)
    {
        if (strPath.empty()) return;
        std::wstring widePath = Utf8ToWide(strPath);
        CreateDirectoryW(widePath.c_str(), nullptr);
    }

    std::shared_ptr<spdlog::logger> Logger::CreateLogger(
        const std::string& strName,
        const std::string& strFilepath,
        bool bDaily,
        spdlog::level::level_enum spdLevel,
        spdlog::level::level_enum spdflushLevel)
    {
        size_t pos = strFilepath.find_last_of("/\\");
        if (pos != std::string::npos)
        {
            std::string dir = strFilepath.substr(0, pos);
            EnsureDirectory(dir);
        }

        std::shared_ptr<spdlog::sinks::sink> file_sink;
        if (bDaily)
            file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(strFilepath, 0, 0);
        else
            file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(strFilepath, true);

        std::vector<spdlog::sink_ptr> sinks{ file_sink };
        sinks.push_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());

        auto logger = std::make_shared<spdlog::logger>(strName, sinks.begin(), sinks.end());
        logger->set_level(spdLevel);
        logger->flush_on(spdflushLevel);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [T%t] [%^%l%$] [%n] %v");

        spdlog::register_logger(logger);
        return logger;
    }

    void Logger::Init(const std::string& strName, const std::string& strFilepath, bool bDaily)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        try
        {
            s_logger = CreateLogger(strName, strFilepath, bDaily, spdlog::level::debug, spdlog::level::debug);
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            ::OutputDebugStringA(("Logger init failed: " + std::string(ex.what()) + "\n").c_str());
        }
    }

    void Logger::CreateModuleLogger(const std::string& strModuleName,
        const std::string& strFilepath,
        bool bDaily)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        try
        {
            auto logger = CreateLogger(strModuleName, strFilepath, bDaily, spdlog::level::debug, spdlog::level::debug);
            s_ModuleLoggers[strModuleName] = logger;
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            ::OutputDebugStringA(("Module logger init failed: " + std::string(ex.what()) + "\n").c_str());
        }
    }

    void Logger::InitFromIni(const std::string& iniPath)
    {
        mINI::INIFile file(iniPath);
        mINI::INIStructure ini;
        if (!file.read(ini)) {
            ::OutputDebugStringA(("Failed to read ini file: " + iniPath + "\n").c_str());
            return;
        }

        // [default] 로거
        if (ini.has("default")) {
            auto& section = ini["default"];
            std::string name = section.has("name") ? section["name"] : "Default";
            std::string filepath = section.has("filepath") ? section["filepath"] : "logs/default.log";
            bool daily = (section.has("daily") && section["daily"] == "true");
            auto level = toLevel(section.has("level") ? section["level"] : "info");
            auto flushLevel = toLevel(section.has("flush") ? section["flush"] : "info");

            s_logger = CreateLogger(name, filepath, daily, level, flushLevel);
        }

        // [module.xxx] 로거
        for (auto const& it : ini) {
            if (it.first.find("module.") == 0) {
                std::string moduleName = it.first.substr(strlen("module."));
                auto& section = ini[it.first];
                std::string filepath = section.has("filepath") ? section["filepath"] : ("logs/" + moduleName + ".log");
                bool daily = (section.has("daily") && section["daily"] == "true");
                auto level = toLevel(section.has("level") ? section["level"] : "info");
                auto flushLevel = toLevel(section.has("flush") ? section["flush"] : "info");

                s_ModuleLoggers[moduleName] = CreateLogger(moduleName, filepath, daily, level, flushLevel);
            }
        }
    }

    std::shared_ptr<spdlog::logger> Logger::GetLogger(const std::string& strModuleName)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        auto it = s_ModuleLoggers.find(strModuleName);
        if (it != s_ModuleLoggers.end())
            return it->second;
        return nullptr;
    }

    void Logger::SetAllLoggerLevel(spdlog::level::level_enum spdLevel)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        if (s_logger) s_logger->set_level(spdLevel);
        for (auto& kv : s_ModuleLoggers)
            kv.second->set_level(spdLevel);
    }

    void Logger::SetModuleLoggerLevel(const std::string& module, spdlog::level::level_enum spdLevel)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        auto logger = GetLogger(module);
        if (logger) logger->set_level(spdLevel);
    }

} // namespace MyLib