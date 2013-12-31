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
#include "notepaneview.hh"
/*
 * This files contains C Code as I was not able to find a good C++ gtkmm e
 * equivalent wrapper for webkit. Open to suggestions :)
 */

NotePaneView::NotePaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {
	set_orientation (Gtk::ORIENTATION_VERTICAL);

	noteTitle = Gtk::manage (new Gtk::Entry ());
	noteTitle->set_text ("NoteTitle");
	noteTitle->set_has_frame (false);
	noteTitle->set_editable (true);
	
	addCss (noteTitle, "noteTitle", ".noteTitle {\n color:#000;\n font: OpenSans light 18; padding-top:10px;padding-bottom:10px; "
								"padding-left:14px; background-image:none; background-color:white;\n}\n"
								".noteTitle:selected {    background-color: #34393D; color:white ; }");

	Gtk::EventBox* ev = Gtk::manage (new Gtk::EventBox ());

	Gtk::Box* noteTitleAndSaveButtonBox = Gtk::manage (new Gtk::Box ());

	saveButton = Gtk::manage (new Gtk::Button ("  Save Note  "));

  	saveButton->signal_clicked().connect(
    sigc::mem_fun(*this, &NotePaneView::saveNote) );

	noteTitleAndSaveButtonBox->pack_start (*noteTitle, true, true, 0);
	noteTitleAndSaveButtonBox->pack_end (*saveButton, false, false, 0);

	ev->add (*noteTitleAndSaveButtonBox);

	addCss (ev, "ev", ".ev { background-image:none; background-color:white;\n}\n");


	pack_start (*ev, false, false, 0);

	Gtk::Separator* sepBot = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_HORIZONTAL));
	pack_start (*sepBot, false, false, 0);



	webviewWrapper = Gtk::manage (new Gtk::ScrolledWindow ());

	webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
	
	GtkScrolledWindow* webviewWrapper_c = webviewWrapper->gobj ();
	gtk_container_add (GTK_CONTAINER (webviewWrapper_c), GTK_WIDGET (webview));

	webkit_web_view_set_editable(webview, true);
    
	webkit_web_view_load_string (webview, "No Notes :(", "text/html", NULL, NULL);

		webviewWrapper->set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	pack_start (*webviewWrapper);

	show_all ();
}

NotePaneView::~NotePaneView () {}


void NotePaneView::setDatabaseManager (DatabaseManager* d) {
    dbm = d;
}

void NotePaneView::setNote (NoteData n) {
	setWebViewContent (n.getBody ());
	setNoteTitleEntryText (n.getTitle ());
	nd = n;
}

void NotePaneView::setNoteTitleEntryText (std::string content) {
	noteTitle->set_text (content);
}

void NotePaneView::setWebViewContent (std::string content) {
	webkit_web_view_load_string (webview, content.c_str (), "text/html", NULL, NULL);
}

void NotePaneView::newNote () {
	webkit_web_view_load_string (webview, "", "text/html", NULL, NULL);
}

void NotePaneView::disableButtons () {
	saveButton->set_sensitive (false);
}

void NotePaneView::enableButtons () {
	saveButton->set_sensitive (true);
}

std::string
replaceSingleQuote( std::string const& original )
{
    std::string results(
        original.size() 
            + std::count( original.begin(), original.end(), '\''),
        '\'' );
    std::string::iterator dest = results.begin();
    for ( std::string::const_iterator current = original.begin();
            current != original.end();
            ++ current ) {
        if ( *current == '\'' ) {
            ++ dest;
        }
        *dest = *current;
        ++ dest;
    }
    return results;
}

template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}

void NotePaneView::saveNote () {

    std::string body = "";
	webkit_web_view_execute_script (webview, "document.title=document.documentElement.innerHTML;");
	body.append (webkit_web_frame_get_title ((webkit_web_view_get_main_frame (webview))));
	body = replaceSingleQuote (body);

	std::string title = replaceSingleQuote (noteTitle->get_text ());
	std::cout << "saved: " << "update notes set title = '" + title + "', body = '" + body + "' where id = " + NumberToString (nd.getPrimaryKey ()) 	<< std::endl;
  	dbm->exec ("update notes set title = '" + title + "', body = '" + body + "', modified_time = strftime('%s','now') where id = " + NumberToString (nd.getPrimaryKey ()), NULL, this);

  	app->nlpv->fetchNotesForNotebook (app->lpv->getSelectedNotebookId ());
}