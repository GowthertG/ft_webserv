#include "../../include/webserv.hpp"

Webserv::Webserv(){}

Webserv::Webserv(std::string &port, std::string &host)
{
	std::cout << "Server was Created" << std::endl;

  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  this->port = port;
  this->host = host;
  FD_ZERO(&net_fd);
}

Network * Webserv::get_network(int f)
{
	std::vector<Network *>::iterator s = nets.begin();
	while (s < nets.end())
	{
		if ((*s)->get_socket_fd() == f)
		{
			return (*s);
		}
		s++;
	}
	return NULL;
}

void Webserv::add_network()
{
  Network *net = new Network();
  struct sockaddr_storage add;
  socklen_t addr_len = sizeof(add);
  int net_sock = accept(sock_fd, (struct sockaddr *)&add, &addr_len);
  fcntl(net_sock, F_SETFL, O_NONBLOCK);
  if (net_sock < 0)
  {
    std::cerr << "accept: " << strerror(errno) << std::endl;
    exit(1);
  }
  FD_SET(net_sock, &net_fd);
  if (net_sock > maxfd_sock)
    maxfd_sock = net_sock;
  net->set_socket_fd(net_sock);
  net->set_address(add);
  nets.push_back(net);
}

void Webserv::delete_network(Network *net)
{
  if ((net && net->is_done) == true)
  {
    std::cout << "Client disconnected." << std::endl;
    for (size_t i = 0; i < nets.size(); i++)
    {
      if (net->get_socket_fd() == nets[i]->get_socket_fd())
      {
        FD_CLR(net->get_socket_fd(), &net_fd);
        close(net->get_socket_fd());
        nets.erase(nets.begin() + i);
        delete net;
        break;
      }
    }
  }
}
