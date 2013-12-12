#include <iostream>

#include <gtkmm/box.h>
#include <gtkmm/widget.h>

#include "leftpaneview.hh"
#include "windowbody.hh"

LeftPaneView::LeftPaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {

  set_orientation (Gtk::ORIENTATION_VERTICAL);

  set_size_request (200, -1);

  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
  m_ScrolledWindow.add(m_TreeView);
  std::string cssProperties = ".m_TreeView { background-color: #34393D;     border-radius: 0;     color: white; } "
" .m_TreeView:selected, .m_TreeView:prelight:selected, .m_TreeView.category-expander:hover {      color: white;     border-style: solid;     border-width: 1px 0 1px 0; "
"  		-unico-inner-stroke-width: 1px 0 1px 0; background-image: -gtk-gradient (linear, left top, left bottom, from (alpha (#000, 0.11)), to (alpha (#000, 0.07)));     " 
"		-unico-border-gradient: -gtk-gradient (linear,                     left top, left bottom,                     "
" 		from (alpha (#fff, 0.070)), "
"		to (alpha (#fff, 0.10)));      -unico-inner-stroke-gradient: -gtk-gradient (linear,                     left top, left bottom, "
"		from (alpha (#000, 0.03)),                     to (alpha (#000, 0.10))); }  "
"	.m_TreeView:selected:backdrop, .m_TreeView:prelight:selected:backdrop {     background-image: -gtk-gradient (linear,"
"		left top,                     left bottom,                     from (alpha (#000, 0.08)),                     to (alpha (#000, 0.04)));      -unico-border-gradient: -gtk-gradient (linear,       "
"		left top, left bottom,                     from (alpha (#000, 0.19)),                     to (alpha (#fff, 0.25)));      "
"		-unico-inner-stroke-gradient: -gtk-gradient (linear,                     left top, left bottom,                     from (alpha (#000, 0.03)),                     to (alpha (#000, 0.10)));  } "
"	.m_TreeView:prelight { background-color: shade (@bg_color, 1.10); }"
"	.m_TreeView:hover { color: white; border-style: solid; border-width: 1px 0 1px 0; -unico-inner-stroke-width: 1px 0 1px 0; "
"		background-image: -gtk-gradient (linear, left top, left bottom, from (alpha (#FFF, 0.2)), to (alpha (#FFF, 0.2))); "
"		-unico-border-gradient: -gtk-gradient (linear, left top, left bottom, from (alpha (#fff, 0.3)), to (alpha (#fff, 0.30)));}";

  addCss (&m_TreeView, "m_TreeView", cssProperties);

  //Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  pack_start(m_ScrolledWindow);

  //Create the Tree model:
  m_refTreeModel = Gtk::TreeStore::create(m_Columns);
  m_TreeView.set_model(m_refTreeModel);
  m_TreeView.set_headers_visible (false);
  m_TreeView.set_show_expanders (false);
  //All the items to be reordered with drag-and-drop:
  m_TreeView.set_reorderable(false);

  //Fill the TreeView's model
  notebooksRow = *(m_refTreeModel->append());
  notebooksRow[m_Columns.m_col_id] = 0;
  notebooksRow[m_Columns.m_col_name] = "Notebooks";


  tagsRow = *(m_refTreeModel->append());
  tagsRow[m_Columns.m_col_id] = 2;
  tagsRow[m_Columns.m_col_name] = "Tags";
/*
  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_id] = 13;
  childrow[m_Columns.m_col_name] = "\tTag1";

  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_id] = 14;
  childrow[m_Columns.m_col_name] = "\tTag2";
*/

  //Add the TreeView's view columns:
  m_TreeView.append_column("Name", m_Columns.m_col_name);

  //Connect signal:
  m_TreeView.signal_row_expanded().connect(sigc::mem_fun(*this,
              &LeftPaneView::on_treeview_row_expanded) );

  m_TreeView.signal_row_activated().connect(sigc::mem_fun(*this,
              &LeftPaneView::on_treeview_row_activated) );

  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();

  ts->signal_changed().connect(sigc::mem_fun(*this,
              &LeftPaneView::on_treeview_row_changed) );

  /* Select the All Notes item by default */ 
  
  show_all ();
}

void LeftPaneView::on_treeview_row_expanded (const Gtk::TreeModel::iterator& iter, Gtk::TreeModel::Path path){
  std::cout << "TreeView Row Expanded" << std::endl;
}

void LeftPaneView::on_treeview_row_activated (const Gtk::TreePath& tp, Gtk::TreeViewColumn* const& tvc){
  std::cout << "TreeView Row Activated" << std::endl;
}

void LeftPaneView::on_treeview_row_changed () {
  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();
  Gtk::TreeModel::iterator iter = ts->get_selected ();
  Glib::RefPtr<Gtk::TreeModel> tm = ts->get_model ();

  if (iter) {
    Gtk::TreeModel::Path path = tm->get_path (iter);
    if (path.size () == 1) {
      if (notebookListSelected && path[0] == 0)
        ts->unselect_all ();
      else
        ts->select (Gtk::TreeModel::Path (selectedPath));
      
      /* Expand tree */
      if (m_TreeView.row_expanded (path)) {
        m_TreeView.collapse_row (path);
      } else {
        m_TreeView.expand_to_path (path);
        ts->select (Gtk::TreeModel::Path (selectedPath));
      }
    } else {
      std::cout << "Tree Child Clicked." << std::endl;
      selectedPath = tm->get_string (iter);
      if (path[0] == 1)
        notebookListSelected = false;
      else
        notebookListSelected = true;

      /* Callback to fill up the notelistpane. */
    }
    std::cout << "LeftPaneView::on_treeview_row_changed getString (): " << tm->get_string (iter)  << std::endl;
  }
}

LeftPaneView::~LeftPaneView () {

}

void LeftPaneView::setDatabaseManager (DatabaseManager* d) {
  dbm = d;
  dbm->exec ("select * from notebooks", &fillNotebooksCallback,this);
  dbm->exec ("select * from tags", &fillTagsCallback,this);
  m_TreeView.expand_all ();
  m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0:0"));
  selectedPath = "0:0";
  notebookListSelected = true;
}

int LeftPaneView::fillNotebooksCallback (void* lpv, int argc, char **argv, char **azColName) {
  std::cout << "LeftPaneView::fillNotebooksCallback" << std::endl;
  LeftPaneView* p = (LeftPaneView*) lpv;

  std::string notebookName = "\t";
  notebookName += argv[1];

  Gtk::TreeModel::Row childrow = *(p->m_refTreeModel->append(p->notebooksRow.children()));
  childrow[p->m_Columns.m_col_id] = 11;
  childrow[p->m_Columns.m_col_name] = notebookName;
  
  return 0;
}

int LeftPaneView::fillTagsCallback (void* lpv, int argc, char **argv, char **azColName) {
  std::cout << "LeftPaneView::fillTagsCallback" << std::endl;
  LeftPaneView* p = (LeftPaneView*) lpv;

  std::string tagName = "\t";
  tagName += argv[1];

  Gtk::TreeModel::Row childrow = *(p->m_refTreeModel->append(p->tagsRow.children()));
  childrow[p->m_Columns.m_col_id] = 4;
  childrow[p->m_Columns.m_col_name] = tagName;
  
  return 0;
}