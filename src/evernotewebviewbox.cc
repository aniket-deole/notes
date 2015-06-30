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
#include <fstream>
#include <assert.h>
#include <openssl/md5.h>
#include <notepaneview.hh>
#include <evernotewebviewbox.hh>

EvernoteWebViewBox::EvernoteWebViewBox (bool homogeneous, int spacing, Gtk::PackOptions options, int padding, Notify* a) 
  : Gtk::Box (Gtk::ORIENTATION_VERTICAL, padding) {
  app = a;
	set_orientation (Gtk::ORIENTATION_VERTICAL);

	webviewWrapper = Gtk::manage (new Gtk::ScrolledWindow ());

	webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
	
	GtkScrolledWindow* webviewWrapper_c = webviewWrapper->gobj ();
	gtk_container_add (GTK_CONTAINER (webviewWrapper_c), GTK_WIDGET (webview));

	webkit_web_view_set_editable(webview, false);
    
	WebKitWebSettings* wkws = webkit_web_view_get_settings ( webview);
	g_object_set (G_OBJECT(wkws), "default-font-size", 10, NULL);	
	g_object_set (G_OBJECT(wkws), "enable-offline-web-application-cache", true, NULL);	
	g_object_set (G_OBJECT(wkws), "tab-key-cycles-through-elements", false, NULL);	
	

	webviewWrapper->set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	pack_start (*webviewWrapper);

	show_all_children ();
}

EvernoteWebViewBox::~EvernoteWebViewBox () {}

