#include "../../include/cgi.hpp"
int Cgi::runCgi(std::string &CgiBin, std::string &whichScript, std::string &fileToExecute, Request &request)
{
  //statusCode = 0 

  pid = fork();
  if (!pid)
  {
    char **args = new char *[3];
    statusCode = 0;
    //why all of this ?
    //     if (ex == "python")
    // {
    //     args[0] = strdup("/usr/bin/python");
    //     args[1] = strdup(path.c_str());
    //     args[2] = NULL;
    // }
    // else if (ex == "php")
    // {
    //     args[0] = strdup("cgi-env/php-cgi");
    //     args[1] = strdup(path.c_str());
    //     args[2] = NULL;
    // }
    // else
    // {
    //     if (ex != "python" && ex != "php")
    //     {
    //         for (int i = 0; i < 2; i++){
    //             free(args[i]);
    //             free(env[i]);
    //         }
    //         return 0;
    //     }
    // }
    args[0] = strdup(CgiBin.c_str());
    args[1] = strdup(fileToExecute.c_str());
    args[2] = NULL;
    // Envp = new char *[request.headers.size() + 8];
    // initEnv(request);
    // int index = 0;
    if (request.get_met() == "POST")
    {
      dup2(input, STDIN_FILENO);
      close(input);
    }
    dup2(output, STDOUT_FILENO);
    close(output);
    execve(args[0], args, Envp);
    // throw error and send exit code
  }
  if (request.get_met() == "POST")
    close (input);
  close(output);
  waitpid(pid, &statusCode, 0);
}