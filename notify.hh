#ifndef _NOTIFY_HH_
#define _NOTIFY_HH_

#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include "windowbody.hh"
#include "leftpaneview.hh"
#include "notelistpaneview.hh"
#include "notepaneview.hh"

class Notify : public Gtk::Window {
public:
	Notify ();
	virtual ~Notify ();

	LeftPaneView* lfp;
	NoteListPaneView* nlpv;
	NotePaneView* nv;

protected:
	void on_button_clicked();
	Gtk::Button m_button;

private:
    WindowBody* windowBody;
};

#endif

