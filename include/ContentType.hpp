#ifndef CONTENTTYPE_HPP
#define	CONTENTTYPE_HPP

#include"header.hpp"

class ContentType
{
private:
	std::map<std::string, std::string> _ext_types;

public:
	std::string getConteType(std::string compare);
	ContentType();
	~ContentType();
};

#endif //CONTENTTYPE_HPP
