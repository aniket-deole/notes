#ifndef _WINDOWBODY_HH_
#define _WINDOWBODY_HH_

#include <gtkmm.h>

#include "notify.hh"
#include "maintoolbar.hh"
#include "leftpaneview.hh"
#include "notelistpaneview.hh"
#include "notepaneview.hh"

class Notify;

void addCss (Gtk::Widget* widget, std::string cssClass, std::string css);

class WindowBody : public Gtk::Box {
private:
    int a;
    MainToolbar mainToolbar;

    Notify* app;
    LeftPaneView* lpv;
    NoteListPaneView* nlpv;
    NotePaneView* npv;
public:
    WindowBody (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0);
    ~WindowBody ();

    void setApp (Notify* a);
};

#endif
