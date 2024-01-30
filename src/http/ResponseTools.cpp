/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:26:09 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/12 15:46:44 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Response.hpp"
#include "../config/ServerConfig.hpp"

std::map<std::string, bool> g_cookie;

bool Response::isAllowedExtension(const std::string &extension)
{
	try {
		_mimeMap.isInMap(extension);
	}
	catch (const std::exception &e) {
		return 0;
	}
	return 1;
}

void Response::setExtension(const std::string &str)
{
	if (str.find_last_of('.') != std::string::npos) {
		_extension = str.substr(_file_name.find_last_of('.'));
		_content_type = _mimeMap.getMime(_extension);
	}
	else {
		_extension = ".txt";
		_content_type = _mimeMap.getMime(_extension);
	}
}

void replaceWord(std::string &string, const std::string &to_search, const std::string &to_replace)
{
	std::string::size_type pos;
	std::string b = "/";

	if ((pos = string.find(to_search, 0)) != std::string::npos) {
		if ((string[pos] != '/' && to_replace[0] != '/') && (to_replace[to_replace.size() - 1] != '/' && string[pos + to_search.size()] != '/'))
			string = string.substr(0, pos) + b + to_replace + b + string.substr(pos + to_search.size());
		else if (string[pos] != '/' && to_replace[0] != '/')
			string = string.substr(0, pos) + b + to_replace + string.substr(pos + to_search.size());
		else if (to_replace[to_replace.size() - 1] != '/' && string[pos + to_search.size()] != '/')
			string = string.substr(0, pos) + to_replace + b + string.substr(pos + to_search.size());
		else
			string = string.substr(0, pos) + to_replace + string.substr(pos + to_search.size());
	}
}

void Response::checkSecurityPath(std::string path)
{
	std::string::size_type pos;
    if ((pos = path.find("..")) != std::string::npos)
		throw std::runtime_error("403");
}

void Response::multiBackslash(std::string &string)
{
	std::string::size_type pos;
    while ((pos = string.find("//")) != std::string::npos)
		string.erase(pos, pos + 1);
}

int Response::setRoute(std::string path)
{
    try {
        _route = &_req.getConf().getRoutes(path);
        if (_route->getRoot().size()) {
			if (_path_clear != _route->getRoot() + _route->getDefaultFile())
		    	replaceWord(_path_clear, path, _route->getRoot());
		}
        else {
			if (_req.getConf().getRootConfig()[_req.getConf().getRootConfig().size() - 1] != '/' && _path_clear[0] != '/')
				_path_clear = "/" + _path_clear;
			_path_clear = _req.getConf().getRootConfig() + _path_clear;
		}
    }
    catch (const std::exception &e) {
        std::string::size_type pos;
        if ((pos = path.find_last_of('/')) != std::string::npos) {
			if (pos == 0 && path.size() > 1)
				path = "/";
			else
            	path = path.substr(0, pos);
            setRoute(path);
        }
        return 0;
    }
    return 1;
}


void Response::setPath(void)
{
	std::string::size_type pos;
	_path_clear = _req.getUri();
	checkSecurityPath(_path_clear);
	multiBackslash(_path_clear);
	if ((pos = _path_clear.find_last_of('?')) != std::string::npos) {
		_post_extension = _path_clear.substr(pos + 1);
		_path_clear = _path_clear.substr(0, pos);
	}
	setRoute(_path_clear);
	cleanPath(_path_clear);
	if (!isValidDirectory(_path_clear)) {
		if ((pos = _path_clear.find_last_of('/')) != std::string::npos) {
			_file_name = _path_clear.substr(pos + 1);
			_path_clear = _path_clear.substr(0, pos);		
		}
		if ((pos = _file_name.find_last_of('.')) != std::string::npos) {
			setExtension(_file_name);
		}
		_content_type = _mimeMap.getMime(_extension);
	}
	if (_route && _route->getDefaultFile().size() && !_file_name.size()) {
		_file_name = _route->getDefaultFile();
		setExtension(_file_name);
		_content_type = _mimeMap.getMime(_extension);
	}
	cleanPath(_path_clear);
	if (_file_name.size())
		_path_clear = _path_clear + "/";
	if (!_file_name.size() && _extension.size())
		throw std::runtime_error("403");
	_file_name = decodeUrl(_file_name);
}

void Response::cleanPath(std::string &str)
{
	while (str[str.size() - 1] == '/')
		str.erase(str.size() - 1);
	while (str[0] == '/')
		str = str.substr(1);
}

void Response::fileToBinary(const std::string &path_file)
{
	std::ifstream file(path_file.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("404");
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string image_data = buffer.str();
	_content = image_data;
}

void Response::readMap(void)
{
	std::map<std::string, bool> ::iterator it = g_cookie.begin();
	std::map<std::string, bool> ::iterator ite = g_cookie.end();
	std::cout << YELLOW << "Cookie Database" << RESET << std::endl;
	int i = 0;
	while (it != ite) {
		i++;
		std::cout << YELLOW << "[" << i << "] " << it->first << RESET << std::endl;
		it++;
	}
}
