#include <gtest/gtest.h>
#include "../../engine/engine.h"
#include <filesystem>

const char * BIN_NAME = "test.bin";

class FRALTest : public ::testing::Test {
protected:
    fral::FRAL *test;
    void SetUp(size_t size, size_t entries) {
        test = new fral::FRAL(BIN_NAME, size, entries);
    }
    void TearDown() override {
        delete test;
        std::filesystem::remove(BIN_NAME);
    }
};

TEST_F(FRALTest, TEST_INIT){
    SetUp(1000, 10);
    ASSERT_EQ(0, test->size());
    ASSERT_EQ(10, test->maxSize());
    ASSERT_EQ(1000, test->maxMemory());
}

TEST_F(FRALTest, TEST_CONNECT){
    SetUp(1000, 10);
    fral::FRAL test2(BIN_NAME);
    ASSERT_EQ(0, test2.size());
    ASSERT_EQ(10, test2.maxSize());
    ASSERT_EQ(1000, test2.maxMemory());
}


TEST_F(FRALTest, TEST_ALLOCATE_FAIL){
    SetUp(1000, 10);
    ASSERT_EQ(nullptr, test->allocate(1801));
}

TEST_F(FRALTest, TEST_APPEND){
    SetUp(1000, 10);
    auto blob = test->allocate(10);
    ASSERT_EQ(0, test->append(blob));
}

TEST_F(FRALTest, TEST_APPEND_FAIL){
    SetUp(1000, 1);
    auto blob = test->allocate(10);
    test->append(blob);
    ASSERT_EQ(-1, test->append(blob));
}

TEST_F(FRALTest, TEST_GET_BLOB_SIZE){
    SetUp(1000, 10);
    auto blob = test->allocate(10);
    ASSERT_EQ(10, fral::FRAL::getBlobSize(blob));
}

TEST_F(FRALTest, TEST_ALLOCATE_APPEND_READ){
    SetUp(1000, 10);
    auto blob = (int *) test->allocate(sizeof(int));
    *blob = 500023;
    auto idx = test->append(blob);
    ASSERT_EQ(500023, * (int *) test->load(idx));
}

TEST_F(FRALTest, TEST_CONNECT_ALLOCATE_APPEND_READ_BRACKETS){
    SetUp(1000, 10);
    auto test2 = fral::FRAL(BIN_NAME);
    auto blob = (int *) test2.allocate(sizeof(int));
    *blob = 500023;
    auto idx = test2.append(blob);
    ASSERT_EQ(500023, * (int *) test2[idx]);
}

TEST_F(FRALTest, TEST_ALLOCATE_APPEND_READ_STRUCT){

    struct testData {
        int a;
        int b;
        int c;
    };


    SetUp(100000, 100);
    std::vector<testData> testVec;

    auto testOne = testData{1, 2, 3};
    testVec.push_back(testOne);

    for(size_t k = 1; k < 100; ++k){
        auto lastData = testVec[k-1];
        auto nextData = testData{lastData.a + 1, lastData.b + 1, lastData.c + 1};
        testVec.push_back(nextData);
    }


    size_t counter = 0;

    for(auto & entry : testVec){
        auto blob = (testData *) test->allocate(sizeof(testData));
        *blob = entry;
        ASSERT_EQ(sizeof(testData), fral::FRAL::getBlobSize(blob));
        auto idx = test->append(blob);
        ASSERT_EQ(counter++, idx);
    }

    ASSERT_EQ(100, test->size());

    for(size_t k = 0; k < 100; k++){
        auto blob1 = testVec[k];
        auto blob2 = (testData *) (*test)[k];
        ASSERT_EQ(blob1.a, blob2->a);
        ASSERT_EQ(blob1.b, blob2->b);
        ASSERT_EQ(blob1.c, blob2->c);
    }

}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

