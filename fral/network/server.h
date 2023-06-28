#ifndef FRAL_SERVER_H
#define FRAL_SERVER_H

#include "medium.grpc.pb.h"
#include "../engine/engine.h"
#include <string>
#include <thread>
#include <condition_variable>

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

using medium::Medium;
using medium::Allocation;
using medium::Empty;
using medium::Start;

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerBuilder;
using grpc::ServerReader;
using grpc::Server;

namespace fral {

    class server : public Medium::Service {

    public:
        server(fral::FRAL *log, std::string &port, std::string &host);

        Status connect(ServerContext *context, const Empty *hello, Start *start) override;

        Status sync(ServerContext *context, ServerReader <Allocation> *stream, Empty *complete) override;

        Status shutdown(ServerContext *context, const Empty *finish, Empty *ok) override;

        void run();

        void listenForKill();

        int shutdown();

    protected:
        void createServer();

        fral::FRAL *ral;

        bool keepAlive = true;

        std::mutex mutex;

        std::condition_variable cv;

        std::string host;

        std::string port;

        std::unique_ptr<Server> server_;
    };

}

#endif //FRAL_SERVER_H
