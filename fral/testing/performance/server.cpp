#include "../../network/server.h"

#include <sys/wait.h>

#include <fstream>
#include <iostream>
#include <thread>

#include "gflags/gflags.h"
#include "utility.h"

// Writer runs on another machine
std::string HOST = "0.0.0.0";

using namespace std::chrono;

DEFINE_int32(gib, 1, "Provide number of GiB to test");
DEFINE_int32(size, 100, "Enter size of entries");
DEFINE_int32(entries, 10000, "Enter size of entries");
DEFINE_string(bin_name, "testS.bin", "Provide bin name");
DEFINE_string(port, "50051", "Provide port for listener");
DEFINE_string(csv_name, "net-test.csv", "Provide csv name for test results");

void listener(fral::server *server) { server->listenForKill(); }

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, false);

    auto entries = create(FLAGS_size, FLAGS_bin_name, FLAGS_gib, FLAGS_entries);

  auto pid = fork();

  if (pid == 0) {
    auto buffer = (char *)malloc(FLAGS_size);
    auto ralR = fral::FRAL(FLAGS_bin_name.c_str());
    ralR.primeCache();
    for (int i = 0; i < entries;) {
      auto blob = ralR.load(i);
      if (blob) {
        std::memcpy(buffer, blob, FLAGS_size);
        i++;
      }
    }

    auto stop = high_resolution_clock::now();
    auto start = (high_resolution_clock::time_point *)ralR.load(0);

    std::ofstream csvFile(FLAGS_csv_name);
    csvFile << "Time,Entries,Size" << std::endl;
    csvFile << duration_cast<nanoseconds>(stop - *start).count() << ","
            << entries << "," << FLAGS_size << std::endl;
    csvFile.close();
    std::cout << "Finished Reader" << std::endl;
    exit(0);
  }

  auto ralS = fral::FRAL(FLAGS_bin_name.c_str());
  ralS.primeCache();
  auto receiver = new fral::server(&ralS, FLAGS_port, HOST);
  std::thread listen(&listener, receiver);
  receiver->run();
  listen.join();

  wait(nullptr);
  std::cout << "net test complete" << std::endl;
}
