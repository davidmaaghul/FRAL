#include "../../network/client.h"
#include <iostream>
#include "thread"
#include "gflags/gflags.h"
#include "utility.h"

DEFINE_int32(gib, 1, "Provide number of GiB to test");
DEFINE_int32(entries, 10000, "Enter size of entries");
DEFINE_string(port, "50051", "Provide port for sending");
DEFINE_string(bin_name, "testC.bin", "Provide bin name");
DEFINE_int32(size, 100, "Enter size of entries");

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, false);

  auto entries = create(FLAGS_size, FLAGS_bin_name, FLAGS_gib, FLAGS_entries);

  auto pid = fork();

  if (pid == 0) {
    std::cout << "Starting Client" << std::endl;
    fral::FRAL ralC(FLAGS_bin_name.c_str());
    ralC.primeCache();
    fral::client client(&ralC, FLAGS_port, "localhost");
    client.sync(entries);
    std::cout << "Finished sending" << std::endl;
    auto exitCode = client.shutdown();
    std::cout << "Shut down server with exit code " << exitCode << std::endl;
    exit(0);
  }

  sleep(5);  // enough time for server to spin uip
  auto stream = (char *)malloc(FLAGS_size);
  fral::FRAL ralW(FLAGS_bin_name.c_str());
  ralW.primeCache();
  std::cout << "Starting Writer" << std::endl;
  *(high_resolution_clock::time_point *)stream = high_resolution_clock::now();
  for (int i = 0; i < entries; i++) {
    auto blob = ralW.allocate(FLAGS_size);
    std::memcpy(blob, stream, FLAGS_size);
    ralW.append(blob);
  }
  std::cout << "Finished Writer" << std::endl;
  wait(nullptr);
}
