/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestMethod.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:25:41 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/12 15:42:55 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestMethod.hpp"
#include "Response.hpp"

RequestMethod::RequestMethod(std::string header, std::string body, ServerConfig config) :
	_reqbody(body), _config(config), _reqheader(header) {}

int RequestMethod::getHeaderRequest(const std::vector<std::string> &vec)
{
	if (vec.size() != 3)
		return 400;
	if (vec[0] == "GET")
		_type = GET;
	else if (vec[0] == "POST")
		_type = POST;
	else if (vec[0] == "DELETE")
		_type = DELETE;
	else if (vec[0] == "HEAD")
		_type = HEAD;
	else
		return 400;
 	if (vec[2] == "HTTP/1.1")
		_http = "1.1";
	else
		return 505;
	if (vec[1].size() <= 2024)
		_uri = decodeUrl(vec[1]);
	else
		return 414;
	return 0;
}

int RequestMethod::getCookieInfos(std::string line)
{
	std::size_t equal;
	std::size_t end;
	std::string id;
	std::string value;
	while (line.find(";") != std::string::npos) {
		equal = line.find("=");
		end = line.find(";");
		if (equal != std::string::npos)
		{
			id = line.substr(0, equal);
			if (end != std::string::npos)
				value = line.substr(equal + 1, end - equal - 1);
			else
				value = line.substr(equal + 1);
		}
		_cookie[id] = value;
		line.erase(0, end + 2);
	}
	equal = line.find("=");
	if (equal != std::string::npos)
	{
		id = line.substr(0, equal);
		value = line.substr(equal + 1);
	}
	_cookie[id] = value;
	return 0;
}

int RequestMethod::getHostInfos(const std::vector<std::string> &vec)
{
	if (vec.size() != 2 || vec[0] != "Host:")
		return 400;
	if (!_host.size())
		_host = vec[1];
	else
		return 400;
	return 0;
}

int	RequestMethod::getUserAgentInfos(const std::vector<std::string> &vec) {
	if (vec.size() < 2) {
		return 400;
	}
	std::vector<std::string>::const_iterator	it;
	for (it = vec.begin() + 1; it != vec.end(); ++it) {
		_user_agent += " " + *it;
	}
	_user_agent.erase(0, 1);
	return 0;
}

int	RequestMethod::getContentLengthReq(const std::vector<std::string> &vec) {
	if (vec.size() != 2) {
		return 400;
	}
	_reqContentSize = std::atoi(vec[1].c_str());
	if (getType() == POST && !getContentSize())
		return 411;
	return 0;
}

int RequestMethod::setLine(std::string &line){
	std::vector<std::string> vec;
	vec = splitLine(line);
	std::vector<std::string>::iterator it = vec.begin();
	int error = 0;
	if (vec.size() && *it == "Cookie:")
		error = getCookieInfos(line.substr(vec[0].size() + 1));
	else if (vec.size() && *it == "Host:")
		error = getHostInfos(vec);
	else if (vec.size() && *it == "User-Agent:")
		error = getUserAgentInfos(vec);
	else if (vec.size() && *it == "Content-Length:")
		error = getContentLengthReq(vec);
	return error;
}

int  RequestMethod::initRequest() {
	std::string line;
	std::vector<std::string> vec;
	std::size_t pos = 0;
	while ((pos = _reqheader.find("\r", pos)) != std::string::npos)
		_reqheader.erase(pos, 1);
	pos = 0;
	if ((pos = _reqheader.find('\n', pos)) != std::string::npos)
	{
		line = _reqheader.substr(0, pos);
		_reqheader.erase(0, pos + 1);
		if (line.size())
			vec = splitLine(line);
		int error = getHeaderRequest(vec);
		if (error)
			return error;
		_uri = vec[1];
		if (_uri == "/alevasse" || _uri == "/cjunker" || _uri == "/flemaitr") {
            _redirectUrl = checkRedirect(_uri);
        }
		while (1) {
			pos = 0;
			pos = _reqheader.find('\n', pos);
			if (pos == std::string::npos) {
				line = _reqheader.substr(0);
				if (line.size())
				{
					line = decodeUrl(line);
					setLine(line);
				}
				break;
			}
			line = _reqheader.substr(0, pos);
			_reqheader.erase(0, pos + 1);
			if (line.size())
				setLine(line);
		}
	}
	else
		return 400;
	return 0;
}

std::string RequestMethod::checkRedirect(const std::string& path) {
    return (_config.getRoutes(path).getRedirect());
}

