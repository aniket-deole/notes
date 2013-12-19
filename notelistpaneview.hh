/***
Copyright (C) 2013 Aniket Deole <aniket.deole@gmail.com>
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License version 2.1, as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranties of
MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef _NOTELISTPANEVIEW_HH_
#define _NOTELISTPANEVIEW_HH_

#include <gtkmm.h>
#include "notedata.hh"
#include "databasemanager.hh"
#include "notify.hh"

class Notify;
class DatabaseManager;

class NoteListPaneView : public Gtk::Box {
private:
	int a;

	Gtk::TreeView* treeView;
  DatabaseManager* dbm;
  Notify* app;

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

  void setDatabaseManager (DatabaseManager* d);
  static int fillNotesCallback (void*,int,char**,char**);
  void fetchNotesForNotebook (int primaryKey);
  void on_treeview_row_activated (const Gtk::TreePath&, Gtk::TreeViewColumn* const&);
  void on_treeview_row_changed ();

  void setApp (Notify* a) { app = a; }

};

#endif
