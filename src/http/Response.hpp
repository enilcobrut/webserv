/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:26:00 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/09 12:26:01 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "RequestMethod.hpp"
#include "Mimes.hpp"
#include "ErrorStatus.hpp"

extern std::vector<std::string>	g_sessions;

class RequestMethod;
class Mime;

class Response
{

private:
	std::string			_extension;
	std::string			_post_extension;
	std::string			_path_clear;
	std::string			_path_upload;
	std::string			_file_name;
	int					_status_code;
	std::string			_content_type;
	std::string			_content;
	std::string			_header;
	Mime				_mimeMap;
	ErrorStatus			_errorMap;
	RequestMethod		_req;
	const Route			*_route;
	int					_type;
	int					_pid;
	int					_end[2];
	int					_error;

public:
	const std::string	&getContent(void) { return _content; }
	const std::string	&getExt(void) { return _extension; }
	const std::string	&getHeader(void) { return _header; }
	const std::string	&getFilename(void) { return _file_name; }
	int					getPID(void) { return _pid; }
	int					getEND0(void) { return _end[0]; }
	int					getEND1(void) { return _end[1]; }
	void				setContent(const std::string &newContent) { _content = newContent; }
	const std::string	&getPathUpload(void) { return _path_upload; }
	const RequestMethod &getReq(void) { return _req; };
//--------------------------------------------------------------------//
//							 	cgi.cpp		  						  //
//--------------------------------------------------------------------//
	std::size_t			sizeTab(char **tab);
	char				**getEnv(void);
	void				commonGatewayInterface(void);
	std::string			getServerName(void) const;
	std::string 		getPortNumber(void) const;
	std::string			getMethod(void) const;
	void				addEnvVariable(std::vector<std::string> *tmp, const std::string &name, const std:: string &value);
	std::string			getQueryString(const std::string &uri) const;

//--------------------------------------------------------------------//
//						      cookie.cpp	  						  //
//--------------------------------------------------------------------//
	std::string			generateSessionID(void);
	bool				cookieIsTaken(void);
	void				getValueCookieCgi(void);

//--------------------------------------------------------------------//
//					    		 html.cpp	  						  //
//--------------------------------------------------------------------//
	void				getDefaultPageHTML(int &error);
	void				getResponsePageHTML(int type, const std::string str1, const std::string str2);
	void				getDeleteHTML(const char *path);
	void				getDirHTML(void);
	const std::vector <std::string>	getAutoIndex(const char *path);

//--------------------------------------------------------------------//
//					    	 ResponseTools.cpp	  					  //
//--------------------------------------------------------------------//
	bool				isAllowedExtension(const std::string &extension);
	void				setExtension(const std::string &str);
	void				checkSecurityPath(std::string path);
	void				multiBackslash(std::string &string);
	int					setRoute(std::string path);
	void				setPath(void);
	void				cleanPath(std::string &str);
	void				fileToBinary(const std::string &path_file);
	void				readMap(void);

//--------------------------------------------------------------------//
//							 Response.cpp	  						  //
//--------------------------------------------------------------------//
	Response(int type, RequestMethod &req);
	Response& operator=(const Response& other);
	Response()
	{
		_status_code = 0;
		_end[0] = 0;
		_end[1] = 0;
		_pid = 0;
	}
	~Response(void){};
	void				initResponse(void);
	void				isFile(void);
	void				isCGI(void);
	void				isDirectory(void);
	void				requestGet(void);
	void				requestHead(void);
	void				requestPost(void);
	void				requestDelete(void);
	int					setResponse(int status, const std::string &path,
	const std::string &extension);
	int					setErrorResponse(int error);
	void				setBodyContentPost();
};
