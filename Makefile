# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: flemaitr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/09 12:27:42 by flemaitr          #+#    #+#              #
#    Updated: 2023/06/09 18:42:25 by cjunker          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS = "-j 10"

NAME =			webserv

HEADER = 		includes/webserv.hpp

SRCS =			src/main.cpp \
				src/cgi/cgi.cpp \
				src/config/ConfigFile.cpp \
				src/config/Route.cpp \
				src/config/ServerConfig.cpp \
				src/http/cookie.cpp \
				src/http/ErrorStatus.cpp \
				src/http/html.cpp \
				src/http/Mimes.cpp \
				src/http/RequestMethod.cpp \
				src/http/Response.cpp \
				src/http/ResponseTools.cpp \
				src/server/Server.cpp \
				src/server/Client.cpp \
				src/utils/Tools.cpp

OBJS =          $(SRCS:.cpp=.o)

INCLUDES =		-I./includes

CC = 			c++
CPPFLAGS =		-Wall -Wextra -Werror -std=c++98# -g -fsanitize=address 

all: 			$(NAME)

clean:
				@rm -f $(OBJS) $(OBJS_BONUS)

fclean:		   clean
				@rm -f $(NAME) $(NAME)_bonus

re:             
				make fclean
				make all

r:			
				make all
				./webserv config_file.json

%.o:			%.cpp $(HEADER) $(HEADER_BONUS)
				$(CC) $< $(CPPFLAGS) $(INCLUDES) -c -o $@

$(NAME): 		$(OBJS) $(HEADER)
				@$(CC) $(OBJS) $(CPPFLAGS) $(INCLUDES) -o $(NAME)

.PHONY: 		all clean fclean re r

# Reset
RESET		=\033[0m

# Background
bg_black	=\033[40m

# Regular Colors
black		=\033[0;30m
red			=\033[0;31m
green		=\033[38;2;23;99;41m
yellow		=\033[38;2;238;153;47m
blue		=\033[0;34m
purple		=\033[0;35m
cyan		=\033[0;36m
blue_c		=\033[38;2;153;238;251m
white		=\033[0;37m
brown		=\033[38;2;158;72;29m
