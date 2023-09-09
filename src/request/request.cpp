#include "header.hpp"

Request::Request() : chunk_size(0), first_body(true), rcv_size(0),finished(false), is_err(0), req_size(0)
{
  if (map.empty())
  {
    map["text/html"] = "html";
    map["image/png"] = "png";
    map["video/mp4"] = "mp4";
    map["audio/mpeg"] = "mp3";
    map["text/css"] = "css";
    map["text/javascript"] = "js";
    map["application/json"] = "json";
    map["application/xml"] = "xml";
    map["application/pdf"] = "pdf";
    map["application/zip"] = "zip";
    map["text/plain"] = "txt";
    map["image/gif"] = "gif";
    map["image/jpeg"] = "jpg";
    map["image/svg+xml"] = "svg";
    map["audio/wav"] = "wav";
    map["video/mpeg"] = "mpg";
    map["video/quicktime"] = "mov";
    map["video/x-msvideo"] = "avi";
    map["text/x-python-script"] = "py";
    map["text/x-php-script"] = "php";
  }
}

Request::~Request()
{
}

void Request::handle_req_l(std::string req_line)
{
  size_t pos = req_line.find(SPACE);
  if (pos != std::string::npos)
    met = req_line.substr(ZERO, pos);
  size_t new_pos = req_line.find(SPACE, pos + WA7ED);
  if (new_pos != std::string::npos)
    loc = req_line.substr(pos + WA7ED, new_pos - pos - WA7ED);
  else
    loc = req_line.substr(pos + WA7ED);
}

void Request::handle_headers(std::string header)
{
  size_t pos = header.find(LINE_SEP) + 2;
  handle_req_l(header.substr(0, pos));

  while (pos < header.length())
  {
    size_t end_pos = header.find(LINE_SEP, pos);
    if (end_pos == std::string::npos)
      end_pos = header.length();
    size_t colon_pos = header.find(COLON, pos);
    if (colon_pos == std::string::npos)
      break;
    std::string key = header.substr(pos, colon_pos - pos);
    std::string value = header.substr(colon_pos + 2, end_pos - colon_pos - 2);
    headers[key] = value;
    pos = end_pos + 2;
  }
}

std::string Request::parse_chunked(std::string input)
{
  std::string chunked;

  if (first_body)
  {
    input.insert(0, "\r\n");
    first_body = false;
  }
  input.insert(0, tmp);
  tmp = WALO;
  while (input.length() > 0)
  {
    if (chunk_size >= (int)input.length())
    {
      chunked.append(input);
      chunk_size -= (int)input.length();
      break;
    }
    else
    {
      if (chunk_size > 0)
      {
        chunked.append(input.substr(0, chunk_size));
        input = input.substr(chunk_size, input.length() - chunk_size);
      }
      int start = 2;
      int pos = input.find("\r\n", start);
      if (pos == (int)std::string::npos)
      {
        tmp = input;
        chunk_size = 0;
        break;
      }
      std::string chunk_size_str = input.substr(start, pos - start);
      std::istringstream iss(chunk_size_str);
      iss >> std::hex >> chunk_size;
      if (iss.fail())
      {
        is_err = 400;
        break;
      }
      if (chunk_size == 0)
      {
        finished = true;
        break;
      }
      input = input.substr(pos + 2, (int)input.length() - pos - 2);
    }
  }
  return (chunked);
}

void Request::handle_body(std::string req_body)
{
  std::time_t currentTime = std::time(NULL);
  std::stringstream ss;
  ss << currentTime;
  std::string time = ss.str();
  std::string chunk;

  if (first_body)
  {
    file_name = cnf->serverConfigs[srv_index].locations[location_index].uploadPath 
          + SLASH + time + NO9TA + map[headers[CONTENT]];
    file = new std::ofstream(file_name.c_str(), std::ios_base::app);
  }
  if (req_size) {
    first_body = false;
    if (rcv_size < req_size) {
      *file << req_body;
      rcv_size += req_body.length();
    }
    if (rcv_size == req_size) {
      finished = true;
      file->close();
      delete file;
    }
  }
  else {
    chunk = parse_chunked(req_body);
    *file << chunk;
    rcv_size += chunk.length();
    if (rcv_size > max_body_size) {
      is_err = 413;
      file->close();
      delete file;
      if (std::remove(file_name.c_str()) != 0){
        std::cout << "Error deleting the file" << std::endl;
      }else{
        std::cout << "File deleted successfully" << std::endl;
      }
    }
    if (finished) {
      file->close();
      delete file;
    }
  }
}



std::string Request::get_met()
{
  return met;
}

std::string Request::get_loc()
{
  return loc;
}

std::map<std::string, std::string> Request::get_headers()
{
  return headers;
}