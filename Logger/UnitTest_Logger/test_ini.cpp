#include "pch.h"
#include "Logger.h"
#include <fstream>

using namespace MyLib;

class LoggerIniTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        // 테스트용 ini 파일 생성
        std::ofstream ofs("test_logger.ini");
        ofs << "[default]\n";
        ofs << "name=Main\n";
        ofs << "filepath=test_main.log\n";
        ofs << "daily=false\n";
        ofs << "level=info\n";
        ofs << "flush=info\n\n";

        ofs << "[module.Detector]\n";
        ofs << "filepath=test_Detector.log\n";
        ofs << "daily=false\n";
        ofs << "level=debug\n";
        ofs << "flush=warn\n";
        ofs.close();

        Logger::InitFromIni("test_logger.ini");
    }
    void TearDown() override 
    {
        spdlog::drop_all();  // 등록된 모든 로거 해제
    }
};

TEST_F(LoggerIniTest, DefaultLoggerWritesToFile)
{
    Logger::Info("Hello {}", "World");

    std::ifstream ifs("test_main.log");
    std::string content((std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>()));

    EXPECT_NE(content.find("Hello World"), std::string::npos);
}

TEST_F(LoggerIniTest, ModuleLoggerWritesToFile)
{
    auto DetectorLogger = Logger::GetLogger("Detector");
    ASSERT_NE(DetectorLogger, nullptr);

    DetectorLogger->warn("Packet {}", 123);

    std::ifstream ifs("test_Detector.log");
    std::string content((std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>()));

    EXPECT_NE(content.find("Packet 123"), std::string::npos);
}

TEST_F(LoggerIniTest, SetLoggerLevelAffectsLoggers)
{
    Logger::SetAllLoggerLevel(spdlog::level::err);

    Logger::Info("This should not appear");

    std::ifstream ifs("test_main.log");
    std::string content((std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>()));

    EXPECT_EQ(content.find("This should not appear"), std::string::npos);
}