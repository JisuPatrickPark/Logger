#include "Logger.h"
#include <iostream>
#include <ctime>

using namespace MyLib;

int main() 
{
    Logger::Init("System", "Log\\System.log");
    Logger::Info("0x\x42\x45 �ڷ�����");
    Logger::Debug("��\r��");
    Logger::Warn("��\n��");
    Logger::Error("��\t��\t��");
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

    //Logger::SetLevel(spdlog::level::warn);    //spdlog �ܺζ��̺귯�� ���� ���Ϸ��� enum ���� ��ߵǰԵ�
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

// 1. �α� ��� ������ spdlog api ���� ȣ������ ����
// 2. LogLevel�� enum�� ���� 
// 3. Configuration ���� ���ؼ� ������ �� �ֵ��� �ϱ� ini ���ܿ�� json �غ���
