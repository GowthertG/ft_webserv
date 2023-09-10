#ifndef CGI_HPP
#define CGI_HPP

#include "../ft_webserv/include/header.hpp"
# define METHOD "get"
# define CGI_BIN
#define FILE_PATH
#define BUFFER_SIZE 3072
class Cgi
{
    public:
    char **Envp;
    pid_t pid;
    int statusCode;
    int input;
    int output;
    std::string outfileName, tmpOutFileName;
    int outfileFd;



    Cgi();
    ~Cgi();
    void initEnv(Request &);
    void exeCgi(Request &);
    std::string & cgiHandler(Request &);
};

#endif 