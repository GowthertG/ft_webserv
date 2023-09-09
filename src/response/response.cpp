#include "../../include/response.hpp"

Response::Response()
{
}
Response::~Response()
{
}
std::string Response::getContentType(std::string &filename)
{
    if (filename.empty())
        return WALO;
    std::map<std::string, std::string> contentTypeMap;
    contentTypeMap[".html"] = "text/html";
    contentTypeMap[".png"] = "image/png";
    contentTypeMap[".mp4"] = "video/mp4";
    contentTypeMap[".mp3"] = "audio/mpeg";
    contentTypeMap[".css"] = "text/css";
    contentTypeMap[".js"] = "text/javascript";
    contentTypeMap[".json"] = "application/json";
    contentTypeMap[".xml"] = "application/xml";
    contentTypeMap[".pdf"] = "application/pdf";
    contentTypeMap[".zip"] = "application/zip";
    contentTypeMap[".txt"] = "text/plain";
    contentTypeMap[".gif"] = "image/gif";
    contentTypeMap[".jpg"] = "image/jpeg";
    contentTypeMap[".svg"] = "image/svg+xml";
    contentTypeMap[".wav"] = "audio/wav";
    contentTypeMap[".mpg"] = "video/mpeg";
    contentTypeMap[".mov"] = "video/quicktime";
    contentTypeMap[".avi"] = "video/x-msvideo";
    contentTypeMap[".php"] = "php";
    contentTypeMap[".py"] = "python";

    std::string contentType = "application/octet-stream";                // default content type is binary
    std::string extension = filename.substr(filename.find_last_of(NO9TA));
    if (contentTypeMap.count(extension) > 0)
    {
        contentType = contentTypeMap[extension];
        return contentType;
    }
    return contentType;
}

void Response::send_res(Network *net, std::string key)
{
    std::ostringstream contentLengthHeader;
    std::string response;

    if (!net->header_sent)
    {
        if (key == "404:")
            net->file.open("root/error/404.html");
        else
            net->file.open(net->_file_name.c_str());
        if (!net->file.is_open() && !net->_file_name.empty())
        {
            std::cout << "Error opening file" << net->_file_name << std::endl;
            net->is_done = true;
            return;
        }

        // Get file size
        net->file.seekg(0, std::ios::end);
        net->file_size = net->file.tellg();
        net->file.seekg(0, std::ios::beg);

        // Create response header
        if (key == "404:"){
            response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";
            contentLengthHeader << "Content-Length: " << net->file_size << "\r\n\r\n";
            response += contentLengthHeader.str();
        }
        else{
            response = "HTTP/1.1 200 OK\r\nContent-Type: " + getContentType(net->_file_name) + "\r\n";
            contentLengthHeader << "Content-Length: " << net->file_size << "\r\n\r\n";
            response += contentLengthHeader.str();
        }

        if (send(net->get_socket_fd(), response.c_str(), response.size(), 0) < 1)
        {
            std::cout << "Error sending response header" << std::endl;
            net->is_done = true;
            net->file.close();
            return;
        }

        net->header_sent = true;
    }
    else
    {
        const int buff_size = 1024;
        char buffer[buff_size];

        net->file.read(buffer, buff_size);
        int bytes_read = net->file.gcount();

        if (bytes_read == -1)
        {
            std::cout << "Error reading file" << std::endl;
            net->is_done = true;
            net->file.close();
            return;
        }

        if (bytes_read < buff_size)
        {
            if (send(net->get_socket_fd(), buffer, bytes_read, 0) == -1)
                std::cout << "error sending response2" << std::endl;
            net->is_done = true;
            net->file.close();
            return;
        }

        if (send(net->get_socket_fd(), buffer, bytes_read, 0) < 1)
        {
            std::cout << "Error sending response" << std::endl;
            net->file.close();
            return;
        }

        net->file_size -= bytes_read;
    }
}

void Response::handle_response(Network *c)
{
    std::string method = c->request.get_met();


    Response res;
    if (method.empty())
        return;
    if (method == "GET")
    {
        if (c->valid == true)
        {
            res.send_Get_response(WALO, c);
            return;
        }
        else
        {
            res.is_valid_config(c);
            return;
        }
    }
    else if (method == "POST")
    {
        if (c->valid == true && c->request.finished == true)
        {
            std::string Response = "HTTP/1.0 200 OK\r\n\r\n";
            send(c->get_socket_fd(), Response.c_str(), Response.length(), 0);
            c->is_done = true;
            return;
        }
    }
    else if (method == "DELETE")
    {
        if (c->valid == true)
        {
            res.handle_delete(c);
            return;
        } else {
            res.is_valid_config(c);
            return;
        }
    }
    else {
        send_err(c->get_socket_fd(), 501);
        c->is_done = true;
    }
}

