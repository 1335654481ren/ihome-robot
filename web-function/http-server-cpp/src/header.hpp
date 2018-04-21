#ifndef __HEADER_HPP__
#define __HEADER_HPP__

#include <string>
#include <vector>

namespace http {
    namespace server {

	struct header
	{
	    std::string name;
	    std::string value;
	};
	struct parser
	{	
	    std::string key;
	    std::string value;
	};
	struct Req_Parser
	{	
	    std::string body;
	    std::vector<parser> numbers;
	};
    } // namespace server
} // namespace http

#endif // HTTP_HEADER_HPP
