/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:28:14 by cjunker           #+#    #+#             */
/*   Updated: 2023/06/13 11:17:11 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"


Server::Server(const std::vector<ServerConfig> &config){
	std::vector<ServerConfig>::const_iterator	it;

	for (it = config.begin(); it != config.end(); ++it) {
		std::vector<int>	ports = it->getPorts();
		std::vector<int>::const_iterator	jt;

		std::cout << PINK << "Configuration File [" << RESET << (*it).getNameConfigFile() << PINK << "]" << RESET << std::endl;

		for (jt = ports.begin(); jt != ports.end(); ++jt) {
			
			std::cout << VIOLET << "Server [" << RESET << it->getServerName() << VIOLET << "] is listening on the port : " << RESET << *jt  << std::endl;
			int	fd = setupSocket(*jt);
			_server_fds.push_back(fd);
			_config[fd] = *it;
		}
	}
}

int	Server::setupSocket(int port) {
	int 				server_fd;
	struct sockaddr_in	address;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		throw std::runtime_error("In socket");
	}

	int on = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1) {
		throw std::runtime_error("setsockopt failed");
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(port);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		throw std::runtime_error("In bind");
	}
	if (listen(server_fd, 1024) < 0) {
		throw std::runtime_error("In listen");
	}
	return (server_fd);
}

void Server::start_serv() {
   	if ((kq = kqueue()) == -1) {
		throw std::runtime_error("kqueue failed");
	}

	std::vector<int>::iterator	it;
	for (it = _server_fds.begin(); it != _server_fds.end(); ++it) {

		if (apply_kevent(*it, EVFILT_READ, EV_ADD | EV_ENABLE, 0, NULL) == -1){
			throw std::runtime_error("in kevent");
		}
	}
	
	while (1) {
		int	nbEvents;

		if ((nbEvents = kevent(kq, NULL, 0, evList, MAX_EVENTS, NULL)) == -1) {
			break;
		}
		for (int i = 0; i < nbEvents; ++i) {
			try {
				struct kevent &evt = evList[i];

				if (evt.flags & EV_ERROR) {
					continue;                   
				}
				if (std::find(_server_fds.begin(), _server_fds.end(), evt.ident) != _server_fds.end()) {
					newConnection(evt);
				}
				else {
					if (map_sock.find(evt.ident) != map_sock.end()) {
						processRequest(evt, map_sock[evt.ident].getConfigClient());
					}
					if (evt.udata) {
						char buffer1[10000];
						ssize_t valread1 = 0;
						valread1 = read(evt.ident, buffer1, 10000);
							buffer1[valread1] = '\0';
							std::string tp(buffer1);
							map_sock[(uint64_t)evt.udata].resp.setContent(tp);
							map_sock[(uint64_t)evt.udata].resp.setResponse(200, "", ".html");
							close(evt.ident);
							int	status;
							kill(map_sock[(uint64_t)evt.udata].resp.getPID(), SIGKILL);
							waitpid(map_sock[(uint64_t)evt.udata].resp.getPID(),  &status, 0);
							if (WEXITSTATUS(status)) {
								std::string tp = "";
								reqResponse(500, tp, tp,  map_sock[(uint64_t)evt.udata].getConfigClient(), evt, (uint64_t)evt.udata);

							}
							else
								sendResponseCgi(map_sock[(uint64_t)evt.udata].resp, evt);
							map_sock.erase((uint64_t)evt.udata);
							close((uint64_t)evt.udata);
					}
				}
			}
			catch (const std::exception &e) {
					std::cout << RED << "Internal Server Error : " << RESET << e.what() << std::endl;
					continue ;
			}
		}
	}
}

void	Server::newConnection(struct kevent &evt) {
	int	server_fd = evt.ident;
	int client_fd = accept(server_fd, NULL, NULL);

	if (client_fd == -1) {
		return;
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	if (apply_kevent(client_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0,NULL) == -1)
		throw std::runtime_error("in kevent");

	if (apply_kevent(client_fd,  EVFILT_TIMER, EV_ADD | EV_ONESHOT, TIME_OUT,NULL) == -1)
		throw std::runtime_error("in kevent");

	Client new_client(_config[server_fd]);

	map_sock.insert(std::make_pair(client_fd, new_client));
}

void	Server::disconnectClient(struct kevent &evt, int fd) {
	(void)evt;
	if (map_sock.count(fd)) {
		close(fd);
		map_sock.erase(fd);
	}
}

void Server::sendResponseCgi(Response &resp, struct kevent &evt) {

    std::string tp;
	tp = resp.getHeader() + RN + resp.getContent() + RN;
	ssize_t ret = send((uint64_t)evt.udata, tp.c_str(), tp.size(), 0);
    if (ret <= 0) {
		throw std::runtime_error("error in send");
    }
}


void	Server::sendResponse(Response &resp, struct kevent &evt, int fd) {

	map_sock[fd].remaining_data = resp.getHeader() + RN + resp.getContent() + RN;
	if (map_sock[fd].remaining_data.empty()) {
		map_sock[fd].remaining_data = resp.getHeader() + RN + resp.getContent() + RN;
	}
	size_t remaining = map_sock[fd].remaining_data.length() - map_sock[fd].sent_bytes;

	ssize_t sent_size = send(fd, map_sock[fd].remaining_data.c_str() + map_sock[fd].sent_bytes, remaining, 0);
	if (sent_size <= 0) {
		disconnectClient(evt, fd);
		throw std::runtime_error("error in send");
	}
	map_sock[fd].sent_bytes += sent_size;
	if (map_sock[fd].sent_bytes >= map_sock[fd].remaining_data.size()) {
		disconnectClient(evt, fd);
	}
}

void Server::processRequest(struct kevent &evt,  const ServerConfig &config) {
	char buffer[10000];
	ssize_t valread = 0;

	if (map_sock.count(evt.ident) && evt.udata == 0 && evt.flags & EV_EOF) {
		if (map_sock[evt.ident].resp.getEND0() && map_sock[evt.ident].resp.getPID() > 0) {
			kill(map_sock[evt.ident].resp.getPID(), SIGKILL);
			waitpid(map_sock[evt.ident].resp.getPID(), NULL, 0);
			close(map_sock[evt.ident].resp.getEND0());
		}
		disconnectClient(evt, evt.ident);
		return;
	}
	else if ((map_sock.count(evt.ident) != 0 && evt.flags & EV_CLEAR && evt.filter == EVFILT_TIMER)) {
		std::string test = "";
		if (map_sock[evt.ident].resp.getEND0() > 0 && map_sock[evt.ident].resp.getPID() > 0) {
			kill(map_sock[evt.ident].resp.getPID(), SIGKILL);
			waitpid(map_sock[evt.ident].resp.getPID(), NULL, 0);
			close(map_sock[evt.ident].resp.getEND0());
		}
		reqResponse(408, test, test, config, evt, evt.ident);
		disconnectClient(evt, evt.ident);
	    return ;
	}
	else if (map_sock.count(evt.ident) != 0 && evt.filter == EVFILT_READ && map_sock[evt.ident].response == false) {
		valread = recv(evt.ident, buffer, sizeof(buffer), 0);
		if (valread <= 0) {
			disconnectClient(evt, evt.ident);
			return;
		}
		if  (!map_sock[evt.ident].is_body) {
			map_sock[evt.ident].request.append(buffer, valread);
			std::string::size_type separator_pos = map_sock[evt.ident].request.find("\r\n\r\n");

			if (separator_pos != std::string::npos) {
				std::size_t content_length_pos = map_sock[evt.ident].request.find("Content-Length: ");
				if (content_length_pos != std::string::npos)
				{
					content_length_pos += strlen("Content-Length: ");
					std::size_t end_pos = map_sock[evt.ident].request.find("\r\n", content_length_pos);
					if (end_pos != std::string::npos)
					{
						map_sock[evt.ident].is_body = true;
						std::string content_length_str = map_sock[evt.ident].request.substr(content_length_pos, end_pos - content_length_pos);
						map_sock[evt.ident].content_length = std::atoi(content_length_str.c_str());
						if (config.getClientMaxBodySize() <= map_sock[evt.ident].content_length)
						{
							std::string test = "";
							 reqResponse(413, test, test, config, evt, evt.ident);
							 return;
						}
						std::size_t body_start = separator_pos + 4;
						
						map_sock[evt.ident].appendToBody(buffer + body_start, valread - body_start);
						if (map_sock[evt.ident].getBody().size() >= map_sock[evt.ident].content_length)
                            map_sock[evt.ident].all_body = true;				
					}
				}
			}
		} 
		else {
			map_sock[evt.ident].appendToBody(buffer, valread);
			if (map_sock[evt.ident].getBody().size() >= map_sock[evt.ident].content_length)
				map_sock[evt.ident].all_body = true;
		}
		
		if (!map_sock[evt.ident].all_body && map_sock[evt.ident].is_body)
			return;
		if (!map_sock[evt.ident].is_body && map_sock[evt.ident].request.find("\r\n\r\n") == std::string::npos)
			return;
		map_sock[evt.ident].response = true;
		if (apply_kevent(evt.ident, EVFILT_WRITE, EV_ADD, 0, NULL) == -1) {
			throw std::runtime_error("in kevent");
		}

	}
	else if (map_sock.count(evt.ident) != 0 && evt.filter == EVFILT_WRITE && map_sock[evt.ident].response == true) {
		std::string body_str(map_sock[evt.ident].getBody().begin(), map_sock[evt.ident].getBody().end());
		std::string test = map_sock[evt.ident].request;
        reqResponse(0, test, body_str, config, evt, evt.ident);
	}
}

int		Server::apply_kevent(int fd, int filtre1, int filtre2, int time,void *udata) {
	struct kevent  evt;
	EV_SET(&evt, fd, filtre1, filtre2, 0, time, udata);
	return (kevent(kq, &evt, 1, NULL, 0, NULL));
}

void Server::reqResponse(int type, std::string &header, std::string &body, const ServerConfig &config, struct kevent &evt, int fd)
{
	if (type) {
        RequestMethod req(header, body, config);
        Response resp(type, req);
        resp.initResponse();
        sendResponse(resp, evt, fd);
    }
	else {
		RequestMethod req(header, body, config);
		int error;
		error = req.initRequest();
		if (error) {
			type = error;
		}
		Response resp(type, req);
		resp.initResponse();

		if (resp.getEND0())
		{	
			if (apply_kevent(resp.getEND0(), EVFILT_READ, EV_ADD, 0,(void*)evt.ident) == -1 || apply_kevent(evt.ident, EVFILT_WRITE, EV_DELETE, 0, NULL) == -1)
				throw std::runtime_error("in kevent");
			map_sock[evt.ident].resp = resp;
			return;
		}
		sendResponse(resp, evt, fd);
	}
}
