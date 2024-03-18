#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
    
    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);

    // Run all tests
    int ret = RUN_ALL_TESTS();

    // Return the status
    return ret;
}