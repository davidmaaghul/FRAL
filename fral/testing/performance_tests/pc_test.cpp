#include "utility.h"
#include "gflags/gflags.h"
#include <fstream>
#include <sys/wait.h>
#include <iostream>

DEFINE_int32(gib, 1, "Provide number of GiB to test");
DEFINE_int32(size, 100, "Enter size of entries");
DEFINE_int32(writers, 1, "Enter number of writers");
DEFINE_string(bin_name, "pc-test.bin", "Provide bin name");
DEFINE_string(csv_name, "pc-test.csv", "Provide csv name for test results");

int main(int argc, char** argv){

    gflags::ParseCommandLineFlags(&argc, &argv, false);

    auto entries = create(FLAGS_size, FLAGS_bin_name, FLAGS_gib);

    assert(entries % FLAGS_writers == 0);
    auto writerEntries = entries / FLAGS_writers;

    auto buffer = malloc(FLAGS_size);
    pid_t pid;

    for(int process = 0; process < FLAGS_writers + 1; process++){
        pid = fork();
        if(pid == 0){
            auto ral = fral::FRAL(FLAGS_bin_name.c_str());
            ral.primeCache();
            //reader
            if(process == 0){
                std::cout << "Starting Reader" << std::endl;

                for(int i = 0; i < entries;){
                    auto blob = ral.load(i);
                    if(blob){
                        std::memcpy(buffer, blob, FLAGS_size);
                        i++;
                    }
                }
                auto stop = high_resolution_clock::now();

                high_resolution_clock::time_point start = stop;
                for(int i = 0; i < entries; i++){
                    // get the first writer timestamp (yes this could be more eloquent)
                    auto start_ = (high_resolution_clock::time_point *) ral.load(i);
                    if(*start_ < start){
                        start = *start_;
                    }
                }
                std::ofstream csvFile(FLAGS_csv_name);
                auto duration = duration_cast<nanoseconds>(stop - start).count();
                csvFile << "Time,Entries,Writers,Size" << std::endl;
                csvFile << duration << "," << entries << "," << FLAGS_writers << "," << FLAGS_size << std::endl;
                csvFile.close();
                std::cout << "Finished PC Test ~" << duration / entries << "ns" << std::endl;
            }
            else{
                std::cout << "Starting Writer " << process << std::endl;
                sleep(3); // enough time to start all processes

                *(high_resolution_clock::time_point *) buffer = high_resolution_clock::now();

                for(int i = 0; i < writerEntries; i++){
                    auto blob = ral.allocate(FLAGS_size);
                    std::memcpy(blob, buffer, FLAGS_size);
                    ral.append(blob);
                }
                std::cout << "Finished Writer " << process << std::endl;
            }
            exit(0);
        }
    }
    for(int process = 0; process < FLAGS_writers + 1; process++) {
        wait(nullptr);
    }
}