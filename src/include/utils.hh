/***
	Copyright (C) 2013 Aniket Deole <aniket.deole@gmail.com>
	This program is free software: you can redistribute it and/or modify it
	under the terms of the GNU Lesser General Public License version 2.1, as published
	by the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranties of
	MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
	PURPOSE.  See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef __UTILS_HH_
#define __UTILS_HH_

#include <iostream>
#include <fstream>
#include <assert.h>
#include <openssl/md5.h>

std::string base64_encode (unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode (std::string const& encoded_string);

std::string replaceString (std::string subject, const std::string& search,
		const std::string& replace);

template <typename T> std::string NumberToString (T);

std:: string replaceSingleQuote (std::string const& original);

void addCss (Gtk::Widget*, std::string cssClass, std::string css);
#endif

