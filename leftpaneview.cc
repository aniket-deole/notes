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
  Gtk::TreeModel::Row row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 1;
  row[m_Columns.m_col_name] = "Notebooks";

  Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_id] = 11;
  childrow[m_Columns.m_col_name] = "Literature";

  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_id] = 12;
  childrow[m_Columns.m_col_name] = "HowTos";

  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 2;
  row[m_Columns.m_col_name] = "Tags";

  //Add the TreeView's view columns:
  m_TreeView.append_column("Name", m_Columns.m_col_name);

  //Connect signal:
//  m_TreeView.signal_row_activated().connect(sigc::mem_fun(*this,
//              &ExampleWindow::on_treeview_row_activated) );

  show_all ();
}

LeftPaneView::~LeftPaneView () {

}
