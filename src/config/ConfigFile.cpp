/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:47:42 by alevasse          #+#    #+#             */
/*   Updated: 2023/06/13 08:48:55 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"
#include "ServerConfig.hpp"

ConfigFile::ConfigFile(void) {}

void	ConfigFile::initDefault() {
	ServerConfig	server;

	server.setHost("localhost");
	server.addPort(8081);
	server.setServerName("webserv-bands.com");
	server.setRootConfig("www");
	server.addErrorPage(400, "/error_pages/400.html");
	server.addErrorPage(403, "/error_pages/403.html");
	server.addErrorPage(404, "/error_pages/404.html");
	server.addErrorPage(500, "/error_pages/500.html");
	server.setClientMaxBodySize(1048576);
	server.setConfigFileName(getName());

	Route route1;
	route1.setPath("/");
	route1.setRoot("www");
	route1.setDefaultFile("index.html");
	route1.setDirectoryListing(true);
	route1.addAllowedMethod("GET");
	route1.addAllowedMethod("POST");
	route1.addAllowedMethod("HEAD");
	std::vector<std::string> cgi_extension;
	cgi_extension.push_back(".py");
	cgi_extension.push_back(".pl");
	cgi_extension.push_back(".php");
	std::vector<std::string> cgi_path;
	cgi_path.push_back("/cgi-bin");
	route1.addCgi("extension", cgi_extension);
	route1.addCgi("cgi_path", cgi_path);
	route1.addUpload("enabled", "true");
	route1.addUpload("upload_directory", "/upload");

	Route route2;
	route2.setPath("/upload");
	route2.setRoot("www/upload");
	route2.setDefaultFile("delete.html");
	route2.setDirectoryListing(false);
	route2.addAllowedMethod("GET");
	route2.addAllowedMethod("DELETE");
	route2.addUpload("enabled", "true");
	route2.addUpload("upload_directory", "/upload");

	Route route3;
	route3.setPath("/error_pages");
	route3.setRoot("/error_pages");
	route3.setDirectoryListing(false);
	route3.addAllowedMethod("GET");

	Route route4;
	route4.setPath("/css");
	route4.setDirectoryListing(false);
	route4.addAllowedMethod("GET");

	Route route5;
	route5.setPath("/www");
	route5.setDirectoryListing(false);
	route5.addAllowedMethod("GET");

	Route route6;
	route6.setPath("/cgi_path");
	route6.setDirectoryListing(false);
	route6.addAllowedMethod("GET");

	Route route7;
	route7.setPath("/js");
	route7.setDirectoryListing(false);
	route7.addAllowedMethod("GET");

	Route route8;
	route8.setPath("/js/vendor");
	route8.setDirectoryListing(false);
	route8.addAllowedMethod("GET");

	Route route9;
	route9.setPath("/fonts");
	route9.setDirectoryListing(false);
	route9.addAllowedMethod("GET");

	Route route10;
	route10.setPath("/img");
	route10.setDirectoryListing(true);
	route10.addAllowedMethod("GET");
	
	Route route11;
    route11.setPath("/flemaitr");
    route11.setRedirect("https://www.linkedin.com/in/fanny-lemaitre/");
    route11.addAllowedMethod("GET");

    Route route12;
    route12.setPath("/cjunker");
    route12.setRedirect("https://www.linkedin.com/in/celine-junker-6b7870106/");
    route12.addAllowedMethod("GET");

    Route route13;
    route13.setPath("/alevasse");
    route13.setRedirect("https://www.linkedin.com/in/anthony-levasseur/");
    route13.addAllowedMethod("GET");
	
	Route route14;
	route14.setPath("/mp3");
	route14.setDirectoryListing(false);
	route14.addAllowedMethod("GET");

	server.addRoute(route1);
	server.addRoute(route2);
	server.addRoute(route3);
	server.addRoute(route4);
	server.addRoute(route5);
	server.addRoute(route6);
	server.addRoute(route7);
	server.addRoute(route8);
	server.addRoute(route9);
	server.addRoute(route10);
	server.addRoute(route11);
	server.addRoute(route12);
	server.addRoute(route13);
	server.addRoute(route14);

	_servers.push_back(server);
}

void	ConfigFile::init(const std::string &file_name) {
	
	std::ifstream		config_file(file_name.c_str());
	
	DIR *dir = opendir(file_name.c_str());
	if (dir) {
		closedir(dir);
		throw std::runtime_error("Error with the file opening <" + file_name + "> is a directory");
	}
	if (!config_file.is_open()) {
		throw std::runtime_error("Error with the file opening <" + file_name + ">");
	}
	
	std::stringstream		buffer;

	buffer << config_file.rdbuf();
	std::string				config_json_str = buffer.str();
	std::string::iterator	it = config_json_str.begin();
	std::string::iterator	end = config_json_str.end();

	parseConfig(it, end);
}

std::vector<ServerConfig>	ConfigFile::getServers() const {
	return (_servers);
}

const std::string			&ConfigFile::getName() const {
	return (_file_name);
}

void						ConfigFile::setName(const std::string &name) {
	_file_name = name;
}

void	ConfigFile::parseConfig(std::string::iterator &it, const std::string::iterator &end) {
	skipSpace(it, end);
	if (it == end || *it != '{') {
		throw std::runtime_error("syntax error: '{' expected at start of file");
	}
	++it;

	std::string keyword = "servers";

	skipSpace(it, end);
	if (it == end || *it != '"') {
		throw std::runtime_error("syntax error: the keyword must be enclosed in quotes");
	}
	++it;
	for (size_t j(0); j < keyword.size(); j++) {
		if (it == end || *it != keyword[j]) {
			throw std::runtime_error("syntax error: the keyword 'servers' is expected");
		}
		++it;
	}
	if (it == end || *it != '"') {
		throw std::runtime_error("syntax error: the keyword must be enclosed in quotes");
	}
	++it;
	if (it == end || *it != ':') {
		throw std::runtime_error("syntax error: ':' expected after a keyword");
	}
	++it;
		skipSpace(it, end);
	if (it == end && *it != '[') {
		throw std::runtime_error("syntax error: '[' expected to start server list");
	}
	++it;
	while (true) {
		skipSpace(it, end);
		if (it == end || *it != '{') {
			throw std::runtime_error("syntax error: '{' expected to create a ServerConfig object");
		}
		++it;

		ServerConfig	server;

		server.parseServerConfig(it, end);
//
		server.setConfigFileName(getName());
		_servers.push_back(server);
		if (it != end && *it == ',') {
			++it;
			continue ;
		} else {
			break ;
		}
	}
	skipSpace(it, end);
	if (it == end || *it != ']') {
		throw std::runtime_error("syntax error: ']' expected to finish servers list");
	}
	++it;
	skipSpace(it, end);
	if (it == end || *it != '}') {
		throw std::runtime_error("syntax error: '}' expected at end of file");
	}
	++it;
	skipSpace(it, end);
	if (it != end) {
		throw std::runtime_error("syntax error: unexpected character at end of file");
	}
}

void	ConfigFile::skipSpace(std::string::iterator &it, const std::string::iterator &end) {
	while (it != end && std::isspace(*it)) {
		++it;
	}
}
