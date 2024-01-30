/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:27:13 by cjunker           #+#    #+#             */
/*   Updated: 2023/06/09 12:33:09 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "webserv.hpp"
#include "Client.hpp"
#include "../http/RequestMethod.hpp"
#include "../http/Response.hpp"

class Server
{
    private:
        Server(void){};

        std::vector<int> 			_server_fds;
        std::map<int, ServerConfig> _config;
        int							kq;
        struct kevent 				evList[MAX_EVENTS];
        std::map<int, Client> 		map_sock;

    public:
        Server(const std::vector<ServerConfig> &config);
        ~Server(void) {
			std::vector<int>::iterator	it;

			for (it = _server_fds.begin(); it != _server_fds.end(); ++it) {
				close(*it);
			}
			close(kq);
        };

		int		setupSocket(int port);
		void	newConnection(struct kevent &evt);
        void	disconnectClient(struct kevent &evt, int fd);
		void	sendResponse(Response &resp, struct kevent &evt, int fd);
		void	processRequest(struct kevent &evt, const ServerConfig &config);
		void	start_serv();
        int 	apply_kevent(int fd, int filtre1, int filtre2, int time, void *udata);
        void	sendResponseCgi(Response &resp, struct kevent &evt);
        void    reqResponse(int type, std::string &header, std::string &body, const ServerConfig &config, struct kevent &evt, int fd);
};