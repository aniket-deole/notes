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

#ifndef _MAINTOOLBAR_HH_
#define _MAINTOOLBAR_HH_

#include <gtkmm/toolbar.h>
#include "notify.hh"

class Notify;

class MainToolbar : public Gtk::HeaderBar {
private:
    int a;
    Notify* app;
    Gtk::Entry* searchEntry;

    bool searchEntryActive;
    bool collapsedHeaderBar;
    Gtk::Button* collapseHeaderBar;
public:
    MainToolbar ();
    ~MainToolbar ();
    void setApp (Notify* a) { app = a; }
    void exitNotify ();
    void newNote ();
    void newNotebook ();
    void searchCallback ();
    void searchEntryClicked ();
    void toggleHeaderBarCallback ();
};

#endif
