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

#ifndef _NOTEDATA_HH_
#define _NOTEDATA_HH_

class NoteData {
private:
  int primary_key;
   
  std::string title;
  std::string remaining;
  std::string summary;
public:

  NoteData () {}

  NoteData (int p_key, std::string t, std::string r, std::string s) {
    primary_key = p_key;
    title = t;
    remaining = r;
    summary = s;
  }

  int getPrimaryKey () { return primary_key; }

  std::string getTitle () { return title; }
  std::string getRemaining () { return remaining; }
  std::string getSummary () { return summary; }
};


#endif
