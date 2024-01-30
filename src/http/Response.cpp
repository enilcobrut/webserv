/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:25:55 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/13 11:46:49 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(int type, RequestMethod &req) : _status_code(200), _header(""),
	_req(req), _route(NULL), _error(type)
{
	_end[0] = 0;
	_end[1] = 0;
	_pid = 0;
}

Response& Response::operator=(const Response& other)
{
    if (this != &other)
    {
        _extension = other._extension;
        _post_extension = other._post_extension;
        _path_clear = other._path_clear;
        _file_name = other._file_name;
        _status_code = other._status_code;
        _content_type = other._content_type;
        _content = other._content;
        _header = other._header;
        _mimeMap = other._mimeMap;
        _errorMap = other._errorMap;
        _req = other._req;
        _route = other._route;
        _type = other._type;
		_pid = other._pid;
		_end[0] = other._end[0];
		_end[1] = other._end[1];
		_error = other._error;
     }
    return *this;
}

void Response::initResponse() {
	try {
		if (_error) {
			setErrorResponse(_error);
			_error = 0;
			return;
		}
		setPath();
		cookieIsTaken();
		if (_route && !_route->isAllowedMethods(_req.getType()) && _req.getType() != HEAD) {
			throw std::runtime_error("405");
		}
		switch(_req.getType()) {
  			case GET:
  				requestGet();
				break ;
  			case DELETE:
  				requestDelete();
				break ;
			case POST:
  				requestPost();
				break ;
			case HEAD:
				requestHead();
				break ;
  			default:
				throw std::runtime_error("400");
		}
	}
	catch (const std::exception &e) {
		int error = std::atoi(e.what());
		if (!error)
			setErrorResponse(500);
		else
			setErrorResponse(error);
	}
}

void Response::isDirectory(void)
{
	if (_route && _route->getDirectoryListing() == false) {
		throw std::runtime_error("403");
	}
	getDirHTML();
	setResponse(200, "", ".html");
}

void Response::isCGI(void)
{
	if (_route && _route->getCgiPath().size()) {
		_path_clear = _route->getCgiPath();
		cleanPath(_path_clear);
		_path_clear = _path_clear + "/";
		if (_req.getConf().getRootConfig().size())
		{
			std::string rootConfig = _req.getConf().getRootConfig();
			cleanPath(rootConfig);
			_path_clear = rootConfig + "/" + _path_clear;
		}
	}
	else
		throw std::runtime_error("403");
	commonGatewayInterface();
}

void Response::isFile(void)
{
    if (isAllowedExtension(_extension)) {
        if (_req.getUri() == "/upload") {
            getDeleteHTML((const char *)_req.getUri().c_str());
            setResponse(200, "", ".html");
        } 
        else
            setResponse(200, _path_clear + _file_name, "");
    }
    else {
        throw std::runtime_error("403");
    }
}

void Response::requestGet()
{
	if (!_req.getRedirectUrl().empty())
    	setResponse(301, "", ".html");
	else if (!_file_name.size())
		isDirectory();
	else if (((_route && _route->isAllowedCgi(_extension))))
		isCGI();
	else if (isValidFile(_path_clear))
		isFile();
	else
		throw std::runtime_error("404");
}

void Response::requestHead()
{
	std::ostringstream header;
	_status_code = 200;
	_extension = ".txt";
	_content_type = _mimeMap.getMime(_extension);
	header << "HTTP/" << _req.getHTTP() << " " << _status_code << RN;
	header << "Content-Type: " << _content_type << RN;
	header << "Content-Length: " << _content.size() << RN;
	header << "Flag: y0u_ju5t_f0und_" << RN;
	_header = header.str();
}

