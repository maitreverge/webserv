#include <gtest/gtest.h>
#include "Logger.hpp" // Assurez-vous que le chemin est correct

class LoggerTest : public ::testing::Test {
protected:
    Logger logger;

    // Vous pouvez ajouter des initialisations communes ici si nécessaire
    void SetUp() override {
        // Initialisation avant chaque test
    }

    void TearDown() override {
        // Nettoyage après chaque test
    }
};

TEST_F(LoggerTest, InstanceIsUnique) {
    Logger& instance1 = Logger::getInstance();
    Logger& instance2 = Logger::getInstance();
    EXPECT_EQ(&instance1, &instance2); // Vérifie que les deux instances sont identiques
}

TEST_F(LoggerTest, FormatLogLevel) {
    EXPECT_EQ(logger.formatLogLevel(INFO), "\033[32m[INFO] \033[0m");
    EXPECT_EQ(logger.formatLogLevel(DEBUG), "\033[34m[DEBUG] \033[0m");
    EXPECT_EQ(logger.formatLogLevel(WARNING), "\033[33m[WARNING] \033[0m");
    EXPECT_EQ(logger.formatLogLevel(ERROR), "\033[31m[ERROR] \033[0m");
}

TEST_F(LoggerTest, LogMessages) {
    std::ostringstream oss;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(oss.rdbuf()); // Redirige std::cout vers oss

    logger.log(INFO, "This is an info message.");
    logger.log(DEBUG, "This is a debug message.");
    logger.log(WARNING, "This is a warning message.");
    logger.log(ERROR, "This is an error message.");

    std::cout.rdbuf(oldCoutBuffer); // Restaure le buffer original

    std::string output = oss.str();
    EXPECT_NE(output.find("This is an info message."), std::string::npos);
    EXPECT_NE(output.find("This is a debug message."), std::string::npos);
    EXPECT_NE(output.find("This is a warning message."), std::string::npos);
    EXPECT_NE(output.find("This is an error message."), std::string::npos);
}

TEST_F(LoggerTest, IpToString) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.1");
    addr.sin_port = htons(8080);

    EXPECT_EQ(logger.ipToString(addr), "192.168.1.1");
}

TEST_F(LoggerTest, PortToInt) {
    struct sockaddr_in addr;
    addr.sin_port = htons(8080);
    EXPECT_EQ(logger.portToInt(addr), 8080);
}
