#pragma once

#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <sstream>
#include <string>
#include "../../src/RequestParser/RequestParser.hpp"  // Inclure le bon header de ta classe
#include "../../src/kernel/Server.hpp"  // Inclure le bon header de ta classe


// Mock de Logger pour éviter d'écrire dans la sortie pendant les tests
class MockLogger {
	friend class RequestParser;
	friend class RequestParserTest;
public:
    static void log(int level, const std::string& message, const RequestParser& request) {
        // On peut simplement afficher dans la console ou ne rien faire pour les tests
        std::cout << "LOG: " << message << std::endl;
    }
};

class RequestParserTest : public ::testing::Test {
		friend class RequestParser;
	protected:
    RequestParser parser;  // Instance de RequestParser à tester

    void SetUp() override {
        // Optionnel : réinitialiser l'état de parser avant chaque test
    }

    void TearDown() override {
        // Optionnel : nettoyer après chaque test
    }
};
