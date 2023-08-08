#include "client.h"

#include <grpcpp/grpcpp.h>

#include <iostream>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using medium::Allocation;
using medium::Empty;
using medium::Medium;
using medium::Start;

namespace fral {

client::client(
    fral::FRAL *ral,
    const std::string &port,
    const std::string &host)
    : ral(ral) {
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

  std::unique_ptr<grpc::ClientWriter<Allocation>> stream(
      stub->sync(&context, &complete));
  while (nextIdx < end) {
    auto blob = ral->load(nextIdx);
    if (blob) {
      alloc.set_allocation((char *)blob, fral::FRAL::getBlobSize(blob));
      if (!stream->Write(
              alloc,
              grpc::WriteOptions().set_no_compression().set_buffer_hint()))
      {
        std::cout << "Stream is closed" << std::endl;
        return -1;
      }
      nextIdx++;
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
}  // namespace fral
