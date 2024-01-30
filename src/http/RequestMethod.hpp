/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestMethod.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:25:46 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/12 15:43:47 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "../config/ServerConfig.hpp"

class RequestMethod
{
private:
	int							_type;
	std::string					_http;
	std::string					_user_agent;
	std::string					_uri;
	std::string					_host;
	std::string					_reqbody;
	ServerConfig				_config;
	std::string					_reqheader;
	size_t						_reqContentSize;
	std::map<std::string, std::string> _cookie;

public:
	RequestMethod(void){};
	RequestMethod(std::string header, std::string body, ServerConfig config);
	~RequestMethod(void){};

	int							getInfos(const std::vector<std::string> &vec);
	int							getHeaderRequest(const std::vector<std::string> &vec);
	int							initRequest(void);
	int							setLine(std::string &line);
	int							getCookieInfos(std::string line);
	int							getHostInfos(const std::vector<std::string> &vec);
	int							getUserAgentInfos(const std::vector<std::string> &vec);
	int							getContentLengthReq(const std::vector<std::string> &vec);
	const std::string			&getReqBody(void) { return _reqbody; }
	int							getType(void) const { return _type; };
	std::string					getHTTP(void) const { return _http; };
	const std::string			&getUri(void) const { return _uri; };
	size_t						getContentSize(void) const { return _reqContentSize; };
	const std::string			getCookieSession(void) { 
		if (_cookie.count("sessionID"))
			return _cookie["sessionID"];
		return ""; };
	const std::map<std::string, std::string> &getCookie(void) const { return _cookie; };
	const std::string			&getUserAgent(void) { return _user_agent; }
	void						setCookie(const std::string &cookie) { _cookie["sessionID"] = cookie; };
	const ServerConfig			&getConf(void) const { return _config; };
	const std::string			&getHost(void) const { return _host; };
	void						setHTTP(const std::string &http) { _http = http; };
	std::string                 checkRedirect(const std::string& path);
	std::string                 _redirectUrl;
	const std::string           &getRedirectUrl() const { return _redirectUrl; };
};
