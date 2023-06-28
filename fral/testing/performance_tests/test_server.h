#ifndef FRAL_TESTING_PERFORMANCE_TESTS_TEST_SERVER_H
#define FRAL_TESTING_PERFORMANCE_TESTS_TEST_SERVER_H

#include "../../network/server.h"
#include "../../engine/engine.h"
#include "utility.h"

class testServer : public fral::server {
public:
    testServer(fral::FRAL *ral, std::string& port, std::string& host, int entries) : server(ral, port, host), entries(entries) {}
    Status sync(ServerContext *context, ServerReader<Allocation> *stream, Empty *complete) {
        Allocation alloc;
        bool stamp = true;
        while (stream->Read(&alloc)) {
            auto blob = (char *) ral->allocate(alloc.allocation().size());
            memcpy(blob, alloc.allocation().c_str(), alloc.allocation().size());
            if(stamp){
                ((netMessage *) blob)->recT = high_resolution_clock::now();
                stamp = false;
            }
            ral->append(blob);
        }
        return Status::OK;
    }
    int entries;
};

#endif
