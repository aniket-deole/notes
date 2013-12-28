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

#include <iostream>

#include "databasemanager.hh"

DatabaseManager::DatabaseManager (Notify* a) {
	app = a;

	db = 0;
    if (sqlite3_open ("/home/aniket/.local/notify.db", &db) != SQLITE_OK) {
	    /* Report ERROR */
	    std::cout << "Debug.5: Cannot Create Database." << std::endl;
	    exit (-1);
	}

	if (sqlite3_exec (db, "select * from notebooks", NULL, 0, NULL) != SQLITE_OK) {
	    /* DB is not setup. This is the first run. Or the file 
	     * has been corrupted. */
	    sqlite3_exec (db, "CREATE TABLE notebooks (id integer primary key, title text, parent_id int)", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE notes (id integer primary key, title text, body text, notebook_id int, created_time datetime, modified_time datetime);", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE notes_tags_xref (note_id int, tag_id int);", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE tags (id integer primary key, title text);", NULL, 0, NULL);

	    sqlite3_exec (db, "INSERT INTO notebooks values (0,'All Notebooks', 0)", NULL, 0, NULL);
		sqlite3_exec (db, "INSERT INTO tags values (0,'All Tags')", NULL, 0, NULL);
	    
	    sqlite3_exec (db, "INSERT INTO notebooks values (NULL, 'FirstNotebook', 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "COMMIT", NULL, NULL, NULL);
	}
}

int DatabaseManager::exec (std::string statement, int (*callback)(void*,int,char**,char**), void* classPointer) {
	char* errMsg = 0;
	int r = sqlite3_exec (db, statement.c_str (), callback, classPointer,  &errMsg);
	if (r != SQLITE_OK)
		std::cout << "Debug.7: Error Fetching Data From db: returnValue: " << r << ", statement: " << statement << std::endl;
	return r;
}

DatabaseManager::~DatabaseManager () {
	sqlite3_close (db);
}