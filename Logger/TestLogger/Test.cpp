#include "Logger.h"
#include <iostream>

using namespace MyLib;

int main() 
{
    MyLib::Logger::Info("0x\x41\x27 �ڷ�����");
    MyLib::Logger::Debug("��\r��");
    MyLib::Logger::Warn("��\n��");
    MyLib::Logger::Error("��\t��\t��");
    MyLib::Logger::Critical("\"Critical\" Log");

    MyLib::Logger::CreateModuleLogger("SDK", "log\SDK.log", false);
    auto sdkLogger = MyLib::Logger::GetLogger("SDK");
    if (sdkLogger) 
    {
        sdkLogger->info("SDK_Module_Log");
        sdkLogger->warn("WarnLog::BrokenSDK");
    }

    MyLib::Logger::CreateModuleLogger("DB", "log\DB.log", false);
    auto dbLogger = MyLib::Logger::GetLogger("Audio");
    if (dbLogger) 
    {
        dbLogger->info("");
        dbLogger->error("WarnLog::BrokenSDK");
    }

    // MyLib::Logger::SetLevel(spdlog::level::warn);    spdlog �ܺζ��̺귯�� ���� ���Ϸ��� enum ���� ��ߵǰԵ�
    MyLib::Logger::Info("Not Print");
    MyLib::Logger::Warn("Print");

    return 0;
}