#ifndef _NOTEVIEW_HH_
#define _NOTEVIEW_HH_

#include <gtkmm.h>
#include <webkit/webkit.h>
#include "notedata.hh"
#include "databasemanager.hh"

class DatabaseManager;
class LeftPaneView;

class NotePaneView : public Gtk::Box {
private:
	int a;
  WebKitWebView* webview;

  Gtk::ScrolledWindow* webviewWrapper;

  DatabaseManager* dbm;

public:
	NotePaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0);
	~NotePaneView ();

	void setDatabaseManager (DatabaseManager* d);
};
#endif
