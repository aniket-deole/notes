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

#ifndef _LEFTPANEVIEW_HH_
#define _LEFTPANEVIEW_HH_

#include <vector>

#include <gtkmm.h>
#include "databasemanager.hh"
#include "notebookdata.hh"

class DatabaseManager;
class Notify;
class LeftPaneView;

class NotebookTreeStore : public Gtk::TreeStore {
private:
  LeftPaneView* lpv;
protected:
  NotebookTreeStore();

public:

  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_id); add(m_col_name); add (m_notebook_data);  add(m_col_draggable); add(m_col_receivesdrags);
      add (m_col_is_stack); }

    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<NotebookData> m_notebook_data;
    Gtk::TreeModelColumn<bool> m_col_draggable;
    Gtk::TreeModelColumn<bool> m_col_receivesdrags;
    Gtk::TreeModelColumn<bool> m_col_is_stack;
  };

  ModelColumns m_Columns;

  static Glib::RefPtr<NotebookTreeStore> create(LeftPaneView* l);

protected:
  //Overridden virtual functions:
  virtual bool row_draggable_vfunc(const Gtk::TreeModel::Path& path) const;
  virtual bool row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data) const;

};

class LeftPaneView : public Gtk::Box {
private:
	int a;
  Gtk::TreeView* treeView_notebooks;
  Gtk::TreeView* treeView_tags;
  Gtk::TreeModel::Row notebooksRow;
  Gtk::TreeModel::Row tagsRow;
  void on_treeview_row_expanded (const Gtk::TreeModel::iterator& iter, Gtk::TreeModel::Path path);
  void on_treeview_row_activated (const Gtk::TreePath&, Gtk::TreeViewColumn* const&);
  void on_treeview_row_changed ();

  std::string selectedPath;
  bool notebookListSelected;

  Notify* app;
  DatabaseManager* dbm;

  int popupH, popupW;
  bool dragEnded;

  std::vector<std::string> stacks;

public:
    LeftPaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0, Notify* a = NULL, DatabaseManager* d= NULL);
    ~LeftPaneView ();

  NotebookData* notebookBeingDragged;
  NotebookData* notebookDestination;
  bool dbInitialized;

  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView_Notebooks;
  Glib::RefPtr<NotebookTreeStore> m_refTreeModel;

  void setDatabaseManager (DatabaseManager* d);
  void setApp (Notify* a);
  static int fillNotebooksCallback (void*,int,char**,char**);
  static int fillTagsCallback (void*,int,char**,char**);

  void newNotebook ();
  void newNotebookOk ();
  Gtk::Dialog* popup;
  Gtk::Entry* notebookName;
  void on_treeview_button_release_event (GdkEventButton* button);
  void on_menu_file_popup_edit_notebook_name();
  void on_menu_file_popup_delete_notebook ();
  Gtk::Menu m_Menu_Popup;
  void notebookEdit () ;
  NotebookData selectedNotebook;
  std::vector<std::string> selectedNotebookGuids;
  void notebookDelete ();
  void notebookDeleteCancel ();
  void selectNotebookInPane (int pathIndex);
  std::string getSelectedNotebookGuid () { return selectedNotebook.getGuid (); }
  void refreshLeftPaneView ();
  void nnDragFinished(const Glib::RefPtr< Gdk::DragContext >&   context);
  void nnDragStarted(const Glib::RefPtr< Gdk::DragContext >&   context);

  void updateParentGuid (std::string, std::string, std::string stack);

  void newNotebookOkButtonClicked ();
  void newNotebookCancelButtonClicked ();
};

#endif
