#ifndef WHATEVERGAME_SOURCE_NETWORK_COMMON_H_
#define WHATEVERGAME_SOURCE_NETWORK_COMMON_H_

/*
 * Heavily inspired by javidx9's "Networking in C++" tutorial series.
 * https://github.com/OneLoneCoder/Javidx9/tree/master/PixelGameEngine/BiggerProjects/Networking
 */

#include <vector>
#include <iostream>
#include <asio.hpp>

template<typename T>
struct MessageHeader{
  T id;
  size_t size;
};

template<typename T>
struct Message{
  MessageHeader<T> header;
  std::vector<char> body;

  [[nodiscard]] size_t Size() const{
    return body.size();
  }

  template<typename DataType>
  friend Message<T>& operator<<(Message<T>& msg, const DataType& data){
    static_assert(std::is_standard_layout<DataType>::value, "Data is to complex for serialization");
    size_t old_size = msg.Size();
    msg.body.resize(old_size + sizeof(data));
    memcpy(msg.body.data() + old_size, &data, sizeof(data));
    msg.header.size = msg.Size();
    return msg;
  }

  template<typename DataType>
  friend Message<T>& operator>>(Message<T>& msg, DataType& data){
    static_assert(std::is_standard_layout<DataType>::value, "Data is to complex for deserialization");
    if (sizeof(data) > msg.Size()) return msg;
    size_t new_size = msg.body.size() - sizeof(data);
    memcpy(&data, msg.body.data() + new_size, sizeof(data));
    msg.body.resize(new_size);
    msg.header.size = msg.Size();
    return msg;
  }

  void Serialize(std::vector<char>& buffer) const {
    size_t header_size = sizeof(header);
    buffer.resize(sizeof(header) + Size());
    memcpy(buffer.data(), buffer.data(), header_size);
    memcpy(buffer.data() + header_size, body.data(), Size());
  }

  void Deserialize(std::vector<char>& buffer, size_t data_size){
    size_t header_size = sizeof(header);
    memcpy(&header, buffer.data(), header_size);
    size_t body_size = data_size - header_size;
    body.resize(body_size);
    memcpy(body.data(), buffer.data() + header_size, body_size);
    header.size = Size();
  }
};

template <typename T>
struct Packet{
  asio::ip::udp::endpoint endpoint;
  std::shared_ptr<Message<T>> message;
};

#endif //WHATEVERGAME_SOURCE_NETWORK_COMMON_H_