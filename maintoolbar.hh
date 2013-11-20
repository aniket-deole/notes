#ifndef _MAINTOOLBAR_HH_
#define _MAINTOOLBAR_HH_

#include <gtkmm/toolbar.h>

class MainToolbar : public Gtk::Toolbar {
private:
    int a;


public:
    MainToolbar ();
    ~MainToolbar ();
};

#endif