bool Response::is_valid_config(Network *c)
{
    Response res;
    if (res.is_request_in_location(c) == 1)
    {
        c->valid = true;
        return 1;
    }
    else
    {
        send_Get_response("404:", c);
        c->is_done = true;
        return 1;
    }
    return 0;
}

void Response::handle_delete(Network* c)
{
    std::string location = c->request.get_loc();
    std::string response;
    // int status_code = 0;


    // Remove leading slash from the location if present
    if (location[0] == '/' && location.length() > 1)
        location.erase(0, 1);

    if (location.length() == 1 && location[0] == '/')
    {
        response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
        // status_code = 403; // Forbidden
        send(c->get_socket_fd(), response.c_str(), response.length(), 0);
        c->is_done = true;
        return;
    }

    if (access(c->_file_name.c_str(), F_OK) == -1) // File not found
    {
        // The file does not exist
        std::cout << "File does not exist" << std::endl;
        // status_code = 404; // Not Found
        response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    }
    else
    {
        // Check write permission on the file
        if (access(c->_file_name.c_str(), W_OK) == -1)
        {
            // status_code = 403; // Forbidden
            response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
            send(c->get_socket_fd(), response.c_str(), response.length(), 0);
            c->is_done = true;
            return;
        }

        if (remove(c->_file_name.c_str()) != 0)
        {
            std::cout << "Error occurred while deleting the file" << std::endl;
            // status_code = 500; // Internal Server Error
            response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
        }
        else
        {
            // status_code = 204; // No Content
            response = "HTTP/1.1 204 No Content\r\nContent-Length: 0\r\n\r\n";
        }
    }

    // Send the response
    send(c->get_socket_fd(), response.c_str(), response.length(), 0);
    c->is_done = true;
    return;
}