void Response::setBodyContentPost()
{
	std::string line;
    std::size_t pos = 0;
	std::string end_of_file;
	std::string reqBody = _req.getReqBody();
	pos = reqBody.find("\n", pos);
	if (pos != std::string::npos) {
		end_of_file = reqBody.substr(0, pos - 1);
		reqBody.erase(0, pos + 1);
	}
    while ((pos = reqBody.find("\n", pos)) != std::string::npos) {
    	line = reqBody.substr(0, pos);
		if (line.substr(0, 20) == "Content-Disposition:") {
			std::string::size_type pos = line.find("filename=");
			if (pos != std::string::npos) {
			    pos += 10;
			    std::string::size_type end = line.find('"', pos);
			    if (end != std::string::npos)
			        _file_name = line.substr(pos, end - pos);
				reqBody.erase(0, pos + _file_name.size() + 3);
			}
		}
		else if (line.substr(0, 13) == "Content-Type:") {
			std::string::size_type pos = 14;
			std::string::size_type end = line.find('\r', pos);
			_content_type = line.substr(pos, end - pos);
			reqBody.erase(0, pos + _content_type.size() + 4);
			break ;
		}
		else
			reqBody.erase(0, pos + 1);
    	pos++;
    }
	if (reqBody.size() && reqBody.substr(reqBody.size() - 4) == "\r\n\r\n")
		_content = reqBody.substr(0, reqBody.size() - end_of_file.size() - 4);
	else if (reqBody.size())
		_content = reqBody;
}

void Response::requestPost()
{	
	std::map<std::string, std::string> upload = _route->getUpload();
	if (!upload.size() || (upload.size() && upload["enabled"] != "true"))
		throw std::runtime_error("403");
	_path_upload = upload["upload_directory"];
	if (!_path_upload.size())
		throw std::runtime_error("403");
	setBodyContentPost();
	if (!_file_name.size())
		throw std::runtime_error("404");
	else if (((_route && _route->isAllowedCgi(_extension)))) {
		isCGI();
		setResponse(200, "", ".html");
	}
	else {
		std::ofstream outputFile(_req.getConf().getRootConfig() + _path_upload + "/" + _file_name, std::ios::out | std::ios::binary);
		if (!outputFile)
			throw std::runtime_error("500");
		outputFile.write(_content.c_str(), _content.size());
		if (!outputFile)
			throw std::runtime_error("500");
		outputFile.close();
		if (!outputFile)
			throw std::runtime_error("500");
		getResponsePageHTML(UPLOAD, _file_name, "");
		setResponse(201, "", ".html");
	}
}

void Response::requestDelete()
{
	std::ifstream flux(_path_clear + _file_name);
	if (flux)
		unlink((_path_clear + _file_name).c_str());
	else {
		throw std::runtime_error("404");
	}
	getResponsePageHTML(0, "The file has been deleted", _file_name);
	setResponse(200, "", ".html");
}

int Response::setResponse(int status, const std::string &path,
	const std::string &extension)
{
	std::ostringstream header;
	if (status)
		_status_code = status;
	if (extension.size()) {
		_extension = extension;
		_content_type = _mimeMap.getMime(_extension);
	}
	if (path.size())
		fileToBinary(path);
	if (!_content.size() && _status_code != 301)
		_status_code = 204;
	if (!_req.getHTTP().size())
		_req.setHTTP("1.1");
	header << "HTTP/" << _req.getHTTP() << " " << _status_code << " " <<_errorMap.getErrorStatus(_status_code) << RN;
	if (!_req.getRedirectUrl().empty()) {
        header << "Location: " << _req.getRedirectUrl() << RN;
    }
	else if (_req.getType() == POST && getPathUpload().size()) {
		header << "Location: " << getPathUpload() << RN;
	}
	if (_content.size() && _req.getType() != HEAD) {
		header << "Content-Type: " << _content_type << RN;
		header << "Content-Length: " << _content.size() << RN;
	}
	if (_extension == ".js" || _extension == ".css" || _extension == ".jpg" || _extension == ".png") {
		header << "Cache-Control: max-age=604800" << RN;
	}
	_header = header.str();
	getValueCookieCgi();
	return 1;
}

int Response::setErrorResponse(int error) {
	_status_code = error;
	_path_clear = _req.getConf().getErrorPageNumber(_status_code);
	if (!_path_clear.size()) {
		getDefaultPageHTML(_status_code);
		setResponse(0, "", ".html");
	}
	else {
		cleanPath(_path_clear);
		if (!isValidFile(_path_clear))
			getDefaultPageHTML(_status_code);
		setResponse(0, _path_clear, ".html");
	}
	return 1;
}
