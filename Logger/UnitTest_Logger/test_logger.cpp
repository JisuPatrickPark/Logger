#include "pch.h"
#include "Logger.h"
#include <windows.h>
#include <thread>

using namespace MyLib;
class LoggerTest : public ::testing::Test 
{
protected:
    std::string strPathDefaultLogFile = "Logs\\test_default.log";
    std::string strPathModuleLogFile = "Logs\\test_module.log";

    void SetUp() override       // �� Test �� ����
    {
        // �α� ���丮 ����
        CreateDirectoryA("Logs", NULL);

        Logger::Init("TestDefault", strPathDefaultLogFile);
        Logger::CreateModuleLogger("TestModule", strPathModuleLogFile);
    }

    void TearDown() override    // �� Test �� ����
    {
        // �׽�Ʈ �� Log ���� ����
        DeleteFileA(strPathDefaultLogFile.c_str());
        DeleteFileA(strPathModuleLogFile.c_str());
    }

    // ���� ���� ���� Ȯ��
    bool FileExists(const std::string& path) 
    {
        DWORD attr = GetFileAttributesA(path.c_str());
        return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
    }
};

// --------------------------
// Logger üũ
// --------------------------
TEST_F(LoggerTest, CheckLogger) 
{
    // auto defaultLogger; �⺻ Logger ��ü�� ���� �������δ� ���� �� ���� �߰� �ʿ�.. 
    auto moduleLogger = Logger::GetLogger("TestModule");

    //EXPECT_NE(defaultLogger, nullptr);
    EXPECT_NE(moduleLogger, nullptr);

    auto nonExist = Logger::GetLogger("NoModule");
    EXPECT_EQ(nonExist, nullptr);
}

// --------------------------
// Log Level üũ
// --------------------------
TEST_F(LoggerTest, ChangeLoggerLevel) 
{
    Logger::SetAllLoggerLevel(spdlog::level::warn);

    //auto defaultLogger
    auto moduleLogger = Logger::GetLogger("TestModule");

    //EXPECT_EQ(defaultLogger->level(), spdlog::level::warn);
    EXPECT_EQ(moduleLogger->level(), spdlog::level::warn);
}

// --------------------------
// ���� ��Ʈ�� �׽�Ʈ
// --------------------------
TEST_F(LoggerTest, FormatStringLogging) 
{
    EXPECT_NO_THROW(Logger::Info("{} Application started. Version {}", "GT300_OP", "1.0.2.0"));
    EXPECT_NO_THROW(Logger::Error("Error code: {}", 404));
    EXPECT_NO_THROW(Logger::ModuleInfo("TestModule", "Module {} initialized", "Detector"));
}

// --------------------------
// ��Ƽ������ �α� �׽�Ʈ
// --------------------------
TEST_F(LoggerTest, MultiThreadLogging) {
    auto logFunc = []() {
        for (int i = 0; i < 100; ++i) {
            Logger::Info("Thread logging {}", i);
            Logger::ModuleDebug("TestModule", "Thread logging {}", i);
        }
        };

    std::thread t1(logFunc);
    std::thread t2(logFunc);
    std::thread t3(logFunc);

    t1.join();
    t2.join();
    t3.join();

    SUCCEED(); // crash ���� �Ϸ� Ȯ��
}

// --------------------------
// �α� ���� ���� Ȯ��
// --------------------------
TEST_F(LoggerTest, LogFileExistence) {
    Logger::Info("Test message");
    Logger::ModuleInfo("TestModule", "Module test message");

    EXPECT_TRUE(FileExists(strPathDefaultLogFile));
    EXPECT_TRUE(FileExists(strPathModuleLogFile));
}