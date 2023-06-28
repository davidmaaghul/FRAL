#ifndef FRAL_CLIENT_H
#define FRAL_CLIENT_H

#include <grpcpp/grpcpp.h>
#include "medium.grpc.pb.h"
#include "../engine/engine.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using medium::Medium;
using medium::Allocation;

namespace fral {

    class client {
    public:
        client(fral::FRAL *log, const std::string &port, const std::string &host);

        int connect();

        virtual int sync(const int &end);

        int shutdown();

    protected:

        std::unique_ptr <Medium::Stub> stub;
        std::shared_ptr <Channel> channel;
        std::string name;
        std::string host;
        std::string port;
        fral::FRAL *ral;
    };

}


#endif //FRAL_CLIENT_H
