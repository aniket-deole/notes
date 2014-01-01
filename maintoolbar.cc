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

#include <gtkmm/box.h>
#include <gtkmm/widget.h>

#include <iostream>

#include "maintoolbar.hh"


MainToolbar::MainToolbar () {

 //Create the toolbar and add it to a container widget:
  Gtk::ToolButton* button = Gtk::manage(new Gtk::ToolButton());
  // TODO Change this later to set_icon_widget
//  button->set_icon_name("dialog-cancel");
  button->set_label ("Close");
  button->signal_clicked().connect(sigc::mem_fun(*this,
              &MainToolbar::exitNotify) );
  add(*button);

  Gtk::SeparatorToolItem* sti = Gtk::manage (new Gtk::SeparatorToolItem ());
  sti->set_expand (false);
  sti->set_draw (true);
  add (*sti);

  button = Gtk::manage(new Gtk::ToolButton());
  button->set_size_request (40, 40);
  button->set_label ("New Note");
  button->signal_clicked().connect(sigc::mem_fun(*this,
              &MainToolbar::newNote) );
  add(*button);

  button = Gtk::manage(new Gtk::ToolButton());
  button->set_size_request (40, 40);
  button->set_label ("New Notebook");
  button->signal_clicked().connect(sigc::mem_fun(*this,
              &MainToolbar::newNotebook) );
  add(*button);

  Gtk::ToolItem* ti = Gtk::manage (new Gtk::ToolItem ());
  ti->set_expand (true);
  add (*ti);

  Gtk::ToolItem* searchEntryContainer = Gtk::manage (new Gtk::ToolItem ());

  searchEntry = Gtk::manage (new Gtk::Entry ());
  searchEntry->set_text ("");
  searchEntry->set_icon_from_icon_name ("system-search");
  searchEntry->signal_changed ().connect (sigc::mem_fun (*this, 
            &MainToolbar::searchCallback));
  searchEntry->signal_activate ().connect (sigc::mem_fun (*this,
            &MainToolbar::searchEntryClicked));
  searchEntryActive = false;
  addCss (searchEntry, "searchEntry", ".searchEntry { color: #888; \n}\n");
  searchEntryContainer->add (*searchEntry);
  add (*searchEntryContainer);

  button = Gtk::manage(new Gtk::ToolButton());
  button->set_size_request (40, 40);
  button->set_label ("Maximize");
  add (*button);

	Glib::RefPtr<Gtk::StyleContext> sc = get_style_context(); 
	sc->add_class("primary-toolbar");

  show_all ();
}

MainToolbar::~MainToolbar () {

}

void MainToolbar::exitNotify () {
  exit (0);
}

void MainToolbar::newNote () {
  app->nlpv->newNote ();
}

void MainToolbar::newNotebook () {
  app->lpv->newNotebook ();
}

void MainToolbar::searchCallback () {
  if (searchEntry->get_text().empty ()){
    return;
  }
  app->nlpv->noteSearch (searchEntry->get_text ());
}

void MainToolbar::searchEntryClicked () {
  if (!searchEntryActive) {
   searchEntry->set_text ("");
   searchEntryActive = true;
  }
}