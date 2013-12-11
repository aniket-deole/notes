#ifndef _NOTIFY_HH_
#define _NOTIFY_HH_

#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include "windowbody.hh"

class Notify : public Gtk::Window {
public:
	Notify ();
	virtual ~Notify ();

	LeftPaneView* lfp;
	NoteListPaneView* nlpv;
	NoteView

protected:
	void on_button_clicked();
	Gtk::Button m_button;

private:
    WindowBody* windowBody;
};

#endif

