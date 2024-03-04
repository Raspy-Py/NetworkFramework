#ifndef WHATEVERGAME_SOURCE_NETWORK_UDP_CLIENT_H_
#define WHATEVERGAME_SOURCE_NETWORK_UDP_CLIENT_H_

#include <string>
#include <asio.hpp>

#include "common.h"
#include "thread_safe_queue.h"

template <typename T>
class UDPClient {
  using udp = asio::ip::udp;

 public:
  UDPClient()
      : io_context_(), socket_(io_context_, udp::endpoint(udp::v4(), 8081)){}

  virtual ~UDPClient(){
    Disconnect();
  }

  virtual void OnReceive(std::shared_ptr<Message<T>> message) = 0;

  void Connect(const std::string& server, const std::string& port){
    recv_buffer_.resize(kMaxMsgSize);
    send_buffer_.resize(kMaxMsgSize);

    udp::resolver resolver(io_context_);
    udp::resolver::query query(udp::v4(), server, port);
    server_endpoint_ = *resolver.resolve(query);

    StartReceive();
    context_thread_ = std::thread([this]{ io_context_.run(); });
  }

  void Disconnect(){
    io_context_.stop();

    if (context_thread_.joinable())
      context_thread_.join();
  }

  void Send(const Message<T>& msg){
    out_queue_.Push(std::make_shared<Message<T>>(msg));
    HandleSend();
  }

 private:
  void HandleSend(){
    std::shared_ptr<Message<T>> msg_ptr;
    out_queue_.TryPop(msg_ptr);
    msg_ptr->Serialize(send_buffer_);
    socket_.async_send_to(
      asio::buffer(send_buffer_), server_endpoint_,
      [this](const asio::error_code& ec, std::size_t bytes_sent){
        if (ec){
          std::cerr << "[CLIENT] Error: " << ec.message() << std::endl;
          return;
        }
        if (!out_queue_.Empty())
          HandleSend();
      });
  }

  void StartReceive(){
    socket_.async_receive_from(
      asio::buffer(recv_buffer_), sender_endpoint_,
      [this](const asio::error_code& ec, std::size_t bytes_recvd){
        if (ec){
          std::cerr << "[CLIENT] Error: " << ec.message() << std::endl;
          return;
        }
        auto msg_ptr = std::make_shared<Message<T>>();
        msg_ptr->Deserialize(recv_buffer_, bytes_recvd);
        in_queue_.Push(msg_ptr);
        HandleReceive();
      });
  }

  void HandleReceive(){
    if (io_context_.stopped())
      return;

    std::shared_ptr<Message<T>> msg_ptr;
    in_queue_.WaitAndPop(msg_ptr);
    StartReceive();
    OnReceive(msg_ptr);
  }

 private:
  asio::io_context io_context_;
  asio::ip::udp::socket socket_;
  std::thread context_thread_;

  udp::endpoint server_endpoint_;
  udp::endpoint sender_endpoint_;

  ThreadSafeQueue<std::shared_ptr<Message<T>>> in_queue_;
  ThreadSafeQueue<std::shared_ptr<Message<T>>> out_queue_;

  static constexpr size_t kMaxMsgSize = 1024;
  std::vector<char> recv_buffer_;
  std::vector<char> send_buffer_;
};

#endif //WHATEVERGAME_SOURCE_NETWORK_UDP_CLIENT_H_