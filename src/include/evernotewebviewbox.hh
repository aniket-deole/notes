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

#ifndef _EVERNOTE_WEBVIEW_BOX_HH_
#define _EVERNOTE_WEBVIEW_BOX_HH_

#include <gtkmm.h>
#include <webkit/webkit.h>
#include "rapidxml_print.hpp"
#include "rapidxml.hpp"

#include "notedata.hh"
#include "databasemanager.hh"
#if HASPDF
#include <fstream>
#include <pdf.h>
#endif /* HASPDF */
class DatabaseManager;
class LeftPaneView;

class EvernoteWebViewBox : public Gtk::Box {
private:
	int a;

	Gtk::ScrolledWindow* webviewWrapper;

	Notify* app;

public:
	EvernoteWebViewBox (bool homogeneous, int spacing, Gtk::PackOptions options,
			int padding = 0, Notify* a = NULL);
	~EvernoteWebViewBox ();
	
	WebKitWebView* webview;
};
#endif
