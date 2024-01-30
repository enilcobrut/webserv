/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjunker <cjunker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:47:53 by alevasse          #+#    #+#             */
/*   Updated: 2023/06/12 14:00:27 by cjunker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Route {
	private:
		std::string											_path;
		std::vector<std::string>							_allowed_methods;
		std::string											_root;
		bool												_directory_listing;
		std::string											_default_file;
		std::map<std::string, std::string>					_upload;
		std::string											_redirect;
		std::map<std::string, std::vector<std::string> >	_cgi;
	public:

		Route();

		const std::string	getPath() const;
		const std::string	getRoot() const;
		bool	getDirectoryListing() const;
		const std::string	getDefaultFile() const;
		const std::string   getRedirect() const;
		const std::map<std::string, std::string>	getUpload() const;

		void	setPath(const std::string &path);
		void	addAllowedMethod(const std::string &method);
		void	setRoot(const std::string &root);
		void	setDirectoryListing(bool directory_listing);
		void	setDefaultFile(const std::string &default_file);
		void    setRedirect(const std::string &url);
		void	addUpload(const std::string &path, const std::string &value);
		void	addCgi(const std::string &path, const std::vector<std::string> &cgi_config);

		void	parseRoute(std::string::iterator &it, const std::string::iterator &end);
		bool	parseBoolean(std::string::iterator &it, const std::string::iterator &end);
		std::vector<std::string>	parseVectorString(std::string::iterator &it, const std::string::iterator &end);
		std::map<std::string, std::vector<std::string> >	parseMapVector(std::string::iterator &it, const std::string::iterator &end);
		std::map<std::string, std::string>	parseMapString(std::string::iterator &it, const std::string::iterator &end);
		bool    isAllowedMethods(int method) const;
		bool    isAllowedCgi(const std::string &cgi) const;
		const std::string	&getCgi(const std::string &extension) const;
		std::string	getCgiPath() const;
};
