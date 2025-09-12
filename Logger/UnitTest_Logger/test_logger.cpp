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

    void SetUp() override       // 각 Test 전 수행
    {
        // 로그 디렉토리 생성
        CreateDirectoryA("Logs", NULL);

        Logger::Init("TestDefault", strPathDefaultLogFile);
        Logger::CreateModuleLogger("TestModule", strPathModuleLogFile);
    }

    void TearDown() override    // 각 Test 후 수행
    {
        // 테스트 후 Log 파일 삭제
        DeleteFileA(strPathDefaultLogFile.c_str());
        DeleteFileA(strPathModuleLogFile.c_str());
    }

    // 파일 존재 여부 확인
    bool FileExists(const std::string& path) 
    {
        DWORD attr = GetFileAttributesA(path.c_str());
        return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
    }
};

// --------------------------
// Logger 체크
// --------------------------
TEST_F(LoggerTest, CheckLogger) 
{
    // auto defaultLogger; 기본 Logger 객체를 현재 구현으로는 받을 수 없어 추가 필요.. 
    auto moduleLogger = Logger::GetLogger("TestModule");

    //EXPECT_NE(defaultLogger, nullptr);
    EXPECT_NE(moduleLogger, nullptr);

    auto nonExist = Logger::GetLogger("NoModule");
    EXPECT_EQ(nonExist, nullptr);
}

// --------------------------
// Log Level 체크
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
// 포맷 스트링 테스트
// --------------------------
TEST_F(LoggerTest, FormatStringLogging) 
{
    EXPECT_NO_THROW(Logger::Info("{} Application started. Version {}", "GT300_OP", "1.0.2.0"));
    EXPECT_NO_THROW(Logger::Error("Error code: {}", 404));
    EXPECT_NO_THROW(Logger::ModuleInfo("TestModule", "Module {} initialized", "Detector"));
}

// --------------------------
// 멀티스레드 로그 테스트
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

    SUCCEED(); // crash 없이 완료 확인
}

// --------------------------
// 로그 파일 생성 확인
// --------------------------
TEST_F(LoggerTest, LogFileExistence) {
    Logger::Info("Test message");
    Logger::ModuleInfo("TestModule", "Module test message");

    EXPECT_TRUE(FileExists(strPathDefaultLogFile));
    EXPECT_TRUE(FileExists(strPathModuleLogFile));
}