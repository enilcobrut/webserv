/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:26:30 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/12 15:46:58 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "webserv.hpp"

std::string Response::generateSessionID() {
    int                 random_number = rand();
    std::stringstream   ss;
    time_t              now = time(0);
    ss << now << random_number;
    return (ss.str());
}

bool Response::cookieIsTaken(void)
{
	if (!_req.getCookieSession().size())
		return false;
	if (g_cookie.count(_req.getCookieSession())) {
		//readMap();
		return true;
	}
	else {
		g_cookie.insert(std::make_pair<std::string, bool>(_req.getCookieSession(), true));
	}
	//readMap();
	return false;
}

void Response::getValueCookieCgi(void)
{
	if (_content.substr(0, 16) == "<!--SetCookie-->")
	{
		if (_req.getCookieSession().size()) {
			getResponsePageHTML(0, "The cookie is ", "already set.");
			return ;
		}
		std::size_t start = _content.find("</em>", 0);
		std::size_t end = _content.find("</div>", start);
		std::string cookie_tmp;
		if (start != std::string::npos && end != std::string::npos)
			_req.setCookie(_content.substr(start + 5, end - (start + 5)));
		if (cookieIsTaken())
			_req.setCookie(generateSessionID());
		getResponsePageHTML(0, "        The cookie has been set: ", _req.getCookieSession());
		_header = _header + "Set-Cookie: sessionID=" + _req.getCookieSession() + "; path=/; max-age=600" + RN;
	}
}
