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

#ifndef _EVERNOTEDATAPROVIDER_H_
#define _EVERNOTEDATAPROVIDER_H_

#include <iostream>

#include <Python.h>
#include "notify.hh"
namespace evernote {

static std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}
class Note {
public:
    std::string title;
    std::string guid;
    std::string content;
    std::string notebook_guid;
    long int created;
    long int updated;
    bool deleted;
    Note (std::string t, std::string g, std::string c, std::string n_g, long int c_time, long int u_time, bool d = false) {
        title = t; guid = g; content = c; notebook_guid = n_g;
        created = c_time / 1000; updated = u_time / 1000; deleted = d;
    }

    std::string createInsertStatement () {
    	/*	
	    	CREATE TABLE notes (id integer primary key, title text, body text, created_time datetime, modified_time datetime, 
	    	guid text, notebook_guid text, usn integer, dirty integer);
		*/

        std::string query = "INSERT INTO NOTES VALUES (NULL,'";
        query += ReplaceString (title, "'", "''");
        query += "','";
        query += ReplaceString (content, "'", "''");
        query += "',";
        query += NumberToString (created);
        query += ",";
        query += NumberToString (updated);
        query += ",'";
        query += guid;
        query += "','";
        query += notebook_guid;
        query += "', 0, 0);";
		return query;
    }
};

class Notebook {
public:
    std::string name;
    std::string guid;
    bool isDefaultNotebook;
    long int created;
    long int updated;
    Notebook (std::string n,std::string g,bool d ,long int c, long int u) {
        name = n; guid = g; isDefaultNotebook = d; created = c / 1000; updated = u / 1000;
    }

    std::string createInsertStatement () {
	    /*
	    CREATE TABLE notebooks (id integer primary key, title text unique, guid text, parent_guid text, created_time datetime, 
	    modified_time datetime, usn integer, dirty integer)
		*/

        std::string query = "INSERT INTO NOTEBOOKS VALUES (NULL,'";
        query += ReplaceString (name, "'", "''");
        query += "','";
        query += guid;
        query += "','";
        /* Parent GUID should go here. */
        query += "',";
        query += NumberToString (created);
        query += ",";
        query += NumberToString (updated);
        query += ", 0, 0);";
		return query;
    }
};



class EvernoteDataProvider{
private:
	bool hasOAuthToken;
	std::string authToken;

	int notebookCount;

	PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;

    int lastUpdateCount;

    Notify* app;

public:
	EvernoteDataProvider (Notify* n);
	~EvernoteDataProvider ();

	int open ();
	int close ();
	int sync ();
	int login ();
	int logout ();
	int getNotebookCountPy ();
	int getNotebookDetails ();
	std::string getNotebookName (int);
	std::string getNotebookGuid (int);
	long int getNotebookServiceUpdated (int);
	long int getNotebookServiceCreated (int);
	bool getNotebookIsDefault (int);
	bool getNotesForNotebook (std::string);

	void setHasOAuthToken (bool b) {
		hasOAuthToken = b;
	}

	bool getHasOAuthToken () {
		return hasOAuthToken;
	}

	void setAuthToken (std::string at) {
		authToken = at;
	}

	std::string getAuthToken () {
		return authToken;
	}

	int getNotebookCount () { return notebookCount; }
	void setNotebookCount (int a) { notebookCount = a; }
};

}
#endif