/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:25:30 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/09 12:25:32 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Mime
{
private:
	std::map<const std::string, const char*> mime;
public:
	const std::string getMime(std::string &_extension) const;
	bool isInMap(const std::string &_extension);
	Mime();
	~Mime();
};

