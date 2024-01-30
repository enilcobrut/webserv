/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:47:17 by alevasse          #+#    #+#             */
/*   Updated: 2023/06/13 08:58:22 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "config/ConfigFile.hpp"
#include "http/RequestMethod.hpp"
#include "http/Response.hpp"
#include "server/Server.hpp"

char*** g_envPtr = nullptr;

int main(int argc, char **argv, char **envp) {
	try {
		g_envPtr = &envp;
		srand(time(0));
		signal(SIGPIPE, SIG_IGN);
		if (argc > 2)
			throw std::runtime_error("./webserv [configuration file]");
		ConfigFile config_file;
		if (argc == 1) {
			config_file.setName("default");
			config_file.initDefault();
		}
		if (argc == 2) {
			config_file.setName(argv[1]);
			config_file.init(argv[1]);
		}
		
		std::vector<ServerConfig> servers = config_file.getServers();
				
		Server serv(servers);
		serv.start_serv();
	}
	catch (const std::exception &e) {
		std::cout << e.what() << '\n'; 
	}
	return 0;
}