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
