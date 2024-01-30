/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:47:47 by alevasse          #+#    #+#             */
/*   Updated: 2023/06/13 08:43:25 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "ServerConfig.hpp"

class ConfigFile
{
private:
	std::vector<ServerConfig>	_servers;
	std::string					_file_name;

public:
	ConfigFile();

	void	initDefault();
	void	init(const std::string &file_name);
	
	std::vector<ServerConfig>	getServers() const;
	void						setName(const std::string &name);
	const std::string			&getName() const;

	void	parseConfig(std::string::iterator &it, const std::string::iterator &end);
	void	skipSpace(std::string::iterator &it, const std::string::iterator &end);
};