int Response::is_request_in_location(Network *c)
{
    Response res;
    int l = -1;
    for (size_t i = 0; i < cnf->serverConfigs.size(); i++)
    {
        int index = check_which_server(c);
        if (index == -1)
            index = 0; // default server
        for (size_t j = 0; j < cnf->serverConfigs[index].locations.size(); j++)
        {
            // match location
            l =  res.loc_matched(c->request.get_loc(), index, c);
            if (l == -1)
            {
                send_Get_response("404:", c);
                c->is_done = true;
                return 0;
            }
            if (l != -1){
                c->request.srv_index = index;
                c->request.location_index = l;
                if (cnf->serverConfigs[index].locations[l].return_ != "")
                {
                    // send_response_return(c->get_socket_fd(), config->servers[index].locations[l].return_);
                    c->is_done = true;
                    return 1;
                }
                if (!is_allowed_method(c->request.get_met(), cnf->serverConfigs[index].locations[l].methods))
                {
                    res.send_err(c->get_socket_fd(), 405);
                    c->is_done = true;
                    return 0;
                }
                if (is_valid_file(c, index, l) == 1) // if file is valid
                {
                    if (if_location_has_cgi(index, l) == 1 && access(c->_file_name.c_str(), F_OK) != -1)
                    {
                        c->_cgi_path = c->request.get_loc();
                        res.Handle_cgi_response(c, c->_cgi_path);
    
                        return (1);
                    }
                    if (c->request.get_met() == "POST" && if_location_has_cgi(index, l) == 0)
                    {
                        send_err(c->get_socket_fd(), 403);
                        c->is_done = true;
                        return 0;
                    }
                }
                else{
                    if (auto_index(c, index, l) == 1)
                        return (1);
                    else{
                        send_Get_response("404:", c);
                        c->is_done = true;
                        return 0;
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

int is_valid_file(Network *c, int i, int j)
{
    std::string path;
    Request req;
    Response r;

   for(size_t k = 0; k < cnf->serverConfigs[i].locations[j].index.size(); k++)
   {
        is_config_empty(c, i, j);
        if (c->request.get_loc() == "/")
        {
            path = cnf->serverConfigs[i].locations[j].root + cnf->serverConfigs[i].locations[j].index[k];
            c->_file_name = path;
            if (auto_index(c, i, j) == 1)
                return (1);
            return (1);
        }
        if (open(c->url.c_str(), O_DIRECTORY) != -1)
        {
            if (auto_index(c, i, j) == 1)
                return (1);
            for (size_t l = 0; l < cnf->serverConfigs[i].locations[j].index.size(); l++)
            {
                path = cnf->serverConfigs[i].locations[j].root + c->save_delete + "/" + cnf->serverConfigs[i].locations[j].index[l];
                if (access((path).c_str(), F_OK) != -1)
                {
                    c->_file_name = path;
                    return (1);
                }
            }
        }
        else{
            if (access((c->url).c_str(), F_OK) != -1) // is file
            {
                c->_file_name = c->url;
                return (1);
            }
            else
            {
                r.send_err(c->get_socket_fd(), 404);
                c->is_done = true;
                return (0);
            }
        }
   }
    return (0);
}

int if_location_has_cgi(int i, int j){
    
    if (!cnf->serverConfigs[i].locations[j].cgiPath["py"].empty() ||
        !cnf->serverConfigs[i].locations[j].cgiPath["php"].empty())
        return (1);
    return (0);
}

int Response::handle_url(Network *c, std::string url)
{
    Response r;
    int j = 0;

    std::string extension = url.substr(url.find_last_of('.') + 1);
    std::string rootPath = cnf->serverConfigs[c->request.srv_index].locations[c->request.location_index].root;
    std::string cgiPath = cnf->serverConfigs[c->request.srv_index].locations[c->request.location_index].cgiPath[extension];

    int locationMatchIndex = r.loc_matched(url, c->request.srv_index, c);

    if (locationMatchIndex != -1)
    {
        std::string pathLocation = rootPath + cgiPath;

        if (open(pathLocation.c_str(), O_DIRECTORY) != -1)
        {
            if (cnf->serverConfigs[c->request.srv_index].locations[c->request.location_index].autoindex == "on")
            {
                std::string directory = rootPath;
                std::string indexPath = rootPath + c->save_delete;
                indexPath = cnf->serverConfigs[c->request.srv_index].locations[c->request.location_index].location_Config[j];

                if (indexPath.empty())
                {
                    r.send_Get_response("404:", c);
                    c->is_done = true;
                    return 0;
                }

                r.send_response_autoindex(c->get_socket_fd(), indexPath, directory, c);
                send(c->get_socket_fd(), indexPath.c_str(), indexPath.length(), 0);
                c->is_done = true;
            }
            else
            {
                r.send_Get_response("404:", c);
                c->is_done = true;
                return 0;
            }
        }
        else if (access(pathLocation.c_str(), F_OK) != -1)
        {
            c->_file_name = pathLocation;
            return 1;
        }
    }
    else
    {
        send_Get_response("404:", c);
        c->is_done = true;
        return 0;
    }

    return 0;
}

int auto_index(Network *c, int index, int l)
{
    Response r;
    if (cnf->serverConfigs[index].locations[l].autoindex == "on")
    {
        // list all of content of directory and send it to client
        int j = 0;
        std::string dir = cnf->serverConfigs[index].locations[l].root; // root
        std::string url = cnf->serverConfigs[index].locations[l].root + c->save_delete;
        url = cnf->serverConfigs[index].locations[l].location_Config[j];;
        if (url == WALO)
        {
            r.send_Get_response("404:", c);
            c->is_done = true;
            return 0;
        }
        r.send_response_autoindex(c->get_socket_fd(), url, dir, c);
        c->is_done = true;
        return 1;
    }
    return 0;
}

void    is_config_empty(Network *c, int i, int j)
{
    Response r;
    if (cnf->serverConfigs[i].locations[j].root.empty())
    {
        r.send_err(c->get_socket_fd(), 404);
        c->is_done = true;
        return;
    }
    else if (cnf->serverConfigs[i].locations[j].methods.empty())
    {
        r.send_err(c->get_socket_fd(), 404);
        c->is_done = true;
        return;
    }
}

void Response::send_response_autoindex(int fd, std::string &url, std::string &root, Network *c)
{
    // Send the autoindex page with the clickable content of the root
    Response r;
    std::string response;
    std::string content;

    content = "<html>\n"
              "<head><title>Index of " +
              root + "</title></head>\n"
                     "<body bgcolor=\"white\">\n"
                     "<h1>Index of " +
              root + "</h1><hr><pre>\n";

    std::string dir = root;
    DIR *directory;
    struct dirent *directoryEntry;
    if ((directory = opendir(dir.c_str())) == NULL)
    {
        std::cout << "Error opening " << dir << std::endl;
        return;
    }

    while ((directoryEntry = readdir(directory)) != NULL)
    {
        if (directoryEntry->d_name[0] == '.' || directoryEntry->d_name[1] == '.')
            continue;

        content += "<a href=\"" + url + "/" + directoryEntry->d_name + "\">" + directoryEntry->d_name + "</a>\n";
    }

    closedir(directory);

    content += "</pre><hr>\n"
               "</body>\n"
               "</html>";
    std::ostringstream contentLengthStream;
    contentLengthStream << content.length();
    std::string contentLengthStr = contentLengthStream.str();
    response = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: " + contentLengthStr + "\r\n\r\n" + content;

    if (send(fd, response.c_str(), response.length(), 0) < 1)
        c->is_done = true;
    ;
    return;
}

void send_http_response(Network *c, std::string& filepath) {
    // Open the file
    std::ifstream file(filepath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filepath << std::endl;
        return;
    }

    // Read the file into a string
    std::string body;
    std::string line;
    while (std::getline(file, line)) {
        body += line + "\n";
    }
    size_t pos = body.find("<!DOCTYPE html>");
    if (pos != std::string::npos) {
        body = body.substr(pos);
    }
    std::string headers;
    std::ostringstream oss;
    oss << "Content-Length: " << body.length() << "\r\n";
    headers = oss.str() + "Content-Type: text/html\r\n" + "\r\n";
    std::string response = "HTTP/1.1 200 OK\r\n" + headers + "\r\n" + body;

    send(c->get_socket_fd(), response.c_str(), response.length(), 0);
}

void handle_get(Network *c, char *args[3])
{
    std::string cookie = c->request.get_headers()["Cookie"];
    char *env[] = {strdup("PATH_INFO=root/cgi-bin/cookie.php"), strdup(("HTTP_COOKIE=" + cookie).c_str()), NULL};
    int fd = open("cgi-env/output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    int outpt = dup(1);

    dup2(fd, 1);
    pid_t pid = fork();
    if (pid == 0)
    {
        if (execve(args[0], args, env) == -1)
        {
            for (int i = 1; i < 3; i++){
                free(args[i]);
                free(env[i]);
            }
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0)
    {
        // This is the parent process
        int status;

        if (waitpid(pid, &status, WNOHANG) == 0)
        {
            // Child process is still running
            sleep(1);

            if (waitpid(pid, &status, WNOHANG) == 0)
            {
                // Child process did not complete within the timeout
                kill(pid, SIGKILL);
                lseek(fd, 0, SEEK_END);
                const char *timeoutMessage = "This request took too long time.\n";
                if (write(fd, timeoutMessage, strlen(timeoutMessage)) < 1)
                    c->is_done = true;
            }
        }
        dup2(outpt, 1);
        c->_file_name = "cgi-env/output.txt";
        send_http_response(c, c->_file_name);
        for (size_t i = 0; i < 2; i++){
            free(env[i]);
            free(args[i]);
        }
        c->is_done = true;
        return;
    }
    else
    {
        for (int i = 1; i < 3; i++){
            free(args[i]);
            free(env[i]);
        }
        perror("fork");
    }
}

int Response::check_which_server(Network *c)
{
    Response r;
    std::string host = c->request.get_headers()["Host"];
    if (host.empty())
    {
        r.send_err(c->get_socket_fd(), 400);
        c->is_done = true;
        return 0;
    }
    std::string host_c = host.substr(0, host.find(":"));
    std::string port_c = host.substr(host.find(":") + 1, host.size());
    for (size_t j = 0; j < cnf->serverConfigs.size(); j++)
    {
        for (size_t k = 0; k < cnf->serverConfigs[j].serverName.size(); k++)
        {
            if (cnf->serverConfigs[j].serverName[k] == host_c || cnf->serverConfigs[j].host == host_c)
            {
                if ((cnf->serverConfigs[j].port == port_c))
                    return j;
            }
        }
    }
    return -1;
}