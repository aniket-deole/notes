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
#include <gtkmm/stylecontext.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/styleprovider.h>

#include "windowbody.hh"

void addCss (Gtk::Widget* widget, std::string cssClass, std::string css) {
	Glib::RefPtr<Gtk::StyleContext> context;
	context = widget->get_style_context ();
	widget->set_name (cssClass);
	Glib::RefPtr<Gtk::CssProvider> provider = Gtk::CssProvider::create ();

	provider->load_from_data (css);
	context->add_provider (provider, GTK_STYLE_PROVIDER_PRIORITY_SETTINGS);
	context->add_class (cssClass);
}

WindowBody::WindowBody (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {

	set_orientation (Gtk::ORIENTATION_VERTICAL);

//	add (*mainToolbar);

    Gtk::Box* body = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, 0));

    Gtk::Paned* paneOne = Gtk::manage (new Gtk::Paned (Gtk::ORIENTATION_HORIZONTAL));
    addCss (paneOne, "paneOne", ".paneOne{ -GtkPaned-handle-size: 1px;}");

    lpv = new LeftPaneView (false, 0, Gtk::PACK_SHRINK,0);
    paneOne->pack1 (*lpv, false, false);

    Gtk::Box* rightFrameOfPaneOne = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, 0));
    Gtk::Paned* paneTwo = Gtk::manage (new Gtk::Paned (Gtk::ORIENTATION_HORIZONTAL));

    nlpv = new NoteListPaneView (false, 0, Gtk::PACK_SHRINK, 0);
    paneTwo->pack1 (*nlpv, false, false);

    npv = new NotePaneView (false, 0, Gtk::PACK_SHRINK, 0);
    paneTwo->pack2 (*npv, true, false);

    rightFrameOfPaneOne->pack_start (*paneTwo, true, true, 0);

    paneOne->pack2 (*rightFrameOfPaneOne, true, false);

    body->pack_start (*paneOne, true, true, 0);

    pack_end (*body, Gtk::PACK_EXPAND_WIDGET, 0);
  	show_all ();

}

WindowBody::~WindowBody () {

}

void WindowBody::setApp (Notify* a) {
    app = a;
    app->lpv = lpv;
    app->nlpv = nlpv;
    app->npv = npv;
    nlpv->setApp (app);
    lpv->setApp (app);
    npv->setApp (app);
}

void  WindowBody::setDatabaseManager (DatabaseManager* d) {
    dbm = d;
    lpv->setDatabaseManager (d);
    nlpv->setDatabaseManager (d);
    npv->setDatabaseManager (d);
    lpv->dbInitialized = true;
}
