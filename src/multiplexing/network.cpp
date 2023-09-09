#include "../../include/network.hpp"

Network::Network()
{
  addr_size = sizeof(address);
  rcved = 0;
  header = true;
  valid = false;
  is_done = false;
  is_readed = false;
  header_sent = false;
  file_size = 0;

}

Network::~Network()
{}

int Network::get_socket_fd()
{
  return socket_fd;
}

void Network::set_socket_fd(int fd)
{
  socket_fd = fd;
}

void Network::set_address(struct sockaddr_storage &address)
{
  address = address;
}
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    for (std::string::iterator it = result.begin(); it != result.end(); ++it) {
        *it = tolower(*it);
    }
    return result; 
}
void Network::handle_err() {
  std::map<std::string, std::string>::iterator it = request.headers.find("Content-Type");
  std::map<std::string, std::string>::iterator it1 = request.headers.find("Content-Length");
  std::map<std::string, std::string>::iterator it2 = request.headers.find("Transfer-Encoding");

  try {
    const char* maxBodySize_Str = cnf->serverConfigs[request.srv_index].maxBodySize.c_str();
    char* endPtr;
    request.max_body_size = std::strtol(maxBodySize_Str, &endPtr, 10);
    if (*endPtr != '\0') {
        throw std::invalid_argument("Error: body size");
    }
  } catch(const std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
}catch(...) {
    std::cerr << "Error: body size" << std::endl;
  }
  if (it == request.headers.end())
    request.is_err = 400;
  else if (it1 == request.headers.end() && it2 == request.headers.end())
    request.is_err = 400;
  else if (it1 != request.headers.end() && it2 != request.headers.end())
    request.is_err = 400;
  else if (it2 != request.headers.end() && toLowerCase(request.headers["Transfer-Encoding"]) != "chunked")
    request.is_err = 400;
  else if (it2 == request.headers.end() && it1 != request.headers.end()) {
    try {
        std::istringstream iss(request.headers["Content-Length"]);
        size_t size;
        if (!(iss >> size)) {
            throw std::invalid_argument("Error: Content-Length is not a valid integer");
        }

        request.req_size = size;
        if (size > request.max_body_size) {
            request.is_err = 413;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        request.is_err = 413;
    }
  }
}
int Response::loc_matched(std::string request, int index, Network *c)
{
    Response r;
    for (size_t i = 0; i < cnf->serverConfigs[index].locations.size(); i++)
    {
        int j = 0;
        if (request  == cnf->serverConfigs[index].locations[i].location_Config[j])
        {
            c->url = cnf->serverConfigs[index].locations[i].root + c->save_delete;
            return (i);
        }
        else
        {
            if (request[0] == '/' && request.find_last_of("/") != 0) 
            {
                c->save_delete = request.substr(request.find_last_of("/"), request.size());
                request.erase(request.find_last_of("/"), request.size());
            }
            if (request  == cnf->serverConfigs[index].locations[i].location_Config[j])
            {
                c->url = cnf->serverConfigs[index].locations[i].root + c->save_delete;
                return (i);
            }
        }
    }
    return (-1);
}
void Response::send_err(int c_socket, int code)
{
    std::map<int, std::string> response_map;
    // there are 399 status codes in the response
    // 100-199 are informational responses
    // 200-299 are success codes
    // 300-399 are redirection codes
    // 400-500 are client error codes
    // 501-599 are server error codes
    response_map[200] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\n200 OK";
    response_map[201] = "HTTP/1.1 201 Created\r\nContent-Type: text/html\r\nContent-Length: 7\r\n\r\nCreated";
    response_map[301] = "HTTP/1.1 301 Moved Permanently\r\nContent-Type: text/html\r\nContent-Length: 21\r\n\r\n301 Moved Permanently";
    response_map[400] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 15\r\n\r\n400 Bad Request";
    response_map[403] = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: 9\r\n\r\nForbidden";
    response_map[404] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\n404 Not Found";
    response_map[405] = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\nContent-Length: 22\r\n\r\n405 Method Not Allowed";
    response_map[409] = "HTTP/1.1 409 Conflict\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\n409 Conflict";
    response_map[500] = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 25\r\n\r\n500 Internal Server Error";
    response_map[501] = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length: 19\r\n\r\n501 Not Implemented";
    response_map[505] = "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Type: text/html\r\nContent-Length: 26\r\n\r\nHTTP Version Not Supported";
    response_map[413] = "HTTP/1.1 413 Pyload too large\r\nContent-Type: text/html\r\nContent-Length: 20\r\n\r\n413 Pyload too large";

    std::map<int, std::string>::iterator it = response_map.find(code);
    if (it == response_map.end()) // if the code is not in the map
    {
        std::cout << "Error code not valid" << code << std::endl;
        return;
    }

    std::string response = it->second; // get the response from the map using the code as key
    send(c_socket, response.c_str(), response.length(), 0);
}

int Response::is_allowed_method(std::string method, std::vector<std::string> &methods)
{
    if (std::find(methods.begin(), methods.end(), method) == methods.end())
        return (0);
    return (1);
}
void Response::send_Get_response(std::string key, Network *c)
{
    if (!c->header_sent)
    {
        if (key == "404:")
            c->file.open("root/error/404.html");
        else
            c->file.open(c->_file_name.c_str());
        if (!c->file.is_open() && !c->_file_name.empty())
        {
            std::cout << "Error opening file" << c->_file_name << std::endl;
            c->is_done = true;
            return;
        }

        // Get file size
        c->file.seekg(0, std::ios::end);
        c->file_size = c->file.tellg();
        c->file.seekg(0, std::ios::beg);

        // Create response header
        std::string response;
        if (key == "404:"){
            response = "HTTP/1.1 404 Not Found\r\n";
            response += "Content-Type: text/html\r\n";
            std::ostringstream oss;
            oss << "Content-Length: " << c->file_size << "\r\n\r\n";
            response += oss.str();
        }
        else{
            response = "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: " + getContentType(c->_file_name) + "\r\n";
            std::ostringstream _oss;
            _oss << "Content-Length: " << c->file_size << "\r\n\r\n";
            response += _oss.str();
        }

        if (send(c->get_socket_fd(), response.c_str(), response.size(), 0) < 1)
        {
            std::cout << "Error sending response header" << std::endl;
            c->is_done = true;
            c->file.close();
            return;
        }

        c->header_sent = true;
    }
    else
    {
        const int buff_size = 1024;
        char buffer[buff_size];

        c->file.read(buffer, buff_size);
        int bytes_read = c->file.gcount();

        if (bytes_read == -1)
        {
            std::cout << "Error reading file" << std::endl;
            c->is_done = true;
            c->file.close();
            return;
        }

        if (bytes_read < buff_size)
        {
            if (send(c->get_socket_fd(), buffer, bytes_read, 0) == -1)
                std::cout << "error sending response2" << std::endl;
            c->is_done = true;
            c->file.close();
            return;
        }

        if (send(c->get_socket_fd(), buffer, bytes_read, 0) < 1)
        {
            std::cout << "Error sending response" << std::endl;
            c->file.close();
            return;
        }

        c->file_size -= bytes_read;
    }
}

int Response::Handle_cgi_response(Network *c, std::string url)
{
    Response r;

    char *args[3];
    char *env[] = {NULL};
    
    if (r.handle_url(c, url) == 0)
        return 0;
    
    std::string script_name = url.substr(url.find_last_of('/') + 1);
    std::string ex = r.getContentType(url);
    std::string path = cnf->serverConfigs[c->request.srv_index].locations[c->request.location_index].root + "/" + script_name;
    if (ex == "python")
    {
        args[0] = strdup("/usr/bin/python");
        args[1] = strdup(path.c_str());
        args[2] = NULL;
    }
    else if (ex == "php")
    {
        args[0] = strdup("cgi-env/php-cgi");
        args[1] = strdup(path.c_str());
        args[2] = NULL;
    }
    else
    {
        if (ex != "python" && ex != "php")
        {
            for (int i = 0; i < 2; i++){
                free(args[i]);
                free(env[i]);
            }
            return 0;
        }
    }
    if (c->request.get_met() == "POST")
    {
        std::string str = cnf->serverConfigs[c->request.srv_index].locations[c->request.location_index].uploadPath + "/" + script_name;
        int fd2 = open(str.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

        if (fd2 == -1)
        {
            std::cout << "error in open" << std::endl;
            for (int i = 0; i < 2; i++){
                free(args[i]);
            }
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            // This is the child process
            dup2(fd2, 1);
            if (execve(args[0], args, env) == -1)
            {
                std::cout << "error in execve" << std::endl;
                for (int i = 0; i < 2; i++){
                    free(args[i]);
                }
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)
        {
            // This is the parent process
            sleep(5);
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);
            if (result == 0)
            {
                // Child process is still running
                kill(pid, SIGKILL);
            }
            close(fd2);
        }
        else
        {

            std::cout << "error in fork" << std::endl;
            for (size_t i = 0; i < 2; i++){
                free(args[i]);
            }
            exit(EXIT_FAILURE);
        }

        c->_file_name = str;
        r.send_err(c->get_socket_fd(), 201);
        c->is_done = true;
        for (size_t i = 0; i < 2; i++){
            free(args[i]);
        }
        return 1;
    }
    else
    {
        handle_get(c, args);
        return 1;
    }
}

int Response::is_dir(std::string location)
{
    // check if file open if yes is directory or not
    if (open(location.c_str(), O_DIRECTORY) != -1) // O_DIRECTORY is a flag for directories not being opened properly for reading and writing
        return (1);
    else
        return (0);
}


int Response::post_err(Network *c)
{
    Response r;
    std::string ex;

    int index = check_which_server(c);
    int l =  r.loc_matched(c->request.get_loc(), index, c);
    if (index == -1)
        index = 0;
    c->request.srv_index = index;
    c->request.location_index = l;
    ex = c->request.get_loc().substr(c->request.get_loc().find_last_of(".") + 1);
    if (cnf->serverConfigs[index].locations[l].uploadPath.empty())
    {
        r.send_Get_response("404:", c);
        c->is_done = true;
        return 0;
    }
    if (r.is_dir(cnf->serverConfigs[index].locations[l].uploadPath) == 0)
    {
        r.send_Get_response("404:", c);
        c->is_done = true;
        return 0;
    }
    if (r.is_allowed_method(c->request.get_met(), cnf->serverConfigs[index].locations[l].methods) == 0)
    {
        r.send_err(c->get_socket_fd(), 405); // not allowed
        c->is_done = true;
        return 0;
    }
    if (cnf->serverConfigs[index].locations[l].cgiPath[ex] != "")
    {
        c->_cgi_path = c->request.get_loc();
        r.Handle_cgi_response(c, c->_cgi_path);
        c->is_done = true;
        return (1);
    }
    c->valid = true;
    return 1;
}

bool Network::handle_post(Network *net)
{
  Response response;
  if (!response.post_err(net))
  {
    response.send_err(net->get_socket_fd(), 404);
    net->is_done = true;
    return 0;
  }
  handle_err();
  if (net->request.is_err)
  {
    response.send_err(socket_fd, net->request.is_err);
    net->is_done = true;
    return (0);
  }
  return(1);
}

void Network::handle_req(const char *req_body, size_t size)
{
  std::string ext;
  std::string str_header;
  std::string body;
  std::string url;
  std::string str(req_body, size);
  int pos;
  if (header)
  {
    pos = str.find(SEP);
    str_header = str.substr(0, pos);
    body = str.substr(pos + 4);
    request.handle_headers(str_header);
    url = request.get_loc();
    ext = url.substr(url.find_last_of(NO9TA) + WA7ED);
    if (request.get_met() == "POST" and handle_post(this) and cnf->serverConfigs[request.srv_index].locations[request.location_index].cgiPath[ext].empty())
    {
      if (!request.is_err)
        request.handle_body(body);
      header = false;
    }
  }
  else if (request.get_met() == "POST" and handle_post(this) and !request.is_err and cnf->serverConfigs[request.srv_index].locations[request.location_index].cgiPath[ext].empty())
    request.handle_body(str);
}