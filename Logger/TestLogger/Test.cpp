#include "Logger.h"
#include <iostream>
#include <ctime>

using namespace MyLib;

int main() 
{
    Logger::Init("System", "Log\\System.log");
    Logger::Info("0x\x42\x45 쁘로토콜");
    Logger::Debug("벌\r레");
    Logger::Warn("월\n월");
    Logger::Error("러\t러\t럴");
    Logger::Critical("\"Critical\" Log");

    Logger::CreateModuleLogger("App", "Log\\App.log", true);
    auto AppLogger = Logger::GetLogger("App");
    if (AppLogger)
    {
        AppLogger->info("APP Log");
        AppLogger->critical("Application Start");
    }

    Logger::CreateModuleLogger("SDK", "Log\\logSDK.log", false);
    auto sdkLogger = Logger::GetLogger("SDK");
    if (sdkLogger) 
    {
        sdkLogger->info("SDK_Module_Log");
        sdkLogger->warn("WarnLog::BrokenSDK");
    }

    Logger::CreateModuleLogger("DB", "Log\\logDB.log", false);
    auto dbLogger = Logger::GetLogger("DB");
    if (dbLogger) 
    {
        dbLogger->info("DB Open");
        dbLogger->error("WarnLog::BrokenSDK");
    }

    //Logger::SetLevel(spdlog::level::warn);    //spdlog 외부라이브러리 포함 안하려면 enum 따로 써야되게따
    Logger::Info("Not Print");
    Logger::Warn("Print");

    float fSec = 5;
    clock_t tDelay = fSec * CLOCKS_PER_SEC;        
    clock_t tStart = clock();

    while (clock() - tStart < tDelay)
    {
        Logger::Info("Die");
    }

    return 0;
}

// 1. 로그 찍는 곳에서 spdlog api 직접 호출하지 말기
// 2. LogLevel용 enum은 따로 
// 3. Configuration 파일 통해서 조절할 수 있도록 하기 ini 지겨우니 json 해보자
