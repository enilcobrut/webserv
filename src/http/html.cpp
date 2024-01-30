/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   html.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:26:38 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/13 11:45:29 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Response.hpp"

void Response::getResponsePageHTML(int type, const std::string str1, const std::string str2)
{
    std::ifstream file;
    if (isValidFile("response.html"))
        file.open("response.html");
    else
        throw std::runtime_error("404");  
    std::ostringstream outfile;
    std::string		file_content;

	while (getline(file, file_content, '\n'))
	{
    if (file_content == "<p>CONTENT</p>")
    {
        if (type == UPLOAD)
        {
            outfile << "<div class=\"section-heading\">";
            outfile << "<h4><em>";
            outfile << "File successfully uploaded : </em>";
            outfile << str1;
            outfile << "</h4>";
            outfile << "</div>";
            outfile << "<div><img src=\"/upload/" << str1;
            outfile << "\"></div>";
        }
        else
        {
            outfile << "<div class=\"section-heading\">";
            outfile << "<h4><em>";
            outfile << str1 << " : </em>";
            outfile << str2;
            outfile << "</h4>";
            outfile << "</div>";
        }
    }
    else
        outfile << file_content;
	}	
	file.close();
    if (_content.size())
        _content.clear();
    _content = outfile.str();
}

void Response::getDefaultPageHTML(int &error)
{
    std::ifstream file;
    if (isValidFile("www/default.html"))
        file.open("www/default.html");
    else
        throw std::runtime_error("404");

    std::ostringstream outfile;
    std::string		file_content;

	while (getline(file, file_content, '\n'))
	{
        if (file_content == "<title></title>")
        {
            outfile << "<title>";
            outfile << "Error " << error << " " << _errorMap.getErrorStatus(error);
            outfile << "</title>";
        }
        else if (file_content == "<p>Error default</p>")
        {
            outfile << "<h1>";
            outfile << "Error " << error;
            outfile << "</h1><p>";
            outfile << _errorMap.getErrorStatus(error);         
            outfile << "</p>";
        }
        else
            outfile << file_content;
	}	
	file.close();
    _content = outfile.str();
}

void Response::getDeleteHTML(const char *path)
{

    std::ifstream file;
  
	if (isValidFile("delete.html"))
        file.open("delete.html");
    else
        throw std::runtime_error("404");

    std::ostringstream outfile;
    std::string		file_content;
	while (getline(file, file_content, '\n'))
	{
        if (file_content == "<p>CONTENT</p>")
        {
            std::vector <std::string> vec = getAutoIndex("www/upload");
            std::vector<std::string>::iterator it = vec.begin() + 2;
            if (vec.size())
            {
                for (; it != vec.end(); ++it)
                {
                    if (*it != "delete.html") {
                        outfile << "<div style=\"display: table-row;\">";
                        outfile << "<div style=\"display: table-cell; width: 30%;\">";
                        outfile << "<img width=\"350 px\" src=\"" << path << "/" << *it << "\">";
                        outfile << "</div>";
                        outfile << "<div style=\"display: table-cell;\">";
                        outfile << "<button class=\"accent-button button\" onclick=\"deleteFile('";
                        outfile << *it;
                        outfile << "')\">Delete file</button>";
                        outfile << "</div>";
                        outfile << "</div>";
                        outfile << "<br>";
                    }
                }
            }
            
        }
        else
            outfile << file_content;
	}	
	file.close();
    _content = outfile.str();
}


void Response::getDirHTML(void)
{
    std::string path = _req.getUri();
    if (path[path.size() - 1] != '/')
        path = path + "/";
    std::ifstream file;
    if (isValidFile("directory.html"))
        file.open("directory.html");
    else
        throw std::runtime_error("404");

    std::ostringstream outfile;
    std::string	file_content;
	
    struct stat sb;
	while (getline(file, file_content, '\n'))
	{
        if (file_content == "<p>CONTENT</p>")
        {
            std::vector <std::string> vec = getAutoIndex(_path_clear.c_str());
            std::vector<std::string>::iterator it = vec.begin() + 2;
            if (vec.size())
            {
                outfile << "<style>";
                outfile << "table {";
                outfile << "    border-collapse: collapse;";
                outfile << "    width: 100%;";
                outfile << "}";
                outfile << "td, th {";
                outfile << "    padding: 8px;";
                outfile << "    text-align: left;";
                outfile << "}";
                outfile << "tr:nth-child(even) {";
                outfile << "    background-color: #f2f2f2;";
                outfile << "}";
                outfile << "</style>";
                outfile << "<table class=\"my-table\">";
                outfile << "<thead>";
                outfile << "<tr>";
                outfile << "<th>Name</th>";
                outfile << "<th>Last state change</th>";
                outfile << "<th>Last file access</th>";
                outfile << "<th>Last file modification</th>";
                outfile << "<th>File size</th>";
                outfile << "</tr>";
                outfile << "</thead>";
                outfile << "<tbody>";
                std::vector<std::string>::iterator ite = vec.end();
                for (; it != ite; ++it)
                {
                    if (stat((_path_clear + "/" + *it).c_str(), &sb) == -1) {
                        throw std::runtime_error("500");
                    }
                    stat((_path_clear + "/" + *it).c_str(), &sb);
                    outfile << "<tr>";
                    outfile << "<td><a href=\"" << path << *it << "\">" << *it << "</a></td>";
                    outfile << "<td>" << ctime(&sb.st_ctime) << "</td>";
                    outfile << "<td>" << ctime(&sb.st_atime) << "</td>";
                    outfile << "<td>" << ctime(&sb.st_mtime) << "</td>";
                    outfile << "<td>" << (long long)sb.st_size << "</td>";
                    outfile << "</tr>";
                }
                outfile << "</tbody>";
                outfile << "</table>";
            }
        }
        else
            outfile << file_content;
	}	
	file.close();
    _content = outfile.str();
}

const std::vector <std::string> Response::getAutoIndex(const char *path)
{
	struct dirent *ent;
    std::vector<std::string> vec;

	DIR *dir = opendir(path);
	if (!dir)
		throw std::runtime_error("500");
	while ((ent = readdir(dir)) != NULL)
        vec.push_back(ent->d_name);
	if (closedir(dir) == -1)
		throw std::runtime_error("500");
	return vec;
}
