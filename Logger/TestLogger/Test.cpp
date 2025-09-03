#include "Logger.h"
#include <iostream>

using namespace MyLib;

int main() 
{
    //MyLib::Logger::Info("0x\x41\x27 쁘로토콜");
    //MyLib::Logger::Debug("벌\r레");
    //MyLib::Logger::Warn("월\n월");
    //MyLib::Logger::Error("러\t러\t럴");
    //MyLib::Logger::Critical("\"Critical\" Log");

    //MyLib::Logger::CreateModuleLogger("SDK", "log\\SDK.log", false);
    //auto sdkLogger = MyLib::Logger::GetLogger("SDK");
    //if (sdkLogger) 
    //{
    //    sdkLogger->info("SDK_Module_Log");
    //    sdkLogger->warn("WarnLog::BrokenSDK");
    //}

    //MyLib::Logger::CreateModuleLogger("DB", "log\\DB.log", false);
    //auto dbLogger = MyLib::Logger::GetLogger("Audio");
    //if (dbLogger) 
    //{
    //    dbLogger->info("");
    //    dbLogger->error("WarnLog::BrokenSDK");
    //}

    // MyLib::Logger::SetLevel(spdlog::level::warn);    spdlog 외부라이브러리 포함 안하려면 enum 따로 써야되게따
    //MyLib::Logger::Info("Not Print");
    //MyLib::Logger::Warn("Print");

    return 0;
}