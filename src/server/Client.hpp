/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:26:06 by cjunker           #+#    #+#             */
/*   Updated: 2023/06/09 12:33:29 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <string>
#include "../config/ServerConfig.hpp"
#include "../http/Response.hpp"

class Client {
    private:
        std::string		body;
		ServerConfig	_config_client;

    public:
        size_t content_length;
        std::string request;
        bool response;
        bool is_body;
        bool is_send;
        size_t sent_bytes;
        std::string remaining_data;
        bool all_body;
        Response resp;
    
		Client() {};
        Client(const ServerConfig &config) : body(""), _config_client(config), content_length(0), request(""), response(false),
            is_body(false), is_send(false), sent_bytes(0), remaining_data(""), all_body(false) {}

        Client(const Client& other) : body(other.body), _config_client(other._config_client), content_length(other.content_length),
            request(other.request), response(other.response), is_body(other.is_body), is_send(other.is_send), sent_bytes(other.sent_bytes),
            remaining_data(other.remaining_data) {}

		Client	&operator=(const Client& other);

        const std::string &getBody() const {
            return body;
        }

        const std::string &getRequest() const {
            return request;
        }

        void appendToBody(const char* str, size_t len) {
            body.append(str, len);
        }

		const ServerConfig	&getConfigClient() const {
			return (_config_client);
		}
};
