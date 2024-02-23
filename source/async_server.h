#ifndef WHATEVERGAME_SOURCE_NETWORK_SERVER_H_
#define WHATEVERGAME_SOURCE_NETWORK_SERVER_H_

#include <asio.hpp>
#include <array>
#include <iostream>

using asio::ip::udp;

class AsyncServer {
 public:
  AsyncServer(asio::io_context& io_context, short port);
  ~AsyncServer();

 private:
  void start_receive();

 private:
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  std::array<char, 1024> recv_buffer_;
};

#endif //WHATEVERGAME_SOURCE_NETWORK_SERVER_H_