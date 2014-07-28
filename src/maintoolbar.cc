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

  newNoteButton = Gtk::manage (new Gtk::Button ());
  newNoteButton->set_image_from_icon_name ("document-new", Gtk::ICON_SIZE_LARGE_TOOLBAR);
  newNoteButton->signal_clicked ().connect (
    sigc::mem_fun (*this, &MainToolbar::newNote));
  pack_start (*newNoteButton);

  newNotebookButton = Gtk::manage (new Gtk::Button ());
  newNotebookButton->set_image_from_icon_name ("address-book-new", Gtk::ICON_SIZE_LARGE_TOOLBAR);
  newNotebookButton->signal_clicked ().connect (
    sigc::mem_fun (*this, &MainToolbar::newNotebook));
  pack_start (*newNotebookButton);

  syncButton = Gtk::manage (new Gtk::Button ());
  syncButton->set_image_from_icon_name ("address-book-new", Gtk::ICON_SIZE_LARGE_TOOLBAR);
  syncButton->signal_clicked ().connect (
    sigc::mem_fun (*this, &MainToolbar::syncButtonCallback));
  
  collapsedHeaderBar = false;

  collapseHeaderBar = Gtk::manage (new Gtk::Button ());
  collapseHeaderBar->set_image_from_icon_name ("zoom-out",  Gtk::ICON_SIZE_SMALL_TOOLBAR);
  collapseHeaderBar->signal_clicked().connect (
      sigc::mem_fun(*this, &MainToolbar::toggleHeaderBarCallback));
  addCss (collapseHeaderBar, "collapseHeaderBar", " .collapseHeaderBar {\n  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px; -unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
  addCss (syncButton, "syncButton", " .syncButton {\n  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px; -unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");

  pack_end (*collapseHeaderBar);
//  pack_end (*syncButton);

  searchEntry = Gtk::manage (new Gtk::Entry ());
  searchEntry->set_text ("");
  searchEntry->set_icon_from_icon_name ("edit-find");
  searchEntry->signal_changed ().connect (sigc::mem_fun (*this, 
            &MainToolbar::searchCallback));
  searchEntry->signal_activate ().connect (sigc::mem_fun (*this,
            &MainToolbar::searchEntryClicked));
  searchEntryActive = false;
  addCss (searchEntry, "searchEntry", ".searchEntry { color: #888; \n}\n");
  pack_end (*searchEntry);

	Glib::RefPtr<Gtk::StyleContext> sc = get_style_context(); 
	sc->add_class("primary-toolbar");

  set_show_close_button(true);
  set_subtitle ("Connected To Evernote");
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

void MainToolbar::toggleHeaderBarCallback () {
  Glib::RefPtr<Gtk::StyleContext> sc = get_style_context(); 
  if (collapsedHeaderBar == true) {
    sc->add_class("header-bar");
    collapsedHeaderBar = false;
    searchEntry->show ();
    newNoteButton->show ();
    newNotebookButton->show ();
		app->lpv->show();
		app->nlpv->show();
    collapseHeaderBar->set_image_from_icon_name ("zoom-out",  Gtk::ICON_SIZE_SMALL_TOOLBAR);
  } else {
    sc->remove_class("header-bar");
    collapsedHeaderBar = true;
    newNoteButton->hide ();
    newNotebookButton->hide ();
		app->lpv->hide();
		app->nlpv->hide();
    searchEntry->hide ();
    collapseHeaderBar->set_image_from_icon_name ("zoom-in",  Gtk::ICON_SIZE_SMALL_TOOLBAR);
  }
}

void MainToolbar::syncButtonCallback () {

  app->sm->sync ();
  Gtk::MessageDialog* popup = new Gtk::MessageDialog (*app, "Sync Complete", true, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
  Gtk::Box* contentBox = popup->get_content_area ();
  popup->set_resizable (false);
  popup->set_modal (true);
  int reply = popup->run ();
  popup->hide ();
  delete popup;
}
