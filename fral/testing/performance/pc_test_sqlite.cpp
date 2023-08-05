#include <chrono>
#include <fstream>
#include <iostream>

#include "engine_sqlite.h"
#include "gflags/gflags.h"
#include <unistd.h>

using namespace std::chrono;

DEFINE_int32(gib, 1, "Provide number of MB to test");
DEFINE_int32(size, 100, "Enter size of entries");
DEFINE_int32(writers, 1, "Enter number of writers");
DEFINE_string(bin_name, "pc-test2.db", "Provide bin name");
DEFINE_string(csv_name, "pc-test2.csv", "Provide csv name for test results");

int main(int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    FRAL_SQLITE(FLAGS_bin_name.c_str(), true);
    size_t totalSize = static_cast<size_t>(FLAGS_gib) * 1000000;
    auto entries = totalSize / FLAGS_size;

    assert(entries % FLAGS_writers == 0);
    auto writerEntries = entries / FLAGS_writers;

    auto buffer = malloc(FLAGS_size);
    pid_t pid;

    for (int process = 0; process < FLAGS_writers + 1; process++) {
        pid = fork();
        if (pid == 0) {
            auto ral = FRAL_SQLITE(FLAGS_bin_name.c_str());
            // reader
            if (process == 0) {
                std::cout << "Starting Reader" << std::endl;
                for (int i = 0; i < entries;) {
                    auto blob = ral.load(i + 1);
                    if (blob) {
                        std::memcpy(buffer, blob, FLAGS_size);
                        i++;
                    }
                }
                auto stop = high_resolution_clock::now();

                high_resolution_clock::time_point start = stop;
                for (int i = 0; i < entries; i++) {
                    // get the first writer timestamp (yes this could be more eloquent)
                    auto start_ = (high_resolution_clock::time_point *) ral.load(1);
                    if (*start_ < start) {
                        start = *start_;
                    }
                }
                std::ofstream csvFile(FLAGS_csv_name);
                auto duration = duration_cast<nanoseconds>(stop - start).count();
                std::cout << "Finished Reader" << std::endl;
                csvFile << "Time,Entries,Writers,Size" << std::endl;
                csvFile << duration << "," << entries << "," << FLAGS_writers << ","
                        << FLAGS_size << std::endl;
                csvFile.close();

                std::cout << "Finished PC Test ~" << duration / entries << "ns"
                          << std::endl;
            } else {
                std::cout << "Starting Writer " << process << std::endl;
                sleep(3);  // enough time to start all processes

                *(high_resolution_clock::time_point *)buffer =
                        high_resolution_clock::now();

                for (int i = 0; i < writerEntries; i++) {
                    ral.append(buffer, FLAGS_size);
                }
                std::cout << "Finished Writer " << process << std::endl;
            }
            exit(0);
        }
    }
    for (int process = 0; process < FLAGS_writers + 1; process++) {
        wait(nullptr);
    }
}