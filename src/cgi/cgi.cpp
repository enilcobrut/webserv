/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:24:19 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/12 15:37:34 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "../http/Response.hpp"

std::size_t Response::sizeTab(char **tab)
{
    std::size_t size = 0;
    while (tab[size])
        size++;
    return size;
}

std::string	Response::getServerName(void) const {
	std::string host_port = _req.getHost();
	size_t		pos = host_port.find(':');

	if (pos != std::string::npos) {
		return (host_port.substr(0, pos));
	} else {
		return ("");
	}
}

std::string Response::getPortNumber(void) const {
	std::string host_port = _req.getHost();
	size_t		pos = host_port.find(':');

	if (pos != std::string::npos) {
		return (host_port.substr(pos + 1));
	} else {
		return ("");
	}
}

std::string	Response::getMethod(void) const {
	if (_req.getType() == GET) {
		return ("GET");
	} else if (_req.getType() == POST) {
		return ("POST");
	} else if (_req.getType() == DELETE) {
		return ("DELETE");
	} else if (_req.getType() == HEAD) {
		return ("HEAD");
	}
	return ("");
}

std::string	Response::getQueryString(const std::string &uri) const {
	std::size_t	start_pos = uri.find('?');

	if (start_pos == std::string::npos) {
		return "";
	} else {
		std::string	query_string = uri.substr(start_pos + 1);
		return (decodeUrl(query_string));
	}
}

char **Response::getEnv()
{
	std::vector<std::string>	tmp;
	char						**envp = *g_envPtr;

	for (int i = 0; envp[i]; i++) {
		tmp.push_back(envp[i]);
	}
	addEnvVariable(&tmp, "AUTH_TYPE", _req.getHost());
	addEnvVariable(&tmp, "DOCUMENT_ROOT", _req.getConf().getRootConfig() + "/");
	addEnvVariable(&tmp, "HTTP_HOST", getServerName());
	addEnvVariable(&tmp, "CONTENT_LENGTH", std::to_string(_content.size()));
	addEnvVariable(&tmp, "CONTENT_TYPE", _content_type);
	std::string allCookies;
	std::map<std::string, std::string>::const_iterator itm = _req.getCookie().cbegin();
	std::map<std::string, std::string>::const_iterator itme = _req.getCookie().cend();
	while (itm != itme)
	{
		allCookies += (*itm).first + "=" + (*itm).second;
		itm++;
		if (itm != itme)
		 allCookies += "; ";
	}
	addEnvVariable(&tmp, "HTTP_COOKIE", allCookies);
	addEnvVariable(&tmp, "HTTP_REQUEST_METHOD", getMethod());
	addEnvVariable(&tmp, "HTTP_USER_AGENT", _req.getUserAgent());
	addEnvVariable(&tmp, "PATH", "/usr/local/bin:/usr/bin");
	addEnvVariable(&tmp, "QUERY_STRING", getQueryString(_req.getUri()));
	addEnvVariable(&tmp, "SERVER_PORT", getPortNumber());
	addEnvVariable(&tmp, "SERVER_PROTOCOL", "HTTP/1.1");

	char								**ret;
	int									i;
	std::vector<std::string>::iterator	it;

	ret = (char **)calloc(tmp.size() + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	for (it = tmp.begin(); it < tmp.end(); ++it) {
		ret[i] = strdup((*it).c_str());
		i++;
	}
	ret[i] = NULL;
    return (ret);
}

void	Response::addEnvVariable(std::vector<std::string> *tmp, const std::string &name, const std:: string &value) {
	if (value.size()) {
		tmp->push_back(name + "=" + value);
	}
}

void Response::commonGatewayInterface()
{
	 std::string command;
    if (_extension == ".php")
        command = "/usr/bin/php";
    else if (_extension == ".pl")
        command = "/usr/bin/perl";
    else if (_extension == ".py")
        command = "/usr/local/bin/python3";
    else if (_extension == ".js")
        command = "/usr/local/bin/node";
    else
        throw std::runtime_error("403");
	if (!isValidFile(_path_clear + _file_name))
		throw std::runtime_error("404");
    if (pipe(_end))
        throw std::runtime_error("500");
		
    _pid = fork();
    if (_pid < 0)
        throw std::runtime_error("500");
    if (!_pid)
    {
		if (_req.getType() == POST && _content.size())
		{
			int input_pipe[2];
			if (pipe(input_pipe))
				throw std::runtime_error("500");
			write(input_pipe[1], _content.c_str(), _content.length());
			close(input_pipe[1]);
			dup2(input_pipe[0], 0);
			close(input_pipe[0]);
		}

        close(_end[0]);
        char **env = getEnv();
        if (env == NULL)
            exit(EXIT_FAILURE);
        if (dup2(_end[1], 1) == -1)
            exit(EXIT_FAILURE);
        char **arg = (char **)malloc(sizeof(char *) * 3);
        if (!arg)
            exit(EXIT_FAILURE);
        arg[0] = strdup(command.c_str());
        arg[1] = strdup((_path_clear + _file_name).c_str());
        arg[2] = NULL;
        execve(arg[0], arg, env);
        exit(EXIT_FAILURE);
    }
    else
    {        
        close(_end[1]);
		fcntl(_end[0], F_SETFL, O_NONBLOCK);
    }
};
