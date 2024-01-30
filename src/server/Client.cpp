/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:25:12 by cjunker           #+#    #+#             */
/*   Updated: 2023/06/09 12:33:29 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client	&Client::operator=(const Client& other) {
	if (this != &other) {
		body = other.body;
		content_length = other.content_length;
		request = other.request;
		response = other.response;
		is_body = other.is_body;
		is_send = other.is_send;
		remaining_data = other.remaining_data;
		sent_bytes = other.sent_bytes;
		all_body = other.all_body;
		_config_client = other._config_client;
	}
	return *this;
}
