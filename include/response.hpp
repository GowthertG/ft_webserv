#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "header.hpp"
#include "configue.hpp"

class ServerConfig;
class Network;

class Response
{
    private:
        /* data */
    public:
        Response();
        ~Response();
        int post_err(Network *net);
        void send_err(int net_socket, int code);
        int loc_matched(std::string request, int index, Network *c);
        void send_res(Network *net, std::string key);
        std::string getContentType(std::string &filename);
        void handle_response(Network *c);
        void send_Get_response(std::string key, Network *c);
        bool is_valid_config(Network *c);
        void handle_delete(Network* c);
        int is_request_in_location(Network *c);
        int check_which_server(Network *c);
        int is_allowed_method(std::string method, std::vector<std::string> &methods);
        int Handle_cgi_response(Network *c, std::string url);
        int handle_url(Network *c, std::string url);
        void send_response_autoindex(int fd, std::string &url, std::string &root, Network *c);
        int is_dir(std::string location);
        std::string _sCodeStr(short _sCode);
};
int is_valid_file(Network *c, int i, int j);
int if_location_has_cgi(int i, int j);
int auto_index(Network *c, int index, int l);
void    is_config_empty(Network *c, int i, int j);
void handle_get(Network *c, char *args[3]);
void send_http_response(Network *c, const std::string& filepath);
#endif