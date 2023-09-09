#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "header.hpp"

class Request
{
private:
  int chunk_size;
  bool first_body;
  size_t rcv_size;
  std::string met;
  std::string loc;
  std::string request_file;
  std::string body;
  std::string tmp;
  std::map<std::string, std::string> map;
  std::ofstream *file;
  std::string file_name;

public:
  Request();
  ~Request();

  void handle_headers(std::string header);
  void handle_req_l(std::string req_line);
  std::string get_loc();
  std::string get_met();
  std::map<std::string, std::string> get_headers();
  void handle_body(std::string req_body);
  std::string parse_chunked(std::string input);

  bool finished;
  int is_err;
  size_t req_size;
  int srv_index;
  int location_index;
  size_t max_body_size;
  std::map<std::string, std::string> headers;
};
#endif