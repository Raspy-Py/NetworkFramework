#include <iostream>
#include <sstream>
#include <string>
#include <asio.hpp>

#include <chrono>
#include <thread>

using asio::ip::udp;

void broadcast_presence(asio::io_context& io_context, unsigned short port, const std::string& message) 
{
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));
    socket.set_option(asio::socket_base::broadcast(true));
    udp::endpoint broadcast_endpoint(asio::ip::address_v4::broadcast(), port);
    socket.async_send_to(asio::buffer(message), broadcast_endpoint, [](std::error_code /*ec*/, std::size_t /*bytes_sent*/){});
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: asio_server <port>\n";
    return -1;
  }

  std::stringstream port_stream;
  port_stream << argv[1];
  unsigned short port;
  port_stream >> port;

  asio::io_context io_context;
  std::string message = "GameRoomAvailable:" + std::to_string(port);
  for (int i = 0; i < 10; i++)
     broadcast_presence(io_context, port, message);
  std::this_thread::sleep_for(std::chrono::seconds(5));

  io_context.run();

  return 0;
}
