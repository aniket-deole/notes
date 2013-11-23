#include <gtkmm/box.h>
#include <gtkmm/widget.h>
#include <gtkmm/stylecontext.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/styleprovider.h>

#include "windowbody.hh"
#include "leftpaneview.hh"
#include "notelistpaneview.hh"

void addCss (Gtk::Widget* widget, std::string cssClass, std::string css) {
	Glib::RefPtr<Gtk::StyleContext> context;
	context = widget->get_style_context ();
	widget->set_name (cssClass);
	Glib::RefPtr<Gtk::CssProvider> provider = Gtk::CssProvider::create ();

	provider->load_from_data (css);
	context->add_provider (provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	context->add_class (cssClass);
}


WindowBody::WindowBody (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {

	set_orientation (Gtk::ORIENTATION_VERTICAL);

	add (mainToolbar);

    Gtk::Box* body = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, 0));

    Gtk::Paned* paneOne = Gtk::manage (new Gtk::Paned (Gtk::ORIENTATION_HORIZONTAL));
    addCss (paneOne, "paneOne", ".paneOne{ -GtkPaned-handle-size: 1px;}");

    LeftPaneView* lpv = new LeftPaneView (false, 0, Gtk::PACK_SHRINK,0);
    paneOne->pack1 (*lpv, false, false);
//    addCss (leftPaneEventBox, "leftPaneEventBox", ".leftPaneEventBox { background-color: #34393D;}");

    Gtk::Box* rightFrameOfPaneOne = Gtk::manage (new Gtk::Box (Gtk::ORIENTATION_HORIZONTAL, 0));
    Gtk::Paned* paneTwo = Gtk::manage (new Gtk::Paned (Gtk::ORIENTATION_HORIZONTAL));

    NoteListPaneView* noteListPaneView = new NoteListPaneView (false, 0, Gtk::PACK_SHRINK, 0);
//    Gtk::EventBox* middlePaneEventBox = Gtk::manage (new Gtk::EventBox ());
//    middlePaneEventBox->set_size_request (300, -1);
    paneTwo->pack1 (*noteListPaneView, false, false);
 //   addCss (middlePaneEventBox, "middlePaneEventBox", ".middlePaneEventBox { background-color: white;}");

    Gtk::EventBox* rightPaneEventBox = Gtk::manage (new Gtk::EventBox ());
    paneTwo->pack2 (*rightPaneEventBox, true, false);
    addCss (rightPaneEventBox, "rightPaneEventBox", ".rightPaneEventBox { background-color: #DDD;}");

    rightFrameOfPaneOne->pack_start (*paneTwo, true, true, 0);

    paneOne->pack2 (*rightFrameOfPaneOne, true, false);

    body->pack_start (*paneOne, true, true, 0);

    pack_end (*body, Gtk::PACK_EXPAND_WIDGET, 0);
  	show_all ();
}

WindowBody::~WindowBody () {

}
