#include <iostream>

#include "databasemanager.hh"

DatabaseManager::DatabaseManager (Notify* a) {
	app = a;

	db = 0;
    if (sqlite3_open ("notify.db", &db) != SQLITE_OK) {
	    /* Report ERROR */
	    std::cout << "Debug.5: Error Opening db." << std::endl;
	    exit (-1);
	}

	if (sqlite3_exec (db, "select * from notebooks", NULL, 0, NULL) != SQLITE_OK) {
	    /* DB is not setup. This is the first run. Or the file 
	     * has been corrupted. */
	    sqlite3_exec (db, "CREATE TABLE notebooks (id int primary key, title text, parent_id int)", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE notes (id int primary key, title text, body text, notebook_id int, created_time datetime, modified_time datetime);", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE notes_tags_xref (note_id int, tag_id int);", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE tags (id int primary key, title text);", NULL, 0, NULL);

	    sqlite3_exec (db, "INSERT INTO notebooks values (0,'All Notebooks', 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO tags values (0,'All Tags')", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (42,'First Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (2,'Second Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (3,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (4,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (5,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (6,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (7,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (8,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (9,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (10,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (11,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (12,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (13,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (14,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (15,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (16,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (17,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (18,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (19,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (20,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notes values (21,'Third Note', 'Who has been strangely quite for the past few days as her public profile.', 1, 0, 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "COMMIT", NULL, NULL, NULL);
	}
}

int DatabaseManager::exec (std::string statement, int (*callback)(void*,int,char**,char**), void* classPointer) {
	char* errMsg = 0;
	int r = sqlite3_exec (db, statement.c_str (), callback, classPointer,  &errMsg);
	if (r != SQLITE_OK)
		std::cout << "Debug.7: Error Fetching Data From db: " << errMsg << ", returnValue: " << r << std::endl;
	return r;
}

DatabaseManager::~DatabaseManager () {
	sqlite3_close (db);
}