#include "../../include/cgi.hpp"

#define BUFFER_SIZE 3072

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

void Cgi::initEnv(Request &request)
{
  (void) (request);
  int index = 0;
  std::string tmp;
  std::string contentLenghtValue;
  std::string contentTypeValue;
  int find;
  typedef std::map<std::string, std::string>::iterator iterator;
  for (iterator IT = request.headers.begin(); IT != request.headers.end();
       IT++) {
    if (IT->first == "content-lenght") {
      (contentLenghtValue = IT->second);
      continue;
    } else if (IT->first == "content-type") {
      contentTypeValue = IT->second;
      continue;
    }

    for (size_t index = 0; index < tmp.length(); ++index)
      tmp[index] = std::toupper(tmp[index]);
    tmp = "HTTP_" + IT->first + "=";
    find = tmp.find("-");
    if (find != static_cast<int>(std::string::npos))
      tmp[find] = '_';
    tmp.append(IT->second);
    Envp[index++] = strdup(tmp.c_str());
  };
  Envp[index] = NULL;
  index = 0;
  // Test : 
  /*while (Envp[index])
    std::cout << Envp[index++] << std::endl; */
  Envp[index++] = strdup(("CONTENTTYPE=" + contentTypeValue).c_str());
  Envp[index++] = strdup(("CONTENTLENGHT=" + contentLenghtValue).c_str());
  // i need method
  //Envp[index++] = strdup(("REQUEST_METHOD=" + request.requestLine[0]).c_str());
  Envp[index++] = strdup("REDIRECT_STATUS=200");
  // i need file path
  //Envp[index++] = strdup(request.filePath.c_str());
  // i need file path
  //Envp[index++] = strdup(("SCRIPT_FILENAME=" + request.filePath).c_str());
  // i need query 
  //Envp[index++] = strdup(("QUERY_STRING=" + request.query).c_str());
  Envp[index++] = NULL;
};

std::string &Cgi::cgiHandler(Request &request, std::string &CgiBin, std::string &whichScript, std::string &fileToExecute)
{
    char buffer[BUFFER_SIZE];
    std::string line, remain;
    int pos, nRead;
    memset(buffer, NULL, BUFFER_SIZE);
    if (access(fileToExecute.c_str(), R_OK))
      // throw error
    if (access(CgiBin.c_str() , X_OK))
      // throw error
    

    tmpOutFileName = "tmpOutPut" + genFileName();
    output = open (tmpOutFileName.c_str(), O_RDWR);
    if (output == != -1)
    {
      perror("open");
      exit(0);
    }
    if (request.get_met() == "POST")
    {
      if (fileToExecute.empty())
        std::cout << "throw error";
        input = open(fileToExecute.c_str(), O_RDWR);
        if (input == -1)
          std::cout << "throw error or exit ";
    }
    outfileName = "body" + genFileName() + ".html";
    outfileFd = open(outfileName.c_str(), O_CREAT | O_RDWR, 0664);
    if (outfileFd == -1)
      std::cout << "exit with error";
    nRead = read(outfileFd, buffer, BUFFER_SIZE);
    line.append(buffer, nRead);
    if (line.find("\r\n\r\n") != std::string::npos)
    {
      remain = line.substr(pos + 4);
      write(outfileFd, remain.c_str(), remain.size());
    }
    else
      write(outfileFd, &buffer, nRead);
    while (nRead)
    {
      memset(buffer, 0, BUFFER_SIZE);
      nRead = read(outfileFd, buffer, BUFFER_SIZE);
      if (nRead)
        write(fd_out, &buffer, nRead);
    }
    close(outfileFd);
    return (outfileName);
}

std::string genFileName()
{
  std::string baseName = "Cgi";
  std::srand(static_cast <unsigned int> (std::time(0)));
  std::stringstream ss;
  ss << baseName << "number" << "_" << std::rand();
  return(ss.str());
}