#include <exception>
#include "network/common.h"
#include "network/udp_server.h"
#include "network/broadcaster.h"

#include <thread>
#include <chrono>
#include <atomic>
#include <sstream>

std::string find_local_ip() {
    asio::io_context io_context;
    std::string local_ip;
    try {
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::resolver::query query("8.8.8.8", "53");
        asio::ip::tcp::resolver::iterator endpoints = resolver.resolve(query);
        asio::ip::tcp::endpoint ep = *endpoints;
        asio::ip::tcp::socket socket(io_context);
        socket.connect(ep);
        local_ip = socket.local_endpoint().address().to_string();
        socket.close();
    } catch (std::exception& e) {
        std::cerr << "Could not find local IP: " << e.what() << '\n';
    }
    return local_ip;
}


enum class GameEventType{
  Default = 0
};

std::atomic_bool finished = false;

class CustomServer : public UDPServer<GameEventType>{
 public:
  explicit CustomServer(uint16_t port) : UDPServer<GameEventType>(port){}
  ~CustomServer() override{
    std::cout << "[SERVER] Shutting down..." << std::endl;
    Stop();
  }

  void OnReceive(Packet<GameEventType> packet) override{
    auto endp = packet.endpoint;
    std::cout << "[SERVER] was detected by: " << endp.address().to_string()
              << ":" << endp.port() << std::endl;
    
  SendToAllExcept({});
  finished = true;
  }
};

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    std::stringstream ss;
    ss << argv[1];
    uint16_t port;
    ss >> port;

    CustomServer server(port);
    server.Start();

    constexpr uint16_t kBroadcastPort = 8888;
    Broadcaster broadcaster(kBroadcastPort);
    broadcaster.Start(find_local_ip() + ":" + std::to_string(port));
    
    while (!finished) {}

    broadcaster.Stop();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    server.Stop();

    return 0;
}