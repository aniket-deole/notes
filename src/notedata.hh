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

#include <cstring>
#include <ctime>

class NoteData {
private:
  int primary_key;
   
  std::string title;
  std::string remaining;
  std::string body;
  std::string summary;
  std::string notebookName;

  long int create_unix_time;
  long int modified_unix_time;

  std::string guid;
  std::string notebook_guid;

public:

  NoteData () {}
  NoteData (int p_key, std::string t, std::string b, int create_time, int modified_time, std::string g, std::string n_guid, std::string n_name);

  std::string getTitle () { return title; }
  std::string getRemaining () { 
    char buffer[80];
    struct tm* timeinfo = localtime (&modified_unix_time);
    strftime (buffer,80,"%F %R",timeinfo);
    std::string* rem = new std::string (buffer, strnlen(buffer, 80));
    return *rem;
   }
   int getPrimaryKey () { return primary_key; }
  std::string getSummary () { return summary; }
  std::string getBody () { return body; }
  std::string* getBodyPointer () {return &body;}
  void setBody (std::string b) { this->body.clear (); this->body = b; }
  int getCreateTime () { return create_unix_time; }

  int getModifiedTime () { return modified_unix_time; }

  void set_modified_time (int modified_time) {
    modified_unix_time = modified_time;
  }

  void setGuid (std::string g) {
    guid = g;
  }
  std::string getGuid () {
    return guid;
  }
  void setNotebookGuid (std::string g) {
    notebook_guid = g;
  }
  std::string getNotebookGuid () {
    return notebook_guid;
  }
  std::string getNotebookName () {
    return notebookName;
  }
};


#endif
