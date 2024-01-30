/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <flemaitr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:28:40 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/12 15:52:53 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <arpa/inet.h>
#include <curl/curl.h>
#include <ctime>
#include <deque>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <queue>
#include <set>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#define ORANGE						"\033[1;38;2;255;187;54m"
#define VIOLET						"\033[1;38;2;158;46;156m"
#define BLUE						"\033[1;38;2;73;153;254m"
#define RED							"\033[1;38;2;255;0;0m"
#define GREEN						"\033[1;38;2;69;176;26m"
#define YELLOW						"\033[1;38;2;241;223;9m"
#define PINK						"\033[1;38;2;240;170;223m"
#define CYAN						"\033[1;38;2;43;236;195m"
#define JAUNE						"\033[1;38;2;251;255;66"
#define RESET						"\033[0m"
#define PORT						8883
#define TRUE						1
#define FALSE						0
#define MAXLINE						4096
#define MAX_EVENTS					1024
#define RN							"\r\n"
#define RNRN						"\r\n\r\n"
#define TIME_OUT					6000

enum
{
	GET = 1,
	POST,
	DELETE,
	HEAD,
	UPLOAD
};

extern char***						g_envPtr;
extern std::map<std::string, bool>	g_cookie;


//--------------------------------------------------------------------//
//					    	   Tools.cpp	  						  //
//--------------------------------------------------------------------//

const std::vector<std::string>		splitLine(std::string &line);
int									isValidFile(const std::string &path);
int									isValidDirectory(const std::string &path);
std::string							decodeUrl(const std::string &str);
std::string							encodeUrl(const std::string &str);
