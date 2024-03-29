#include <chrono>
#include <fstream>
#include <iostream>

#include "gflags/gflags.h"
#include "engine_sqlite.h"

using namespace std::chrono;

DEFINE_double(gib, 1, "Provide number of GiB to test");
DEFINE_int32(size, 100, "Provide entry size in bytes");
DEFINE_string(bin_name, "write-test2.db", "Provide bin name");
DEFINE_string(csv_name, "write-test2.csv", "Provide csv name");

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, false);

    size_t totalSize = static_cast<size_t>(FLAGS_gib * 1000000000);
    auto entries = totalSize / FLAGS_size;
    FRAL_SQLITE ral(FLAGS_bin_name.c_str(), true);

    auto stream = (char *) malloc(FLAGS_size);

    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < entries; i++) {
        ral.append(stream, FLAGS_size);
    }

    auto stop = high_resolution_clock::now();

    std::ofstream csvFile(FLAGS_csv_name);
    csvFile << "Time,Entries,Size" << std::endl;
    csvFile << duration_cast<nanoseconds>(stop - start).count() << "," << entries
            << "," << FLAGS_size << std::endl;
    csvFile.close();

    std::cout << "write test exe complete" << std::endl;
}
