/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flemaitr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:25:27 by flemaitr          #+#    #+#             */
/*   Updated: 2023/06/09 12:25:28 by flemaitr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mimes.hpp"

Mime::Mime() {
    mime[".aac"] =		"audio/aac", // AAC audio
    mime[".abw"] =		"application/x-abiword", // AbiWord document	
    mime[".arc"] =		"application/x-freearc", // Archive document (multiple files embedded	
    mime[".avif"] =		"image/avif", // AVIF image	mime[
    mime[".avi"] =		"video/x-msvideo", // AVI: Audio Video Interleave	
    mime[".azw"] =		"application/vnd.amazon.ebook", // Amazon Kindle eBook format	
    mime[".bin"] =		"application/octet-stream", // Any kind of binary data	
    mime[".bmp"] =		"image/bmp", // Windows OS/2 Bitmap Graphics	
    mime[".bz"] =		"application/x-bzip", // 	BZip archive	
    mime[".bz2"] =		"application/x-bzip2", // BZip2 archive	
    mime[".cda"] =		"application/x-cdf", // CD audio	
    mime[".csh"] =		"application/x-csh", // C-Shell script	
    mime[".css"] =		"text/css", /// Cascading Style Sheets (CSS	
    mime[".csv"] =		"text/csv", // Comma-separated values (CSV	
    mime[".doc"] =		"application/msword", // Microsoft Word	
    mime[".docx"] =		"application/vnd.openxmlformats-officedocument.wordprocessingml.document", // Microsoft Word (OpenXML	
    mime[".eot"] =		"application/vnd.ms-fontobject", //MS Embedded OpenType fonts
    mime[".epub"] =		"application/epub+zip", //Electronic publication (EPUB
    mime[".woff2"] =	"font/woff2", //font du site
    mime[".gz"] =		"application/gzip", // GZip Compressed Archive	
    mime[".gif"] =		"image/gif", // Graphics Interchange Format (GIF	
    mime[".htm"] =		"text/html", // HyperText Markup Language (HTML	
    mime[".html"] =		"text/html", // HyperText Markup Language (HTML	
    mime[".ico"] =		"image/vnd.microsoft.icon", // Icon format	
    mime[".ics"] =		"text/calendar", // iCalendar format	
    mime[".jar"] =		"application/java-archive", // Java Archive (JAR	
    mime[".jpeg"] =		"image/jpeg", // JPEG images	
    mime[".jpg"] =		"image/jpeg", // JPEG images	
    mime[".js"] =		"text/javascript", // (Specifications: HTML and RFC 9239 JavaScript	
    mime[".json"] =		"application/json", // JSON format	
    mime[".jsonld"] =	"application/ld+json", //JSON-LD format	
    mime[".mid"] =		"audio/midi, audio/x-midi", //Musical Instrument Digital Interface (MIDI	
    mime[".midi"] =		"audio/midi, audio/x-midi", //Musical Instrument Digital Interface (MIDI	
    mime[".mjs"] =		"text/javascript", // JavaScript module	
    mime[".mp3"] =		"audio/mpeg", // MP3 audio	
    mime[".mp4"] =		"video/mp4", // MP4 video	
    mime[".mpeg"] =		"video/mpeg", // MPEG Video	
    mime[".mpkg"] =		"application/vnd.apple.installer+xml", // Apple Installer Package	
    mime[".odp"] =		"application/vnd.oasis.opendocument.presentation", // OpenDocument presentation document	
    mime[".ods"] =		"application/vnd.oasis.opendocument.spreadsheet", // OpenDocument spreadsheet document	
    mime[".odt"] =		"application/vnd.oasis.opendocument.text", //OpenDocument text document	
    mime[".oga"] =		"audio/ogg", // OGG audio	
    mime[".ogv"] =		"video/ogg", // OGG video	
    mime[".ogx"] =		"application/ogg", // OGG	
    mime[".opus"] =		"audio/opus", // Opus audio	
    mime[".otf"] =		"font/otf", // OpenType font	
    mime[".png"] =		"image/png", // Portable Network Graphics	
    mime[".pdf"] =		"application/pdf", // Adobe Portable Document Format (PDF	
    mime[".php"] =		"application/x-httpd-php", // Hypertext Preprocessor (Personal Home Page
    mime[".pl"] =		"application/x-perl",
    mime[".ppt"] =		"application/vnd.ms-powerpoint", // Microsoft PowerPoint	
    mime[".pptx"] =		"application/vnd.openxmlformats-officedocument.presentationml.presentation", // Microsoft PowerPoint (OpenXML
    mime[".py"] =       "application/x-python-code", // ou text/x-python
    mime[".rar"] =		"application/vnd.rar", // RAR archive	
    mime[".rtf"] =		"application/rtf", //Rich Text Format (RTF	
    mime[".sh"] =		"application/x-sh", // Bourne shell script	
    mime[".svg"] =		"image/svg+xml", // Scalable Vector Graphics (SVG	
    mime[".tar"] =		"application/x-tar", // Tape Archive (TAR	
    mime[".tif"] =		"image/tiff", // Tagged Image File Format (TIFF	
    mime[".tiff"] =		"image/tiff", // Tagged Image File Format (TIFF	
    mime[".ts"] =		"video/mp2t", // MPEG transport stream	
    mime[".ttf"] =		"font/ttf", // TrueType Font	
    mime[".txt"] =		"text/plain", // Text, (generally ASCII or ISO 8859-n	
    mime[".vsd"] =		"application/vnd.visio", // Microsoft Visio	
    mime[".wav"] =		"audio/wav", // Waveform Audio Format	
    mime[".weba"] =		"audio/webm", // WEBM audio	
    mime[".webm"] =		"video/webm", // WEBM video	
    mime[".webp"] =		"image/webp", // WEBP image	
    mime[".woff"] =		"font/woff", // Web Open Font Format (WOFF	
    mime[".woff2"] =	"font/woff2", // Web Open Font Format (WOFF	
    mime[".xhtml"] =	"application/xhtml+xml", // XHTML	
    mime[".xls"] =		"application/vnd.ms-excel", // Microsoft Excel	
    mime[".xlsx"] =		"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", // Microsoft Excel (OpenXML	
    mime[".xml"] =		"application/xml, text/xml", // application/xml is recommended as of RFC 7303 (section 4.1, XML but text/xml is still used sometimes. You can assign a specific MIME type to a file with .xml extension depending on how its contents are meant to be interpreted. For instance, an Atom feed is application/atom+xml, but application/xml serves as a valid default.
    mime[".xul"] =		"application/vnd.mozilla.xul+xml", // XUL	
    mime[".zip"] =		"application/zip", // ZIP archive	
    mime[".7z"] =		"application/x-7z-compressed"; // 	7-zip archive*/
}

Mime::~Mime(){};

const std::string Mime::getMime(std::string &_extension) const
{
	std::string content_type;
	try {
		content_type = mime.at(_extension);
	}
	catch (const std::exception &e) {
		_extension = ".txt";
		content_type = mime.at(_extension);
	}
	return content_type;
}

bool Mime::isInMap(const std::string &_extension)
{
	try {
		mime.at(_extension);
	}
	catch (const std::exception &e) {
		return false;
	}
	return true;
}
