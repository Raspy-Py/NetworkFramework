#include "async_server.h"

#include <asio.hpp>

using asio::ip::udp;

AsyncServer::AsyncServer(asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)) {
  start_receive();
}

AsyncServer::~AsyncServer()
{

}

void AsyncServer::start_receive() {
  socket_.async_receive_from(
      asio::buffer(recv_buffer_), remote_endpoint_,
      [this](std::error_code ec, std::size_t bytes_recvd){
        if (!ec && bytes_recvd > 0)
        {
          std::string data(recv_buffer_.data(), bytes_recvd);
          if (data == "kill")
          {
            std::cout << "Shutting down" << std::endl;
            return;
          }
          std::cout << "Received some data: " << std::endl;
          std::cout << data << std::endl;
          start_receive();
        }else{
          std::cout << "Receive failed." << std::endl;
          start_receive();
        }
      });
}