#include <gtkmm/box.h>
#include <gtkmm/widget.h>

#include "windowbody.hh"


WindowBody::WindowBody (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {

  set_orientation (Gtk::ORIENTATION_VERTICAL);

  add (mainToolbar);

// add (leftPane);
// add (midPane);
// add (rightPane);
/*
    Gtk::Box* body = Gtk::manage(new Gtk::Box());
    Gtk::Widget* hpaned = Gtk::manage (new Gtk::Paned ());
    Gtk::EventBox* frame1 = Gtk::manage (new Gtk::EventBox ());

    add (body);
*/
  show_all ();
}

WindowBody::~WindowBody () {

}
