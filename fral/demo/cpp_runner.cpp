#include "../engine/engine.h"
#include <iostream>


const char *BIN_NAME = "demo.bin";
const int ENTRIES = 10;

int main(){

    fral::FRAL ral(BIN_NAME);

    for(int idx = 0;;){
        auto read_blob = (int *) ral.load(idx);
        if(read_blob){
            std::cout << "C++ received " << *read_blob << " from Python!" << std::endl;
            auto write_blob = (int *) ral.allocate(sizeof(int));
            *write_blob = *read_blob + 1;
            auto rIdx = ral.append(write_blob);
            if(rIdx == ENTRIES){
                break;
            }
            idx+=2;
        }
    }

}