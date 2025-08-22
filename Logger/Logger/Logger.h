#pragma once
#include <memory>                                // std::shared_ptr
#include <string>                                // std::string
#include <unordered_map>                         // 모듈별 로거 관리용
#define FMT_UNICODE 0                            // fmt 유니코드 지원 비활성화
#include <spdlog/spdlog.h>                       // spdlog 핵심
#include <spdlog/sinks/basic_file_sink.h>        // 기본 파일 sink
#include <spdlog/sinks/daily_file_sink.h>        // daily 파일 sink
#include <spdlog/sinks/msvc_sink.h>              // MSVC 디버그 콘솔용 sink
//#include <nlohmann/json.hpp>                   // JSON 파싱용
#include <SimpleIni.h>                           // INI 파싱용

// JSON 편의 alias
//using json = nlohmann::json;

namespace MyLib {

    class Logger {
    public:
        // --------------------------
        // 기본 로거 초기화
        // --------------------------
        // name: 로거 이름
        // filepath: 로그 파일 경로
        // daily: 매일 새로운 파일로 기록 여부
        static void Init(const std::string& strName, const std::string& strFilepath, bool bDaily = false);

        // --------------------------
        // 모듈별 로거 생성
        // --------------------------
        static void CreateModuleLogger(const std::string& strModuleName, const std::string& strFilepath, bool bDaily = false);

        // --------------------------
        // 모듈 이름으로 로거 가져오기
        // --------------------------
        static std::shared_ptr<spdlog::logger> GetLogger(const std::string& strModuleName);

        // --------------------------
        // 전체 로그 레벨 변경
        // --------------------------
        static void SetLevel(spdlog::level::level_enum spdLevel);

        // --------------------------
        // JSON 설정 파일 기반 초기화
        // --------------------------
        //static void InitFromJson(const std::string& configPath);

        // --------------------------
        // INI 설정 파일 기반 초기화
        // --------------------------
        //static void InitFromIni(const std::string& configPath);

        // --------------------------
        // 기본 로거 단축 로그 함수
        // --------------------------
        static void Trace(const std::string& strMsg);
        static void Debug(const std::string& strMsg);
        static void Info(const std::string& strMsg);
        static void Warn(const std::string& strMsg);
        static void Error(const std::string& strMsg);
        static void Critical(const std::string& strMsg);

    private:
        // --------------------------
        // 기본 로거 (한 개)
        // --------------------------
        static std::shared_ptr<spdlog::logger> s_logger;

        // --------------------------
        // 모듈별 로거 map
        // --------------------------
        static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> s_ModuleLoggers;

        // --------------------------
        // 내부 공용 logger 생성 함수
        // --------------------------
        static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& strName, const std::string& strFilepath, bool bDaily, spdlog::level::level_enum spdLevel);

        // 문자열 레벨 → spdlog enum 변환
        static spdlog::level::level_enum toLevel(const std::string& strLevel);
    };

} // namespace MyLib