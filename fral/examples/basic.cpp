#include "../engine/engine.h"
#include <iostream>

int main() {

    //Process 1
    auto ralA = fral::FRAL("test.bin", 1000, 10);
    const char *TEST_STR = "TEST";

    auto blob = (char *) ralA.allocate(strlen(TEST_STR));
    strcpy(blob, TEST_STR);
    printf("%s\n", blob);

    ralA.append(blob);


    //Process 2
    auto ralB = fral::FRAL("test.bin");

    auto blob2 = (char *) ralB.load(0);
    printf("%s\n", blob2);
    blob2[0] = 'D';

    auto blob3 = (char *) ralB.load(0);
    printf("%s\n", blob3);
}
