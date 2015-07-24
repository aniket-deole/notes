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
#include <welcomewindow.hh>
#include <utils.hh>

WelcomeWindow::WelcomeWindow(bool homogeneous, int spacing, Gtk::PackOptions options, int padding, Notify* a) 
  : Gtk::Box (Gtk::ORIENTATION_VERTICAL, padding) {
	set_orientation (Gtk::ORIENTATION_VERTICAL);

  app = a;
  addCss (this, "welcomeWindow", ".welcomeWindow {\n "
								" background-image:none; background-color:white;\n}\n");
	Gtk::Alignment *al = Gtk::manage(new Gtk::Alignment());
    al->set_size_request(0, 40);
  pack_start(*al, false, true); 
  
  welcomeString = Gtk::manage (new Gtk::Label ("Welcome to Notes For Linux", false));
 	addCss (welcomeString, "welcomeString", ".welcomeString {\n color:#333;\n font: OpenSans light 36; padding-top:10px;padding-bottom:10px; "
								" background-image:none; background-color:white;\n}\n");
	pack_start (*welcomeString, false, true, 0);
	
  evernoteSyncChoiceString = Gtk::manage (new Gtk::Label ("Would you like to connect to Evernote (Sandbox environment)?", false));
 	addCss (evernoteSyncChoiceString, "evernoteSyncChoiceString", ".evernoteSyncChoiceString{\n color:#888;\n font: OpenSans light 14; padding-top:10px;padding-bottom:10px; "
								" background-image:none; background-color:white;\n}\n");
	pack_start (*evernoteSyncChoiceString, false, true, 0);
  
  al = Gtk::manage(new Gtk::Alignment());
    al->set_size_request(0, 40);
  pack_start(*al, false, true); 
  
 Gtk::Box* buttonBox = Gtk::manage (new Gtk::Box ());
  syncWithEvernoteButton = Gtk::manage (new Gtk::Button ("Sync With Evernote", false));
  addCss (syncWithEvernoteButton, "syncWithEvernoteButton", ".syncWithEvernoteButton {"
      "border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;"
      "	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;\n}\n");

  buttonBox->pack_start (*syncWithEvernoteButton, true, false, 0);
 
  dontSyncButton = Gtk::manage (new Gtk::Button ("Don't use Evernote", false));
  addCss (dontSyncButton, "dontSyncButton", ".dontSyncButton{"
      "border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;"
      "	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;\n}\n");

  buttonBox->pack_start (*dontSyncButton, true, false, 0);
  
  add (*buttonBox);
  syncWithEvernoteButton->signal_clicked()
    .connect(sigc::mem_fun(*app->mainToolbar, &MainToolbar::syncButtonCallback));
  dontSyncButton->signal_clicked()
    .connect(sigc::mem_fun(*this, &WelcomeWindow::dontSyncCallback));

  show_all_children ();
}

WelcomeWindow::~WelcomeWindow() {}

void WelcomeWindow::dontSyncCallback () {
		app->remove ();
		app->add (*app->windowBody);
		app->show_all ();
}

