/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorStatus.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:25:04 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/09 12:25:06 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorStatus.hpp"

ErrorStatus::ErrorStatus() {
    errorStatus[100] = "Continue",
    errorStatus[101] = "Switching Protocols",
    errorStatus[102] = "Processing",
    errorStatus[200] = "OK",
    errorStatus[201] = "Created",
    errorStatus[202] = "Accepted",
    errorStatus[203] = "Non-Authoritative Information",
    errorStatus[204] = "No Content",
    errorStatus[205] = "Reset Content",
    errorStatus[206] = "Partial Content",
    errorStatus[207] = "Multi-Status",
    errorStatus[208] = "Already Reported",
    errorStatus[226] = "IM Used",
    errorStatus[300] = "Multiple Choices",
    errorStatus[301] = "Moved Permanently",
    errorStatus[302] = "Found",
    errorStatus[303] = "See Other",
    errorStatus[304] = "Not Modified",
    errorStatus[305] = "Use Proxy",
    errorStatus[307] = "Temporary Redirect",
    errorStatus[308] = "Permanent Redirect",
    errorStatus[400] = "Bad Request",
    errorStatus[402] = "Payment Required",
    errorStatus[403] = "Forbidden",
    errorStatus[404] = "Not Found",
    errorStatus[405] = "Method Not Allowed",
    errorStatus[406] = "Not Acceptable",
    errorStatus[407] = "Proxy Authentification Required",
    errorStatus[408] = "Request Timeout",
    errorStatus[409] = "Conflict",
    errorStatus[410] = "Gone",
    errorStatus[411] = "Length Required",
    errorStatus[412] = "Precondition Failed",
    errorStatus[413] = "Payload Too Large",
    errorStatus[414] = "Request-URI Too Long",
    errorStatus[415] = "Unsupported Media Type",
    errorStatus[416] = "Requested Unsupported Media Type",
    errorStatus[417] = "Expectation Failed",
    errorStatus[418] = "I'm a teapot",
    errorStatus[421] = "Misdirect Request",
    errorStatus[422] = "Unprocessable Entity",
    errorStatus[423] = "Locked",
    errorStatus[424] = "Failed Dependency",
    errorStatus[426] = "Upgrade Required",
    errorStatus[428] = "Precondition Required",
    errorStatus[429] = "Too Many Requests",
    errorStatus[431] = "Request Header Fields Too Large",
    errorStatus[499] = "Client Closed Request",
    errorStatus[500] = "Internal Server Error",
    errorStatus[501] = "Not Implemented",
    errorStatus[502] = "Bad Gateway",
    errorStatus[503] = "Service Unavailable",
    errorStatus[504] = "Gateway Timeout",
    errorStatus[505] = "HTTP Version Not Supported",
    errorStatus[506] = "Variant Also Negotiates",
    errorStatus[507] = "Insufficient Storage",
    errorStatus[508] = "Loop Detected",
    errorStatus[510] = "Not Extended",
    errorStatus[511] = "Network Authentification Required",
    errorStatus[599] = "Network Connect Timeout Error";
}

ErrorStatus::~ErrorStatus(){};

const std::string ErrorStatus::getErrorStatus(int &_status_code) const
{
	std::string status;
	try {
		status = errorStatus.at(_status_code);
	}
	catch (const std::exception &e) {
		_status_code = 500;
		status = errorStatus.at(_status_code);
	}
	return status;
}
