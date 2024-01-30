/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:47:58 by alevasse          #+#    #+#             */
/*   Updated: 2023/06/13 08:29:10 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Route.hpp"

class ServerConfig {
	private:
		std::string					_host;
		std::vector<int>			_ports;
		std::string					_server_name;
		std::map<int, std::string>	_error_pages;
		std::size_t					_client_max_body_size;
		std::vector<Route>			_routes;
		std::string					_root_config;
		std::string					_config_file_name;

	public:
		ServerConfig();

		const std::string					&getHost() const;
		std::vector<int>					getPorts() const;
		const std::string					&getServerName() const;
		const std::string					&getConfigName() const;
		const std::map<int, std::string>	getErrorPages() const;
		std::size_t							getClientMaxBodySize() const;
		const Route			&getRoutes(const std::string &route_name) const;
		
		void	setHost(const std::string &host);
		void	addPort(int port);
		void	setServerName(const std::string &server_name);
		void	addErrorPage(int error_code, const std::string &page);
		void	setClientMaxBodySize(int client_max_body_size);
		void	addRoute(const Route &route);
		void	setRootConfig(const std::string &root_config);
		void	setConfigFileName(const std::string &name);
		const std::string &getNameConfigFile(void) const;

		void	parseServerConfig(std::string::iterator &it, const std::string::iterator &end);
		static void	skipSpace(std::string::iterator &it, const std::string::iterator &end);
		static std::string	parseString(std::string::iterator &it, const std::string::iterator &end);
		int	stringPositiveInt(const std::string &str);
		std::size_t	parseInteger(std::string::iterator &it, const std::string::iterator &end);
		std::vector<int>	parseArray(std::string::iterator &it, const std::string::iterator &end);
		std::map<int, std::string>	parseMap(std::string::iterator &it, const std::string::iterator &end);
		std::vector<Route>	parseRoutes(std::string::iterator &it, const std::string::iterator &end);
		std::string   		getPathsRedirection(const std::string &route_name) const;
		const std::string   &getErrorPageNumber(int error_number) const;
		const std::string   &getRootConfig() const;
};