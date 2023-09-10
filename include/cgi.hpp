#include "./network.hpp"

class Cgi
{
    private :
        int statusCode;
        char **Envp;
        pid_t pid;
        int input;
        int output;

        std::string outFileName, tmpOutFileName;
        int outFileFd;
    public :
    // Cgi();
    // ~Cgi();
    // void initEnv(Request &);
    int runCgi(std::string &, std::string &, std::string &, Request &);
    // std::string &cgiHandler(Request &);
};