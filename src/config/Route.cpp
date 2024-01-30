/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:47:50 by alevasse          #+#    #+#             */
/*   Updated: 2023/06/12 14:00:07 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include "ServerConfig.hpp"

Route::Route() : _path(""), _root(""), _directory_listing(false), _default_file("") {
	return ;
}

const std::string	Route::getPath() const {
	return (_path);
}

const std::string	Route::getRoot() const {
	return (_root);
}

bool	Route::getDirectoryListing() const {
	return (_directory_listing);
}

const std::string	Route::getDefaultFile() const {
	return (_default_file);
}

const std::map<std::string, std::string>	Route::getUpload() const {
	return (_upload);
}

void	Route::setPath(const std::string &path) {
	_path = path;
}

void	Route::addAllowedMethod(const std::string &method) {
	_allowed_methods.push_back(method);
}

void	Route::setRoot(const std::string &root) {
	_root = root;
}

void	Route::setDirectoryListing(bool directory_listing) {
	_directory_listing = directory_listing;
}

void	Route::setDefaultFile(const std::string &default_file) {
	_default_file = default_file;
}

void    Route::setRedirect(const std::string &url) {
    _redirect = url;
}

void	Route::addUpload(const std::string &path, const std::string &value) {
	_upload[path] = value;
}

void	Route::addCgi(const std::string &path, const std::vector<std::string> &cgi_config) {
	_cgi[path] = cgi_config;
}

void	Route::parseRoute(std::string::iterator &it, const std::string::iterator &end) {
	bool	path_found = false;

	while (it != end) {
		ServerConfig::skipSpace(it, end);
		std::string	key = ServerConfig::parseString(it, end);

		if (it == end || *it != ':') {
			throw std::runtime_error("syntax error: ':' expected after a keyword");
		}
		++it;
		ServerConfig::skipSpace(it, end);
		if (key == "path") {
			_path = ServerConfig::parseString(it, end);
			path_found = true;
		} else if (key == "allowed_methods") {
			_allowed_methods = parseVectorString(it, end);
		} else if (key == "root") {
			_root = ServerConfig::parseString(it, end);
		} else if (key == "directory_listing") {
			_directory_listing = parseBoolean(it, end);
		} else if (key == "default_file") {
			_default_file = ServerConfig::parseString(it, end);
		} else if (key == "upload") {
			_upload = parseMapString(it, end);
		}
		else if (key == "redirect") {
			_redirect = ServerConfig::parseString(it, end);
		} 
		else if (key == "cgi") {
			_cgi = parseMapVector(it, end);
		} else {
			throw std::runtime_error("syntax error: unknown key '" + key + "'");
		}
		ServerConfig::skipSpace(it, end);
		if (it != end && *it == ',') {
			++it;
		} else if (it == end || *it != '}') {
			throw std::runtime_error("syntax error: '}' or ',' expected for a Route object");
		} else {
			++it;
			break ;
		}
	}
	if (!path_found) {
		throw std::runtime_error("syntax error: 'path' key is missing");
	}
}

bool	Route::parseBoolean(std::string::iterator &it, const std::string::iterator &end) {
	if (it == end) {
		throw std::runtime_error("syntax error: unexpected end of file");
	}

	std::string	boolean;

	while (it != end && (*it != ' ' && *it != ',' && *it != '\n')) {
		boolean.push_back(*it);
		++it;
	}
	if (boolean == "true") {
		return (true);
	} else if (boolean == "false") {
		return (false);
	} else {
		throw std::runtime_error("syntax error: boolean value (true or false) expected");
	}
}

std::vector<std::string>	Route::parseVectorString(std::string::iterator &it, const std::string::iterator &end) {
	std::vector<std::string>	result;

	if (it == end || *it != '[') {
		throw std::runtime_error("syntax error: '[' expected for a vector of strings");
	}
	++it;
	while (true) {
		ServerConfig::skipSpace(it, end);
		if (it != end && *it == ']') {
			++it;
			break ;
		}

		std::string	str = ServerConfig::parseString(it, end);

		result.push_back(str);
		ServerConfig::skipSpace(it, end);
		if (it != end && *it == ',') {
			++it;
			ServerConfig::skipSpace(it, end);
            if (it == end || *it != '\"') {
                throw std::runtime_error("syntax error: new expected value");
            }
		} else if (it == end || *it != ']') {
			throw std::runtime_error("syntax error: ',' or ']' expected for a vector of strings");
		}
	}
	if (result.empty()) {
		throw std::runtime_error("syntax error: vector of strings cannot be empty");
	}
	return (result);
}

std::map<std::string, std::vector<std::string> >	Route::parseMapVector(std::string::iterator &it, const std::string::iterator &end) {
	std::map<std::string, std::vector<std::string> >	result;

	if (it == end || *it != '{') {
		throw std::runtime_error("syntax error: '{' expected for a map of vector");
	}
	++it;
	while (true) {
		ServerConfig::skipSpace(it, end);
		if (it != end && *it == '}') {
			++it;
			break ;
		}

		std::string key = ServerConfig::parseString(it, end);

		if (result.find(key) != result.end()) {
			throw std::runtime_error("syntax error: duplicate key in a map of vector");
		}
		ServerConfig::skipSpace(it, end);
		if (it == end || *it != ':') {
			throw std::runtime_error("syntax error: ':' expected for a key-value pair in a map of vector");
		}
		++it;
		ServerConfig::skipSpace(it, end);

		std::vector<std::string>	value = parseVectorString(it, end);

		result[key] = value;
		ServerConfig::skipSpace(it, end);
		if (it != end && *it == ',') {
			++it;
			ServerConfig::skipSpace(it, end);
            if (it == end || *it != '\"') {
                throw std::runtime_error("syntax error: new expected value");
            }
		} else if (it == end || *it != '}') {
			throw std::runtime_error("syntax error: ',' or '}' expected for a map of vector");
		}
	}
	return (result);
}

std::map<std::string, std::string>	Route::parseMapString(std::string::iterator &it, const std::string::iterator &end) {
	std::map<std::string, std::string>	result;
	std::string							key;
	std::string							value;

	ServerConfig::skipSpace(it, end);
	if (it == end || *it != '{') {
		throw std::runtime_error("syntax error: '{' expected for a map");
	}
	++it;
	while (true) {
		ServerConfig::skipSpace(it, end);
		if (it != end && *it == '}') {
			++it;
			break ;
		}

		std::string	key = ServerConfig::parseString(it, end);
		if (it == end || *it != ':') {
			throw std::runtime_error("syntax error: ':' expected after a key in a map");
		}
		++it;
		ServerConfig::skipSpace(it, end);
		if (it == end) {
			throw std::runtime_error("syntax error: unexpected end of file");
		}
		if (key == "enabled") {
			bool	enabled = parseBoolean(it, end);
			value = enabled ? "true" : "false";
		} else {
			value = ServerConfig::parseString(it, end);
		}
		if (result.find(key) != result.end()) {
			throw std::runtime_error("error: duplicate key detected for key " + key);
		}
		result[key] = value;
		ServerConfig::skipSpace(it, end);
		if (it != end && *it == ',') {
			++it;
			ServerConfig::skipSpace(it, end);
			if (it == end || *it != '\"') {
                throw std::runtime_error("syntax error: new expected value");
            }
		} else if (it == end || *it != '}') {
			throw std::runtime_error("syntax error: ',' or '}' expected for a map");
		}
	}
	return (result);
}

bool    Route::isAllowedMethods(int method) const {
	std::string method_name;
	if (method == GET)
		method_name = "GET";
	else if (method == POST)
		method_name = "POST";
	else if (method == DELETE)
		method_name = "DELETE";
	else
		method_name = "OTHER";
    if (std::find(_allowed_methods.begin(), _allowed_methods.end(), method_name) != _allowed_methods.end()) {
        return (true);
    } else {
        return (false);
    }
}

bool    Route::isAllowedCgi(const std::string &cgi) const {

    std::map<std::string, std::vector<std::string> >::const_iterator it = _cgi.find("extension");
    if (it != _cgi.end()) {
        std::vector<std::string> extensionVector = it->second;
        for (std::vector<std::string>::const_iterator vecIt = extensionVector.begin(); vecIt != extensionVector.end(); ++vecIt) {
			if (*vecIt == cgi)
				return true;
        }
    }
	return false;
}

std::string	Route::getCgiPath() const {
    std::map<std::string, std::vector<std::string> >::const_iterator it = _cgi.find("cgi_path");
    if (it != _cgi.end()) {
        std::vector<std::string> extensionVector = it->second;
		std::string test = extensionVector[0];
		return test;
        /*if (it->second.size())
		{
			std::string ret = *(it)->second
			return ;
		}*/
    }
    throw std::runtime_error("No cgi matches with the argument");
}

const std::string   Route::getRedirect() const {
    return (_redirect);
}