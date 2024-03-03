#include <asio.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include "network/sniffer.h"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
	  std::cerr << "Usage: asio_client <port>\n";
	  return -1;
  }

  std::stringstream port_stream;
  port_stream << argv[1];
  unsigned short port;
  port_stream >> port;
  try{
    std::atomic_bool finished = false;
    Sniffer sniffer(port);
    sniffer.Start([&](const std::string& message){
        std::cout << "Received: " << message << std::endl;
        finished = true;
    });
    
    while (!finished)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  } catch (const std::exception& e){
  	std::cerr << e.what() << std::endl;
  }

  return 0;
}