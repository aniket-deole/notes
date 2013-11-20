#ifndef _LEFTPANE_HH_
#define _LEFTPANE_HH_

#include <gtkmm.h>

class LeftPane : public Gtk::Box {
private:
    int a;
public:
    LeftPane (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0);
    ~LeftPane ();
};

#endif
