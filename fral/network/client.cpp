#include "client.h"
#include <memory>
#include <iostream>
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using medium::Medium;
using medium::Empty;
using medium::Allocation;
using medium::Start;

namespace fral {

    client::client(fral::FRAL *ral, const std::string &port, const std::string &host) : ral(ral) {
        std::string loc = host + ":" + port;
        channel = grpc::CreateChannel(loc, grpc::InsecureChannelCredentials());
        stub = Medium::NewStub(channel);
    }

    int client::connect() {
        ClientContext context;
        Empty hello;
        Start start;
        Status status = stub->connect(&context, hello, &start);
        if (!status.ok()) {
            std::cout << "failed to connect" << std::endl;
            return -1;
        }
        return start.idx();
    }

    int client::sync(const int &end) {

        ClientContext context;
        Allocation alloc;
        Empty complete;

        auto nextIdx = connect();

        if ((nextIdx == -1) || (nextIdx >= end)) {
            return -1;
        }

        std::unique_ptr <grpc::ClientWriter<Allocation>> stream(stub->sync(&context, &complete));
        while (nextIdx < end) {
            auto blob = ral->load(nextIdx);
            if (blob) {
                nextIdx++;
                alloc.set_idx(nextIdx);
                alloc.set_allocation((char *) blob, fral::FRAL::getBlobSize(blob));
                if (!stream->Write(alloc)) {
                    std::cout << "Stream is closed" << std::endl;
                    return -1;
                }
            }
        }

        stream->WritesDone();
        stream->Finish();
        return nextIdx - 1;
    }

    int client::shutdown() {
        ClientContext context;
        Empty finish;
        Empty ok;
        Status status = stub->shutdown(&context, finish, &ok);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }
}
