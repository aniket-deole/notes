#ifndef _LEFTPANEVIEW_HH_
#define _LEFTPANEVIEW_HH_

#include <gtkmm.h>

class LeftPaneView : public Gtk::Box {
private:
	int a;

	Gtk::TreeView* treeView;

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

};

#endif
