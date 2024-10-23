#include <gtest/gtest.h>

// Test simple pour vérifier le fonctionnement du framework
TEST(SimpleTest, BasicAssertions) {
    EXPECT_EQ(1, 1);         // Vérifie que 1 est égal à 1
    EXPECT_TRUE(true);      // Vérifie que true est vrai
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
