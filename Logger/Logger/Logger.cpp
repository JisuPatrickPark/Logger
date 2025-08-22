#include "pch.h"
#include "framework.h"

#include "Logger.h"
#include <fstream>

namespace MyLib 
{
    // --------------------------
    // static 멤버 정의
    // - 기본 Logger
    // - Logger 담는 ordered_map
    // --------------------------
    std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;
    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::s_ModuleLoggers;

    // --------------------------
    // 문자열로 spdlog 레벨 변환
    // --------------------------
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

    // --------------------------
    // Logger 생성 공용 함수
    // --------------------------
    std::shared_ptr<spdlog::logger> Logger::CreateLogger(const std::string& strName, const std::string& strFilepath, bool bDaily, spdlog::level::level_enum spdLevel)
    {
        // MSVC 디버그 콘솔용 sink (디버깅용)
        //auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();

        // 파일 sink 선택
        std::shared_ptr<spdlog::sinks::sink> file_sink;
        if (bDaily)
            file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(strFilepath, 0, 0);
        else 
            file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(strFilepath, true);

        // sink들을 vector에 넣고 logger 생성
        // std::vector<spdlog::sink_ptr> sinks{ debug_sink, file_sink };
        std::vector<spdlog::sink_ptr> sinks{ file_sink };
        auto logger = std::make_shared<spdlog::logger>(strName, sinks.begin(), sinks.end());

        logger->set_level(spdLevel);                     // 로그 레벨 적용
        logger->flush_on(spdlog::level::info);           // info 이상이면 즉시 flush
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [T%t] [%^%l%$] [%n] %v"); // 로그 기록 포맷

        spdlog::register_logger(logger);           // 전역 등록 (같은 name Logger 들어가면 기존꺼 날아감)
        return logger;
    }

    // --------------------------
    // 기본 로거 초기화
    // --------------------------
    void Logger::Init(const std::string& strName, const std::string& strFilepath, bool bDaily = false)
    {
        try 
        {
            s_logger = CreateLogger(strName, strFilepath, bDaily, spdlog::level::debug);
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            ::OutputDebugStringA(("Logger init failed: " + std::string(ex.what()) + "\n").c_str());
        }
    }

    // --------------------------
    // 모듈별 로거 생성
    // --------------------------
    void Logger::CreateModuleLogger(const std::string& strModuleName, const std::string& strFilepath, bool bDaily) 
    {
        try 
        {
            auto logger = CreateLogger(strModuleName, strFilepath, bDaily, spdlog::level::debug);
            s_ModuleLoggers[strModuleName] = logger;
        }
        catch (const spdlog::spdlog_ex& ex) 
        {
            ::OutputDebugStringA(("Module logger init failed: " + std::string(ex.what()) + "\n").c_str());
        }
    }

    // --------------------------
    // 모듈 이름으로 Logger 가져오기
    // --------------------------
    std::shared_ptr<spdlog::logger> Logger::GetLogger(const std::string& strModuleName) 
    {
        auto mLogger = s_ModuleLoggers.find(strModuleName);
        if (mLogger != s_ModuleLoggers.end())
            return mLogger->second;
        return nullptr;
    }

    // --------------------------
    // 전체 로그 레벨 변경
    // --------------------------
    void Logger::SetLevel(spdlog::level::level_enum spdLevel) 
    {
        if (s_logger) 
            s_logger->set_level(spdLevel);
        for (auto& mLogger : s_ModuleLoggers) 
            mLogger.second->set_level(spdLevel);
    }

    // --------------------------
    // JSON 설정 파일 기반 초기화
    // --------------------------
    //void Logger::InitFromJson(const std::string& configPath) {
    //    try {
    //        std::ifstream file(configPath);
    //        if (!file.is_open()) throw std::runtime_error("Config file not found: " + configPath);

    //        json config;
    //        file >> config; // JSON 파싱

    //        // 기본 로거
    //        if (config.contains("default")) {
    //            auto def = config["default"];
    //            std::string name = def.value("name", "AppLogger");
    //            std::string path = def.value("file", "app.log");
    //            bool daily = def.value("daily", false);
    //            auto level = toLevel(def.value("level", "info"));
    //            s_logger = createLogger(name, path, daily, level);
    //        }

    //        // 모듈별 로거
    //        if (config.contains("modules")) {
    //            for (auto& m : config["modules"]) {
    //                std::string name = m.value("name", "Module");
    //                std::string path = m.value("file", name + ".log");
    //                bool daily = m.value("daily", false);
    //                auto level = toLevel(m.value("level", "info"));
    //                s_moduleLoggers[name] = createLogger(name, path, daily, level);
    //            }
    //        }

    //    }
    //    catch (const std::exception& ex) {
    //        ::OutputDebugStringA(("Logger JSON config load failed: " + std::string(ex.what()) + "\n").c_str());
    //    }
    //}

    // --------------------------
    // INI 설정 파일 기반 초기화
    // --------------------------
    //void Logger::InitFromIni(const std::string& configPath) 
    //{
    //    try 
    //    {
    //        CSimpleIniA ini;
    //        ini.SetUnicode();
    //        SI_Error rc = ini.LoadFile(configPath.c_str());
    //        if (rc < 0) 
    //            throw std::runtime_error("Config file not found: " + configPath);

    //        // 기본 로거
    //        std::string name = ini.GetValue("default", "name", "AppLogger");
    //        std::string path = ini.GetValue("default", "file", "app.log");
    //        bool daily = ini.GetBoolValue("default", "daily", false);
    //        std::string levelS = ini.GetValue("default", "level", "info");
    //        auto level = toLevel(levelS);
    //        s_logger = createLogger(name, path, daily, level);

    //        // 모듈별 로거
    //        CSimpleIniA::TNamesDepend sections;
    //        ini.GetAllSections(sections);
    //        for (auto& sec : sections) 
    //        {
    //            std::string s = sec.pItem;
    //            if (s.rfind("module.", 0) == 0) // "module."로 시작하면
    //            { 
    //                std::string moduleName = s.substr(7); // 접두사 제거
    //                std::string path = ini.GetValue(s.c_str(), "file", (moduleName + ".log").c_str());
    //                bool daily = ini.GetBoolValue(s.c_str(), "daily", false);
    //                std::string levelS = ini.GetValue(s.c_str(), "level", "info");
    //                auto level = toLevel(levelS);
    //                s_moduleLoggers[moduleName] = createLogger(moduleName, path, daily, level);
    //            }
    //        }

    //    }
    //    catch (const std::exception& ex) {
    //        ::OutputDebugStringA(("Logger INI config load failed: " + std::string(ex.what()) + "\n").c_str());
    //    }
    //}

    // --------------------------
    // 기본 로거 단축 로그 함수
    // --------------------------
    void Logger::Trace(const std::string& strMsg) { if (s_logger) s_logger->trace(strMsg); }
    void Logger::Debug(const std::string& strMsg) { if (s_logger) s_logger->debug(strMsg); }
    void Logger::Info(const std::string& strMsg) { if (s_logger) s_logger->info(strMsg); }
    void Logger::Warn(const std::string& strMsg) { if (s_logger) s_logger->warn(strMsg); }
    void Logger::Error(const std::string& strMsg) { if (s_logger) s_logger->error(strMsg); }
    void Logger::Critical(const std::string& strMsg) { if (s_logger) s_logger->critical(strMsg); }

} // namespace MyLib