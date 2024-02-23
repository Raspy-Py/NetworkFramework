#ifndef WHATEVERGAME_SOURCE_NETWORK_CLIENT_H_
#define WHATEVERGAME_SOURCE_NETWORK_CLIENT_H_

#include <asio.hpp>
#include <iostream>
#include <string>

using asio::ip::udp;

class AsyncClient {
 public:
  AsyncClient(asio::io_context& io_context, std::string_view server, std::string_view port);
  ~AsyncClient();

  void Send(std::string_view message);

 private:
  asio::io_context& io_context_;
  udp::socket socket_;
  udp::endpoint server_endpoint_;
};

#endif //WHATEVERGAME_SOURCE_NETWORK_CLIENT_H_