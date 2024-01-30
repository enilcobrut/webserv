/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorStatus.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:25:19 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/09 12:25:20 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class ErrorStatus
{
private:
	std::map<int, const char*> errorStatus;
public:
	ErrorStatus();
	~ErrorStatus();
	const std::string getErrorStatus(int &_status_code) const;
};

