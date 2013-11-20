#include <gtkmm/box.h>
#include <gtkmm/widget.h>

#include "maintoolbar.hh"


MainToolbar::MainToolbar () {

 //Create the toolbar and add it to a container widget:
  Gtk::ToolButton* button = Gtk::manage(new Gtk::ToolButton());
  button->set_size_request (40, 40);
  // TODO Change this later to set_icon_widget
  button->set_icon_name("dialog-cancel");

  add(*button);

  button = Gtk::manage(new Gtk::ToolButton());
  button->set_size_request (40, 40);
  button->set_icon_name("document-new");
  add(*button);

  button = Gtk::manage(new Gtk::ToolButton());
  button->set_size_request (40, 40);
  button->set_icon_name("address-book-new");
  add(*button);

  show_all ();
}

MainToolbar::~MainToolbar () {

}
