/***
Copyright (C) 2013-2015 Aniket Deole <aniket.deole@gmail.com>
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

#ifndef _WEBVIEWPOPUPVIEW_HH_
#define _WEBVIEWPOPUPVIEW_HH_

#include <vector>
#include <gtkmm.h>
#include "notify.hh"

class Notify;
class DatabaseManager;

class WebViewPopup : public Gtk::Dialog {
private:
  Notify* app;
	WebKitWebView* webview;

public:
    WebViewPopup (std::string, Gtk::Window&, bool);
    ~WebViewPopup ();
};

#endif
