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

#ifndef _NOTEVIEW_HH_
#define _NOTEVIEW_HH_

#include <gtkmm.h>
#include <webkit/webkit.h>
#include "notedata.hh"
#include "databasemanager.hh"

class DatabaseManager;
class LeftPaneView;

class NotePaneView : public Gtk::Box {
private:
	int a;
	WebKitWebView* webview;

	Gtk::ScrolledWindow* webviewWrapper;

	DatabaseManager* dbm;
	Gtk::Entry* noteTitle;

	Gtk::Button* saveButton;
	NoteData nd;

	Notify* app;

	Gtk::Button* boldButton;
	Gtk::Button* italicsButton;
	Gtk::Button* underlineButton;
	Gtk::Button* strikeButton;
	Gtk::Button* clearFormattingButton;
	Gtk::Button* tabOutButton;
	Gtk::Button* tabInButton;
	Gtk::Button* justifyButton;
	Gtk::Button* olistButton;
	Gtk::Button* ulistButton;
	Gtk::Button* clistButton;

public:
	NotePaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0);
	~NotePaneView ();

	void setDatabaseManager (DatabaseManager* d);
	void setWebViewContent (std::string);
	void setNoteTitleEntryText (std::string);
	void setNote (NoteData n);
	void newNote ();
	void disableButtons ();
	void enableButtons ();
	void saveNote ();
	void setApp (Notify* n) { app = n; }

	void boldButtonCallback();
	void italicsButtonCallback();
	void underlineButtonCallback();
	void strikeButtonCallback();
	void clearFormattingButtonCallback();
	void tabOutButtonCallback();
	void tabInButtonCallback();
	void justifyButtonCallback();
	void olistButtonCallback();
	void ulistButtonCallback();
	void clistButtonCallback();
};
#endif
