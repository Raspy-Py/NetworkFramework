#include <asio.hpp>
#include <iostream>

using asio::ip::udp;

void start_receive(udp::socket& socket, std::array<char, 1024>& recv_buffer, udp::endpoint& sender_endpoint) {
    socket.async_receive_from(
        asio::buffer(recv_buffer), sender_endpoint,
        [&](const std::error_code& error, std::size_t bytes_recvd) {
            if (!error && bytes_recvd > 0) {
                std::cout << "Received message: " << std::string(recv_buffer.data(), bytes_recvd)
                          << " from " << sender_endpoint.address().to_string()
                          << ":" << sender_endpoint.port() << std::endl;
                
                // You can now use sender_endpoint to reply or to store the sender's address and port

                // Optionally, continue to listen for more messages
                start_receive(socket, recv_buffer, sender_endpoint);
            } else {
                // Handle error or no bytes received
                std::cerr << "Receive failed: " << error.message() << std::endl;
                // Optionally, continue to listen for more messages even in case of error
                start_receive(socket, recv_buffer, sender_endpoint);
            }
        });
}

int main() {
    try {
        asio::io_context io_context;
        
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 8080)); // Listening on a random available port
        std::array<char, 1024> recv_buffer;
        udp::endpoint sender_endpoint;
        
        start_receive(socket, recv_buffer, sender_endpoint);
        
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
