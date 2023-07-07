#include <chrono>
#include <fstream>
#include <iostream>

#include "gflags/gflags.h"
#include "utility.h"

using namespace std::chrono;

DEFINE_int32(gib, 1, "Provide number of GiB to test");
DEFINE_int32(size, 100, "Provide entry size in bytes");
DEFINE_string(bin_name, "write-test.bin", "Provide bin name");
DEFINE_string(csv_name, "write-test.csv", "Provide csv name");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, false);

  auto entries = create<fral::FRAL>(FLAGS_size, FLAGS_bin_name, FLAGS_gib);

  fral::FRAL ral(FLAGS_bin_name.c_str());
  ral.primeCache();
  auto stream = malloc(FLAGS_size);

  auto start = high_resolution_clock::now();

  for (;;) {
    auto blob = ral.allocate(FLAGS_size);

    if (!blob) {
      break;
    }

    memcpy(blob, stream, FLAGS_size);
    ral.append(blob);
  }

  auto stop = high_resolution_clock::now();

  std::ofstream csvFile(FLAGS_csv_name);
  csvFile << "Time,Entries,Size" << std::endl;
  csvFile << duration_cast<nanoseconds>(stop - start).count() << "," << entries
          << "," << FLAGS_size << std::endl;
  csvFile.close();

  std::cout << "write test exe complete" << std::endl;
}
