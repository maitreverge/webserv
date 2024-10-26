#include <gtest/gtest.h>
#include "Logger.hpp"

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
