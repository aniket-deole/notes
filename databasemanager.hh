#ifndef _DATABASEMANAGER_HH_
#define _DATABASEMANAGER_HH_

#include <gtkmm.h>
#include <sqlite3.h>
#include "notify.hh"

class Notify;

class DatabaseManager {
private:
  Notify* app;

  sqlite3 *db;
public:
  DatabaseManager (Notify*);
  ~DatabaseManager ();

  int exec (std::string, int (*callback)(void*,int,char**,char**), void *);
};

#endif
