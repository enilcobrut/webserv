/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:24:42 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/09 12:24:44 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

const std::vector<std::string> splitLine(std::string &line)
{
    std::vector<std::string> vec;
    if (!line.size())
        return vec;
    size_t i = 0;
    while (i < line.size()) {
        while (i < line.size() && line[i] == ' ')
            i++;
        if (i == line.size())
            break;
        size_t j = i;
        while (j < line.size() && line[j] != ' ')
            j++;
        vec.push_back(line.substr(i, j - i));
        i = j;
    }
    return vec;
}

int isValidFile(const std::string &path)
{
	std::ifstream file;

	file.open(path);
	if (!file.is_open())
		return 0;
	file.close();
	return 1;
}

int isValidDirectory(const std::string &path)
{
	DIR *dir = opendir(path.c_str());
	if (!dir)
		return 0;
	closedir(dir);
	return 1;	
}

std::string decodeUrl(const std::string &str) {
	std::string	ret;
	char		ch;
	int			j;

	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] == '%') {
			sscanf(str.substr(i + 1, 2).c_str(), "%x", &j);
			ch = static_cast<char>(j);
			ret += ch;
			i += 2;
		} else {
			ret += str[i];
		}
	}
	return (ret);
}

std::string encodeUrl(const std::string &str)
{
	std::string	ret;
    std::size_t pos;
	ret = str;
    while ((pos = ret.find(' ')) != std::string::npos)
    {
        ret.erase(pos, 1);
        ret.insert(pos, "%20");
    }
	return (ret);
}