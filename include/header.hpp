#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <fstream>
#include <iterator>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <sys/wait.h>
#include "configue.hpp"
#include "ContentType.hpp"
#include "network.hpp"
#include "request.hpp"
#include "response.hpp"

extern ServerConfig *cnf;


#define SLISTEN true

#define PORT "8011"
#define BACK_LOG 1024
#define BUFFER_SIZE 4096
#define CRLF "\r\n"
#define CNF "conf"
#define WA7ED 1
#define NO9TA "."
#define SLASH "/"
#define ZERO 0
#define SEP "\r\n\r\n"
#define LINE_SEP "\r\n"
#define COLON ": "
#define SPACE ' '
#define POST "POST"
#define GET "GET"
#define DELETE "DELETE"
#define CONTENT "Content-Type"
#define HOST "Host"
#define WALO ""



#endif
