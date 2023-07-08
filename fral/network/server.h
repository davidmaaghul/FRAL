#ifndef FRAL_SERVER_H
#define FRAL_SERVER_H

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <condition_variable>
#include <string>
#include <thread>

#include "../engine/engine2.h"
#include "medium.grpc.pb.h"

using medium::Allocation;
using medium::Empty;
using medium::Medium;
using medium::Start;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;

namespace fral {

class server : public Medium::Service {
 public:
  server(fral::FRAL2 *log, std::string &port, std::string &host);

  Status connect(ServerContext *context, const Empty *hello,
                 Start *start) override;

  Status sync(ServerContext *context, ServerReader<Allocation> *stream,
              Empty *complete) override;

  Status shutdown(ServerContext *context, const Empty *finish,
                  Empty *ok) override;

  void run();

  void listenForKill();

  int shutdown();

 protected:
  void createServer();

  fral::FRAL2 *ral;

  bool keepAlive = true;

  std::mutex mutex;

  std::condition_variable cv;

  std::string host;

  std::string port;

  std::unique_ptr<Server> server_;
};

}  // namespace fral

#endif  // FRAL_SERVER_H
