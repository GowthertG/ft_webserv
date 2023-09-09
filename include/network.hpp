#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "header.hpp"
#include "request.hpp"

class Network
{
private:
  struct sockaddr_storage address;
  socklen_t addr_size;
  int socket_fd;
  bool isListen;
  int rcved;
  bool header;

public:
  Network();
  ~Network();
  void set_socket_fd(int fd);
  void set_address(struct sockaddr_storage &address);
  void handle_req(const char *req_body, size_t length);
  int get_socket_fd();
  void handle_err();
  int file_size;
  bool handle_post(Network *c);
  bool is_readed;
  bool  is_done;
  bool valid;
  bool header_sent;
  Request request;
  std::string url;
  std::string save_delete;
  std::string _cgi_path;
  std::string _file_name;
  std::ifstream file;
};

#endif
