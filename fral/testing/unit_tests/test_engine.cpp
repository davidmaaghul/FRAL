#include <gtest/gtest.h>
#include "../../engine/engine.h"


TEST(FRAL, ALLOCATE){
    const int ENTRIES = 10;
    fral::FRAL home("test.bin", 1000, ENTRIES);
    int currIndex;
    int * intBlob;
    for (int i = 0; i < ENTRIES + 1; i++){
        intBlob = (int *) home.allocate(sizeof(int));
        *intBlob = i;
        currIndex = home.append(intBlob);
    }
    EXPECT_EQ(currIndex , -1);
}

TEST(FRAL, ALLOCATE_APPEND_LOAD){
    fral::FRAL home("test.bin", 1000, 10);

    int * intBlob = (int *) home.allocate(sizeof(int));
    int * intBlob2 = (int *) home.allocate(sizeof(int));

    *intBlob = 5;
    *intBlob2 = 6;

    home.append(intBlob);
    home.append(intBlob2);

    EXPECT_EQ( *(int *) home.load(0), 5 );
    EXPECT_EQ( *(int *) home.load(1), 6 );
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

