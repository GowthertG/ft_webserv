#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "header.hpp"

class Network;
class Request;

class Webserv
{
  private:
    int sock_fd;
  	struct addrinfo hints;
	  struct addrinfo *records;
    std::vector<Network *> nets;
    int maxfd_sock;
    fd_set net_fd;
    fd_set fdread;
    fd_set fdwrite;
    fd_set fderror;
    std::string port;
    std::string host;

  public:

    Webserv();
    Webserv(std::string &port, std::string &host);
    void  setuping();
    Network *get_network(int s);
    void  add_network();
    void  delete_network(Network *net);
    void  multiplexing(Network *net, struct timeval &t);
    void  init_fdbit();
};

#endif