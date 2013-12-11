#ifndef _NOTIFY_HH_
#define _NOTIFY_HH_

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <sqlite3.h>

#include "windowbody.hh"
#include "leftpaneview.hh"
#include "notelistpaneview.hh"
#include "notepaneview.hh"

class WindowBody;

class Notify : public Gtk::Window {
public:
	Notify ();
	virtual ~Notify ();

	LeftPaneView* lpv;
	NoteListPaneView* nlpv;
	NotePaneView* npv;

	sqlite3 *db;


protected:
	void on_button_clicked();
	Gtk::Button m_button;

private:
    WindowBody* windowBody;
    void setupDatabase ();
};

#endif

