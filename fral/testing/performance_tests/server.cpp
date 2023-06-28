#include "gflags/gflags.h"
#include "test_server.h"
#include "utility.h"
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <thread>

//Writer runs on another machine
std::string HOST = "0.0.0.0";

using namespace std::chrono;

DEFINE_int32(gib, 1, "Provide number of GiB to test");
DEFINE_int32(size, 100, "Enter size of entries");
DEFINE_int32(entries, 10000, "Enter size of entries");
DEFINE_string(bin_name, "testS.bin", "Provide bin name");
DEFINE_string(port, "50051", "Provide port for listener");
DEFINE_string(csv_name, "net-test.csv", "Provide csv name for test results");

void listener(testServer *server){
    server->listenForKill();
}

int main(int argc, char **argv){

    gflags::ParseCommandLineFlags(&argc, &argv, false);

    auto entries = create(FLAGS_size, FLAGS_bin_name, FLAGS_gib, FLAGS_entries);

    auto pid = fork();
    if(pid == 0){
        char buffer[FLAGS_size];
        auto ralR = fral::FRAL(FLAGS_bin_name.c_str());

        for(int i = 0; i < entries;){
            auto blob = ralR.load(i);
            if(blob){
                std::memcpy(buffer, blob, FLAGS_size);
                i++;
            }
        }
        auto stop = high_resolution_clock::now();
        auto blob1 = (netMessage *) ralR.load(0);
        auto blob2 = (netMessage *) buffer;

        std::ofstream csvFile(FLAGS_csv_name);
        csvFile << "WriteT,SendT,RecT,ReadT,Entries,Size" << std::endl;
        csvFile << convertTime(blob1->writeT) << ","
                << convertTime(blob2->sendT) << ","
                << convertTime(blob1->recT) << ","
                << convertTime(stop) << ","
                << entries << ","
                << FLAGS_size << std::endl;
        csvFile.close();
        std::cout << "Finished Reader" << std::endl;
        exit(0);

    }

    auto ralS = fral::FRAL(FLAGS_bin_name.c_str());
    auto receiver = new testServer(&ralS, FLAGS_port, HOST, entries);
    std::thread listen(&listener, receiver);
    receiver->run();
    listen.join();

    wait(nullptr);
    std::cout << "net test complete" << std::endl;
}
