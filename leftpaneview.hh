#ifndef _LEFTPANEVIEW_HH_
#define _LEFTPANEVIEW_HH_

#include <gtkmm.h>
#include "databasemanager.hh"

class DatabaseManager;

class LeftPaneView : public Gtk::Box {
private:
	int a;
  Gtk::TreeView* treeView;
  Gtk::TreeModel::Row notebooksRow;
  Gtk::TreeModel::Row tagsRow;
  void on_treeview_row_expanded (const Gtk::TreeModel::iterator& iter, Gtk::TreeModel::Path path);
  void on_treeview_row_activated (const Gtk::TreePath&, Gtk::TreeViewColumn* const&);
  void on_treeview_row_changed ();



  std::string selectedPath;
  bool notebookListSelected;

  DatabaseManager* dbm;

public:
    LeftPaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0);
    ~LeftPaneView ();


  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_id); add(m_col_name); }

    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  };

   ModelColumns m_Columns;

  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;

  void setDatabaseManager (DatabaseManager* d);
  static int fillNotebooksCallback (void*,int,char**,char**);
  static int fillTagsCallback (void*,int,char**,char**);
};

#endif
