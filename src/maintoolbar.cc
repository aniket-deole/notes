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
#include <pthread.h>
#include <iostream>

#include <maintoolbar.hh>
#include <webviewpopup.hh>

std::string MainToolbar::headerBarSubStatus = "Connected to Evernote";

MainToolbar::MainToolbar () {
  connectedToEvernote = false;
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

  syncButton = Gtk::manage (new Gtk::Button ("Sync With Evernote"));
  syncButton->signal_clicked ().connect (
      sigc::mem_fun (*this, &MainToolbar::syncButtonCallback));

  collapsedHeaderBar = false;

  collapseHeaderBar = Gtk::manage (new Gtk::Button ());
  collapseHeaderBar->set_image_from_icon_name ("zoom-out",  Gtk::ICON_SIZE_SMALL_TOOLBAR);
  collapseHeaderBar->signal_clicked().connect (
      sigc::mem_fun(*this, &MainToolbar::toggleHeaderBarCallback));
  addCss (collapseHeaderBar, "collapseHeaderBar", " .collapseHeaderBar {\n  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px; -unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
  addCss (syncButton, "syncButton", " .syncButton {\n  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px; -unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");


  progressBar = Gtk::manage (new Gtk::ProgressBar ());
  progressBar->set_hexpand (true);

  pack_end (*collapseHeaderBar);
  pack_end (*syncButton);

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

	evernoteConnectionInProgress = false;

  set_show_close_button(true);
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
    app->npv->toolbarBoxUpper->show ();
    syncButton->show ();
    collapseHeaderBar->set_image_from_icon_name ("zoom-out",  Gtk::ICON_SIZE_SMALL_TOOLBAR);
  } else {
    sc->remove_class("header-bar");
    collapsedHeaderBar = true;
    newNoteButton->hide ();
    newNotebookButton->hide ();
    app->lpv->hide();
    app->nlpv->hide();
    app->npv->toolbarBoxUpper->hide ();
    searchEntry->hide ();
    syncButton->hide ();
    collapseHeaderBar->set_image_from_icon_name ("zoom-in",  Gtk::ICON_SIZE_SMALL_TOOLBAR);
  }
}

void* MainToolbar::asynchronousSync (void* data) {
  MainToolbar* d = static_cast<MainToolbar*>(data);
  d->app->sm->sync ();
  d->progressBarStarted = false;
  return NULL;
}

int MainToolbar::checkAuthTokenCallback (void* p, int argc, char** argv, char** azColName) {
	MainToolbar* mt = (MainToolbar*) p;

	if (argc == 1) {
		mt->tempAuthToken = argv[0];
	}

	return 0;
}


void MainToolbar::syncButtonCallback () {
	
	tempAuthToken = "";
	app->dbm->exec ("select value from system_parameters where parameter='evernoteAuthToken';",
			&checkAuthTokenCallback, (void*) this);

	if (tempAuthToken.length () != 0) {
		connectedToEvernote = true;
		std::cout << "We already have a token." << std::endl;
	}
	
	if (evernoteConnectionInProgress) {
		evernoteConnectionInProgress = false;
		newNoteButton->show ();
		newNotebookButton->show ();
		searchEntry->show ();
		app->remove ();
		app->add (*app->windowBody);
		app->show_all ();

		syncButton->set_label ("Sync With Evernote");
		return;
	} else {
		if (!connectedToEvernote) {
		syncButton->set_label ("Cancel Sync");
		newNoteButton->hide ();
		newNotebookButton->hide ();
		searchEntry->hide ();
		evernoteConnectionInProgress = true;
		connectedToEvernote = false;
		}
	}
  if (!connectedToEvernote) {
    progressBar = Gtk::manage (new Gtk::ProgressBar ());
    progressBar->set_hexpand (true);

    set_custom_title (*progressBar);
    progressBarStarted = true;
    syncComplete = false;
//    progressBar->set_text (headerBarSubStatus);
//    set_subtitle (headerBarSubStatus);
    progressBar->set_show_text (true);
    progressBar->show_now ();
    Glib::signal_timeout().connect(sigc::mem_fun(*this,
          &MainToolbar::on_timeout), 50 );

			asynchronousSync (this);
  } else {
  
	} 
}

bool MainToolbar::on_timeout()
{
  if (progressBarStarted)
    progressBar->pulse();

  if (progressBarStarted)
    return true;
  else {
    property_custom_title () = 0;
    return false;
  }
}
