#include"header.hpp"

ContentType::ContentType()
{
	_ext_types[".html"] = "text/html";
    _ext_types[".htm"] = "text/html";
    _ext_types[".css"] = "text/css";
    _ext_types[".ico"] = "image/x-icon";
    _ext_types[".avi"] = "video/x-msvideo";
    _ext_types[".bmp"] = "image/bmp";
    _ext_types[".doc"] = "application/msword";
    _ext_types[".gif"] = "image/gif";
    _ext_types[".gz"] = "application/x-gzip";
    _ext_types[".ico"] = "image/x-icon";
    _ext_types[".jpg"] = "image/jpeg";
    _ext_types[".jpeg"] = "image/jpeg";
    _ext_types[".png"] = "image/png";
    _ext_types[".txt"] = "text/plain";
    _ext_types[".mp3"] = "audio/mp3";
    _ext_types[".pdf"] = "application/pdf";
    _ext_types["default"] = "text/html";
}

std::string ContentType::getConteType(std::string find){
	if (_ext_types.count(find))
		return (_ext_types[find]);
	return (_ext_types["default"]);
}

ContentType::~ContentType()
{
}