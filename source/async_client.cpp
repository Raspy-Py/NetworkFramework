#include "async_client.h"

AsyncClient::AsyncClient(asio::io_context& io_context, std::string_view server, std::string_view port)
    : io_context_(io_context), socket_(io_context, udp::endpoint(udp::v4(), 0)) {
  udp::resolver resolver(io_context_);
  udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), server.data(), port.data());
  server_endpoint_ = *endpoints.begin();
}

void AsyncClient::Send(std::string_view message)
{
  socket_.async_send_to(
      asio::buffer(message), server_endpoint_,
      [this](std::error_code ec, std::size_t bytes_sent){
        if (ec){
          std::cerr << "Failed to send message: " << ec.message() << std::endl;
        }
      });
}

AsyncClient::~AsyncClient()
{
  socket_.close();
}