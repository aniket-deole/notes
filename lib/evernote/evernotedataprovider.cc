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

#include "evernotedataprovider.hh"

EvernoteDataProvider::EvernoteDataProvider () {
	hasOAuthToken = false;
	authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";

	/* Add Evernote python libs to PYTHONPATH */

	/* Check if module is working. Instantiate it. */
}

int EvernoteDataProvider::open () {
	/* Connect using auth */

	return 0;
}

int EvernoteDataProvider::close () {
	return 0;
}

int EvernoteDataProvider::sync () {
	return 0;
}

int EvernoteDataProvider::login () {
	
}

int EvernoteDataProvider::logout () {

}