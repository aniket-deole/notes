#include <iostream>

#include "notify.hh"

Notify::Notify() {
	/** Window Related Properties **/
	set_default_size (1280, 720);

    windowBody = Gtk::manage (new WindowBody (false, 0, Gtk::PACK_SHRINK));

	add(*windowBody);
	show_all ();
	gdk_window_set_decorations (gtk_widget_get_window ((GtkWidget*) gobj ()), GDK_DECOR_BORDER);

    windowBody->setApp (this);

    setupDatabase ();
}

void Notify::setupDatabase () {
    db = 0;
    if (sqlite3_open ("notify.db", &db) != SQLITE_OK) {
	    /* Report ERROR */
	    std::cout << "Debug.5: Error Opening db." << std::endl;
	    exit (-1);
	}

	if (sqlite3_exec (db, "select * from notebooks", dummy_callback, 0, NULL) != SQLITE_OK) {
	    /* DB is not setup. This is the first run. Or the file 
	     * has been corrupted. */
	    sqlite3_exec (db, "CREATE TABLE notebooks (id int primary key, title text, parent_id int)", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE notes (id int primary key, title text, body text, notebook_id int, created_time datetime, modified_time datetime);", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE notes_tags_xref (note_id int, tag_id int);", NULL, 0, NULL);
	    sqlite3_exec (db, "CREATE TABLE tags (id int primary key, title text);", NULL, 0, NULL);

	    sqlite3_exec (db, "INSERT INTO notebooks values (0,'All Notebooks', 0)", NULL, 0, NULL);
	    sqlite3_exec (db, "INSERT INTO notebooks values (0,'All Tags')", NULL, 0, NULL);
	    sqlite3_exec (db, "COMMIT", NULL, NULL, NULL);
	}

}

Notify::~Notify () {}

void Notify::on_button_clicked () {
	std::cout << "HelloWorld!" << std::endl;
}

