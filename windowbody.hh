#ifndef _WINDOWBODY_HH_
#define _WINDOWBODY_HH_

#include <gtkmm.h>

#include "maintoolbar.hh"

void addCss (Gtk::Widget* widget, std::string cssClass, std::string css);

class WindowBody : public Gtk::Box {
private:
    int a;
    MainToolbar mainToolbar;
public:
    WindowBody (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0);
    ~WindowBody ();
};

#endif
