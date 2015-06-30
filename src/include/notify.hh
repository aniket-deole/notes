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

#ifndef _NOTIFY_HH_
#define _NOTIFY_HH_

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include "porting.hh"
#include "windowbody.hh"
#include "leftpaneview.hh"
#include "maintoolbar.hh"
#include "notelistpaneview.hh"
#include "notepaneview.hh"
#include "databasemanager.hh"
#include "sync.hh"

class WindowBody;
class DatabaseManager;
class SyncManager;

class Notify : public Gtk::Window {
public:
	Notify ();
	virtual ~Notify ();

  MainToolbar* mainToolbar;
  WindowBody* windowBody;
	LeftPaneView* lpv;
	NoteListPaneView* nlpv;
	NotePaneView* npv;

	DatabaseManager* dbm;
	SyncManager* sm;	
protected:
	void on_button_clicked();
	Gtk::Button m_button;

private:
    void setupDatabase ();
};

#endif

