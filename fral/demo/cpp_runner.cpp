#include "../engine/engine.h"
#include <iostream>


const char *BIN_NAME = "demo.bin";
const int ENTRIES = 6;

int main(){

    fral::FRAL ral(BIN_NAME);

    for(int idx = 0;;){
        auto read_blob = (int *) ral.load(idx);
        if(read_blob){
            std::cout << "C++ received " << *read_blob << " from Python, ";
            if(idx == ENTRIES - 1){
                std::cout << "done!" << std::endl;
                break;
            }
            auto write_blob = (int *) ral.allocate(sizeof(int));
            *write_blob = *read_blob + 1;
            std::cout << "sending " << *write_blob << "!" << std::endl;
            idx = ral.append(write_blob) + 1;
            if(idx == ENTRIES){
                break;
            }
        }
    }

}