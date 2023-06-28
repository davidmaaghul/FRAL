#ifndef FRAL_TESTING_PERFORMANCE_TESTS_TEST_CLIENT_H
#define FRAL_TESTING_PERFORMANCE_TESTS_TEST_CLIENT_H

#include <grpcpp/grpcpp.h>
#include "../../network/client.h"
#include "../../engine/engine.h"
#include "utility.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::WriteOptions;

using medium::Medium;
using medium::Empty;
using medium::Allocation;
using medium::Start;

class testClient : public fral::client {
public:

    testClient(fral::FRAL *ral, const std::string& port, const std::string& host)
        : client(ral, port, host) {}


    int sync(const int &end) override {
        ClientContext context;
        Allocation alloc;
        Empty complete;

        auto nextIdx = connect();
        if((nextIdx == -1) || (nextIdx >= end)){
            //Unable to connect or portion already allocated
            return -1;
        }

        std::unique_ptr<grpc::ClientWriter<Allocation>>stream(stub->sync(&context, &complete));
        while(nextIdx < end){
            auto blob = ral->load(nextIdx);
            if(blob){
                if(nextIdx == end - 1){
                    ((netMessage *) blob)->sendT = high_resolution_clock::now();
                }
                nextIdx++;
                alloc.set_idx(nextIdx);
                alloc.set_allocation((char*) blob, fral::FRAL::getBlobSize(blob));
                if(!stream->Write(alloc, WriteOptions().set_no_compression().set_buffer_hint())){
                    std::cout << "Stream is closed" << std::endl;
                    return -1;
                }
            }
        }
        stream->WritesDone();
        stream->Finish();
        return nextIdx - 1;
    }
};



#endif
