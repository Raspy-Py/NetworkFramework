#include <exception>
#include "network/common.h"
#include "network/udp_client.h"
#include "network/sniffer.h"

#include <thread>
#include <chrono>
#include <atomic>
#include <sstream>

enum class GameEventType{
  Default = 0
};

std::atomic_bool finished = false;

class CustomClient : public UDPClient<GameEventType>{
 public:
  CustomClient() : UDPClient<GameEventType>() {}
  ~CustomClient() override{
    Disconnect();
  }

  void OnReceive(std::shared_ptr<Message<GameEventType>> msg) override{
    std::cout << "[CLIENT] Server responded." << std::endl;
    finished = true;
  }
};

int main(int argc, char** argv)
{
    CustomClient client;

    constexpr uint16_t kBroadcastPort = 8888;
    Sniffer sniffer(kBroadcastPort);
    sniffer.Start([&](std::string msg){
        if (auto pos = msg.find(':'); pos != std::string::npos) {
            std::string host = msg.substr(0, pos);
            std::string port = msg.substr(pos + 1);

            client.Connect(host, port);
            Message<GameEventType> msg{};
            msg.header.id = GameEventType::Default;
            msg << 42.0f;
            client.Send(msg);
        }else{
            std::cerr << "Sniffer sniffed some bullshit: " << msg << std::endl;
        }
    });

    while (!finished) {}

    return 0;
}