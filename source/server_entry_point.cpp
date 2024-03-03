#include <iostream>
#include <sstream>
#include <string>
#include <asio.hpp>

#include "network/broadcaster.h"

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

  try {
	Broadcaster broadcaster(port);
	broadcaster.Start("Yo sniffer, what's up dude?");
	std::this_thread::sleep_for(std::chrono::seconds(5));
  } catch (const std::exception& e){
	std::cerr << e.what() << std::endl;
  }


  return 0;
}