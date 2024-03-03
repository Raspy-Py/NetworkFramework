#ifndef WHATEVERGAME_SOURCE_NETWORK_SNIFFER_H_
#define WHATEVERGAME_SOURCE_NETWORK_SNIFFER_H_

#include <cstdint>
#include <functional>
#include <iostream>

#include <asio.hpp>

class Sniffer {
  using udp = asio::ip::udp;
 public:
  Sniffer(uint16_t broadcast_port)
      : io_context_(), socket_(io_context_, udp::endpoint(udp::v4(), broadcast_port)) {}

  ~Sniffer() {
    Stop();
  }

  void Start(std::function<void(const std::string&)>&& callback){
    callback_ = std::move(callback);
    StartReceive();
    context_thread_ = std::thread([&]{io_context_.run();});
  }

  void Stop(){
    io_context_.stop();

    if (context_thread_.joinable())
      context_thread_.join();
  }

 private:
  void StartReceive(){
    socket_.async_receive_from(
      asio::buffer(buffer_), sender_endpoint_,
      [&](const std::error_code& error, std::size_t bytes_recvd) {
        if (!error && bytes_recvd > 0) {
          std::string message(buffer_.data(), bytes_recvd);
          callback_(message);
        } else {
          StartReceive();
        }
      });
  }
 private:
  // Context should always go first
  asio::io_context io_context_;
  std::thread context_thread_;

  udp::socket socket_;
  udp::endpoint sender_endpoint_;
  std::function<void(const std::string&)> callback_;

  static constexpr size_t kBufferSize = 128;
  std::array<char, kBufferSize> buffer_;
};

#endif //WHATEVERGAME_SOURCE_NETWORK_SNIFFER_H_