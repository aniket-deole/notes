#include <gtkmm/box.h>
#include <gtkmm/widget.h>

#include "leftpane.hh"


WindowBody::WindowBody (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {

  set_orientation (Gtk::ORIENTATION_VERTICAL);

  add (mainToolbar);

// add (leftPane);
// add (midPane);
// add (rightPane);



  show_all ();
}

WindowBody::~WindowBody () {

}
