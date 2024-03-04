#include <exception>
#include "network/common.h"
#include "network/udp_client.h"
#include "network/udp_server.h"

#include <thread>
#include <chrono>
#include <atomic>


std::atomic_bool g_finished = false;

enum class GameEventType{
  Default = 0
};

class CustomClient : public UDPClient<GameEventType>{
 public:
  CustomClient() : UDPClient<GameEventType>() {}
  ~CustomClient() override{
    std::cout << "[CLIENT] Shutting down..." << std::endl;
    Disconnect();
  }

  void OnReceive(std::shared_ptr<Message<GameEventType>> msg) override{
    float a{};
    *msg >> a;
    std::cout << "[CLIENT] Received msg: " << a << std::endl;
    g_finished = true;
  }
};

class CustomServer : public UDPServer<GameEventType>{
 public:
  explicit CustomServer(uint16_t port) : UDPServer<GameEventType>(port){}
  ~CustomServer() override{
    std::cout << "[SERVER] Shutting down..." << std::endl;
    Stop();
  }

  void OnReceive(Packet<GameEventType> packet) override{
    auto msg = *(packet.message);
    float a{};
    *(packet.message) >> a;
    std::cout << "[SERVER] Received msg: " << a << std::endl;
    SendToAllExcept(msg);
  }
};


int main(int argc, char** argv)
{
  Message<GameEventType> msg;
  msg.header.id = GameEventType::Default;
  float a = 42.0f;
  msg << a;

  CustomServer server(8080);
  server.Start();
  std::this_thread::sleep_for(std::chrono::seconds(3));

  CustomClient client;
  client.Connect("localhost", "8080");

  for (int i = 0; i < 5; ++i) {
    client.Send(msg);
  }
  std::this_thread::sleep_for(std::chrono::seconds(3));

  while (!g_finished){}

  return 0;
}