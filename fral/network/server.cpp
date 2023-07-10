#include "server.h"

#include <grpcpp/server.h>
#include <grpcpp/server_context.h>

#include <chrono>
#include <fstream>
#include <utility>

namespace fral {

server::server(fral::FRAL *ral, std::string &port, std::string &host)
    : ral(ral), port(port), host(host) {}

Status server::connect(ServerContext *context, const Empty *hello,
                       Start *start) {
  auto startIdx = ral->size() - 1;
  start->set_idx(startIdx);
  return Status::OK;
}

Status server::sync(ServerContext *context, ServerReader<Allocation> *stream,
                    Empty *complete) {
  Allocation alloc;
  while (stream->Read(&alloc)) {
    auto blob = (char *)ral->allocate(alloc.allocation().size());
    memcpy(blob, alloc.allocation().c_str(), alloc.allocation().size());
    ral->append(blob);
  }
  return Status::OK;
}

void server::createServer() {
  ServerBuilder builder;
  std::string address = host + ":" + port;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(this);
  server_ = std::unique_ptr<Server>(builder.BuildAndStart());
}

Status server::shutdown(ServerContext *context, const Empty *finish,
                        Empty *ok) {
  keepAlive = false;
  cv.notify_one();
  return Status::OK;
}

void server::run() {
  createServer();
  std::cout << "FRAL2 server listening on " << host + ":" + port << std::endl;
  server_->Wait();
}

void server::listenForKill() {
  std::unique_lock<std::mutex> lock(mutex);
  while (keepAlive) {
    cv.wait(lock);
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  this->shutdown();
}

int server::shutdown() {
  if (server_) {
    server_->Shutdown();
    return 0;
  }
  return -1;
}

}  // namespace fral