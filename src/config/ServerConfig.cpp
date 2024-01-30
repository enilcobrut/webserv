/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:47:55 by alevasse          #+#    #+#             */
/*   Updated: 2023/06/13 11:22:47 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "Route.hpp"

ServerConfig::ServerConfig() : _host(""), _server_name(""), _client_max_body_size(0), _root_config("") {
	return ;
}

void	ServerConfig::parseServerConfig(std::string::iterator &it, const std::string::iterator &end) {
	std::set<std::string>	encountered_keywords;

	while (it != end) {
		skipSpace(it, end);

		std::string	keyword = parseString(it, end);

		if (encountered_keywords.count(keyword) > 0) {
			throw std::runtime_error("error: duplicate keyword: " + keyword);
		}
		encountered_keywords.insert(keyword);
		if (it == end || *it != ':') {
			throw std::runtime_error("syntax error: ':' expected after a keyword");
		}
		++it;
		skipSpace(it, end);
		if (keyword == "host") {
			_host = parseString(it, end);
		} else if (keyword == "port") {
			_ports = parseArray(it, end);
		} else if (keyword == "server_name") {
			_server_name = parseString(it, end);
		} else if (keyword == "root") {
            _root_config = parseString(it, end);
        } else if (keyword == "error_pages") {
			_error_pages = parseMap(it, end);
		} else if (keyword == "client_max_body_size") {
			_client_max_body_size = parseInteger(it, end);
		} else if (keyword == "routes") {
			_routes = parseRoutes(it, end);
		} else {
			throw std::runtime_error("error: unknown keyword: " + keyword);
		}
		skipSpace(it, end);
		if (it != end && *it == ',') {
			++it;
			continue ;
		} else if (it != end && *it == '}') {
			++it;
			break ;
		} else {
			throw std::runtime_error("syntax error: '}' or ',' expected");
		}
	}

	std::string	required_keywords_arr[] = {"host", "port", "server_name", "root", "error_pages",
		"client_max_body_size", "routes"};
	size_t	required_keywords_size = sizeof(required_keywords_arr) / sizeof(required_keywords_arr[0]);

	for (size_t i(0); i < required_keywords_size; ++i) {
		const std::string &keyword = required_keywords_arr[i];
		if (encountered_keywords.count(keyword) == 0) {
			throw std::runtime_error("error: missing keyword: " + keyword);
		}
	}
}

const std::string	&ServerConfig::getHost() const {
	return (_host);
}

std::vector<int>	ServerConfig::getPorts() const {
	return (_ports);
}

const std::string	&ServerConfig::getServerName() const {
	return (_server_name);
}

const std::map<int, std::string>	ServerConfig::getErrorPages() const {
	return (_error_pages);
}

std::size_t	ServerConfig::getClientMaxBodySize() const {
	return (_client_max_body_size);
}

const Route &ServerConfig::getRoutes(const std::string &route_name) const {
    std::vector<Route>::const_iterator  it;
    for (it = _routes.begin(); it != _routes.end(); ++it) {
        if (it->getPath() == route_name) {
            return (*it);
        }
    }
    throw std::runtime_error("No route matches with the argument");
}

void	ServerConfig::setHost(const std::string &host) {
	_host = host;
}

void	ServerConfig::setConfigFileName(const std::string &name) {
	_config_file_name = name;
}

const std::string &ServerConfig::getNameConfigFile(void) const {
	return (_config_file_name);
}

void	ServerConfig::addPort(int port) {
	_ports.push_back(port);
}

void	ServerConfig::setServerName(const std::string &server_name) {
	_server_name = server_name;
}

void	ServerConfig::addErrorPage(int error_code, const std::string &page) {
	_error_pages[error_code] = page;
}

void	ServerConfig::setClientMaxBodySize(int client_max_body_size) {
	_client_max_body_size = client_max_body_size;
}

void	ServerConfig::addRoute(const Route &route) {
	_routes.push_back(route);
}

void	ServerConfig::setRootConfig(const std::string &root_config) {
	_root_config = root_config;
}

void	ServerConfig::skipSpace(std::string::iterator &it, const std::string::iterator &end) {
	while (it != end && std::isspace(*it)) {
		++it;
	}
}

std::string	ServerConfig::parseString(std::string::iterator &it, const std::string::iterator &end) {
	if (it == end || *it != '\"') {
		throw std::runtime_error("syntax error: '\"' expected for a string");
	}
	++it;
	std::string	result;
	while (it != end && *it != '\"') {
		result.push_back(*it);
		++it;
	}
	if (result.empty()) {
		throw std::runtime_error("syntax error: expected string");
	}
	if (it == end || *it != '\"') {
		throw std::runtime_error("syntax error: '\"' expected for a string");
	}
	++it;
	return (result);
}

int	ServerConfig::stringPositiveInt(const std::string &str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!isdigit(*it)) {
			throw std::runtime_error("syntax error: expected integer for an error page");
		}
	}
	return (std::atoi(str.c_str()));
}

std::size_t	ServerConfig::parseInteger(std::string::iterator &it, const std::string::iterator &end) {
	std::string	num_str;

	while (it != end && std::isdigit(*it)) {
		num_str.push_back(*it);
		++it;
	}
	if (num_str.empty()) {
		throw std::runtime_error("syntax error: expected integer");
	}
	return (std::atol(num_str.c_str()));
}

std::vector<int>	ServerConfig::parseArray(std::string::iterator &it, const std::string::iterator &end) {
	std::vector<int>	result;

	if (it == end) {
		throw std::runtime_error("syntax error: unexpected end of file");
	}
	if (*it == '[') {
		++it;
		while (true) {
			skipSpace(it, end);
			if (it != end && *it == ']') {
				++it;
				break ;
			}
			result.push_back(parseInteger(it, end));
			skipSpace(it, end);
			if (it != end && *it == ',') {
				++it;
				skipSpace(it, end);
				if (it == end || !std::isdigit(*it)) {
					throw std::runtime_error("syntax error: new expected value");
				}
			} else if (it == end || *it != ']') {
				throw std::runtime_error("syntax error: ',' or ']' expected for an array");
			}
		}
	} else {
		result.push_back(parseInteger(it, end));
	}
	if (result.empty()) {
        throw std::runtime_error("syntax error: at least one integer expected");
    }
	return (result);
}

std::map<int, std::string>	ServerConfig::parseMap(std::string::iterator &it, const std::string::iterator &end) {
	std::map<int, std::string>	result;

	skipSpace(it, end);
	if (it == end || *it != '{') {
		throw std::runtime_error("syntax error: '{' expected for a map");
	}
	++it;
	while (true) {
		skipSpace(it, end);
		if (it != end && *it == '}') {
			++it;
			break ;
		}

		std::string	key_str = parseString(it, end);
		int	key = stringPositiveInt(key_str);

		if (it == end || *it != ':') {
			throw std::runtime_error("syntax error: ':' expected after the error_pages name");
		}
		++it;
		skipSpace(it, end);

		std::string	value = parseString(it, end);

		if (result.find(key) != result.end()) {
			throw std::runtime_error("error: duplicate key detected for key " + key_str);
		}
		result[key] = value;
		skipSpace(it, end);
		if (it != end && *it == ',') {
			++it;
			skipSpace(it, end);
			if (it == end || *it != '\"') {
				throw std::runtime_error("syntax error: new expected value");
			}
		} else if (it == end || *it != '}') {
			throw std::runtime_error("syntax error: ',' or '}' expected for a map");
		}
	}
	return (result);
}

std::vector<Route>	ServerConfig::parseRoutes(std::string::iterator &it, const std::string::iterator &end) {
	std::vector<Route>	result;

	if (it == end || *it != '[') {
		throw std::runtime_error("syntax error: '[' expected for a Route object");
	}
	++it;
	while (true) {
		skipSpace(it, end);
		if (it != end && *it == ']') {
			++it;
			break;
		}
		if (it == end || *it != '{') {
			throw std::runtime_error("syntax error: '{' expected for a Route object");
		}
		++it;
		skipSpace(it, end);

		Route route;

		route.parseRoute(it, end);
		result.push_back(route);
		skipSpace(it, end);
		if (it != end && *it == ',') {
			++it;
			skipSpace(it, end);
            if (it == end || *it != '{') {
                throw std::runtime_error("syntax error: new expected value");
            }
		} else if (it == end || *it != ']') {
			throw std::runtime_error("syntax error: ',' or ']' expected for a Route object");
		}
	}
	return (result);
}

std::string   ServerConfig::getPathsRedirection(const std::string &route_name) const {
    std::vector<Route>::const_iterator  it;
    for (it = _routes.begin(); it != _routes.end(); ++it) {
        if (it->getPath() == route_name) {
            return (it->getRoot());
        }
    }
    static const std::string    emptyString;
    return (emptyString);
}
const std::string   &ServerConfig::getErrorPageNumber(int error_number) const {
    std::map<int, std::string>::const_iterator  it = _error_pages.find(error_number);
    if (it != _error_pages.end()) {
        return (it->second);
    } else {
        static const std::string    emptyString;
        return (emptyString);
    }
}

const std::string   &ServerConfig::getRootConfig() const {
    return (_root_config);
}
