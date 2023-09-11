#ifndef CGI_HPP
#define CGI_HPP

// #include "./header.hpp"
// #define BUFFER_SIZE 3072
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
    int fdOut;



    Cgi();
    ~Cgi();
    // void initEnv(Request &);
    void exeCgi(Request &, std::string &, std::string &);
    std::string &cgiHandler(Request &, std::string &, std::string &, std::string &);
};
    std::string genFileName();

#endif 