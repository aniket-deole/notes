#ifndef _NOTELISTPANEVIEW_HH_
#define _NOTELISTPANEVIEW_HH_

#include <gtkmm.h>
#include "notedata.hh"

class NoteListPaneView : public Gtk::Box {
private:
	int a;

	Gtk::TreeView* treeView;

public:
    NoteListPaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0);
    ~NoteListPaneView ();


  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_id); add(m_col_name); add(m_note_data);}

    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<NoteData> m_note_data;
  };

  ModelColumns m_Columns;

  
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;
void treeviewcolumn_validated_on_cell_data(
        Gtk::CellRenderer* /* renderer */,
        const Gtk::TreeModel::iterator& iter);
};

#endif
