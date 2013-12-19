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

#include "notepaneview.hh"
/*
 * This files contains C Code as I was not able to find a good C++ gtkmm e
 * equivalent wrapper for webkit. Open to suggestions :)
 */

NotePaneView::NotePaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {
	set_orientation (Gtk::ORIENTATION_VERTICAL);

	webviewWrapper = Gtk::manage (new Gtk::ScrolledWindow ());

	webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
	
	GtkScrolledWindow* webviewWrapper_c = webviewWrapper->gobj ();
	gtk_scrolled_window_add_with_viewport (webviewWrapper_c, GTK_WIDGET (webview));

	webkit_web_view_set_editable(webview, true);
    
	webkit_web_view_load_string (webview, "<html><body>Hel<b>lo</b><a href=\"http://www.google.com\">asdasd</a></body></html>", "text/html", NULL, NULL);
//	webkit_web_view_load_uri (webview, "http://news.google.com");

	pack_start (*webviewWrapper);
	show_all ();
}

NotePaneView::~NotePaneView () {}


void NotePaneView::setDatabaseManager (DatabaseManager* d) {
    dbm = d;
}

void NotePaneView::setWebViewContent (std::string content) {
	webkit_web_view_load_string (webview, content.c_str (), "text/html", NULL, NULL);
}