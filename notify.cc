#include <iostream>

#include "notify.hh"

Notify::Notify() {
	/** Window Related Properties **/
	set_default_size (1280, 720);

    windowBody = Gtk::manage (new WindowBody (false, 0, Gtk::PACK_SHRINK));

	add(*windowBody);
	show_all ();
	gdk_window_set_decorations (gtk_widget_get_window ((GtkWidget*) gobj ()), GDK_DECOR_BORDER);

    dbm = new DatabaseManager (this);

    windowBody->setApp (this);
    windowBody->setDatabaseManager (dbm);
}

Notify::~Notify () {}

void Notify::on_button_clicked () {
	std::cout << "HelloWorld!" << std::endl;
}

