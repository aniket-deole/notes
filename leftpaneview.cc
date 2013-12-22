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

#include <iostream>

#include <gtkmm.h>

#include "leftpaneview.hh"
#include "windowbody.hh"

#include "notebookdata.hh"

class NotebookCellRenderer : public Gtk::CellRenderer {
  public:
  Glib::PropertyProxy< int > property_id()
  {
    return property_id_.get_proxy();
  }
  
  Glib::PropertyProxy<NotebookData> property_note()
  {
    return property_notebook_.get_proxy();
  }

  NotebookCellRenderer () :
    Glib::ObjectBase( typeid(NotebookCellRenderer) ),
    Gtk::CellRenderer(), 
    property_id_(*this, "id"),
    property_notebook_(*this, "notebook") {
    set_fixed_size (-1, 20);
  }

  Pango::Rectangle* renderNotebook (const ::Cairo::RefPtr< ::Cairo::Context >& cr, Gtk::Widget& widget, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Pango::Rectangle* pr, int id) {
    Pango::FontDescription font_from;
    font_from.set_size (10 * Pango::SCALE);
    if (property_notebook_.get_value ().getPrimaryKey () < 0) {
      font_from.set_weight (Pango::WEIGHT_SEMIBOLD);
      cr->move_to (10, cell_area.get_y () + 3);
    } else {
      font_from.set_weight (Pango::WEIGHT_NORMAL);
      cr->move_to (25, cell_area.get_y () + 3);
    }
    Glib::RefPtr <Pango::Layout> layout_from = widget.create_pango_layout ("");
    layout_from->set_font_description (font_from);
    layout_from->set_markup ("<span foreground='#FFF'>" + property_notebook_.get_value ().getTitle () + "</span>");
    layout_from->set_width(210 * Pango::SCALE);
    layout_from->show_in_cairo_context (cr);
    return pr;
  }

  Glib::Property< int > property_id_;
  Glib::Property< NotebookData > property_notebook_;
  
  protected:
   virtual void render_vfunc (const ::Cairo::RefPtr< ::Cairo::Context >& cr, Gtk::Widget& widget, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags) {
    Pango::Rectangle* pr = new Pango::Rectangle ();
    renderNotebook (cr, widget, background_area, cell_area, pr, property_id_);
  }
};

static Gtk::TreeViewColumn* create_column (Gtk::TreeModelColumn<int> tmc, Gtk::TreeModelColumn<NotebookData> n) {
  NotebookCellRenderer* ncr = new NotebookCellRenderer ();
  Gtk::TreeViewColumn* c = Gtk::manage (new Gtk::TreeViewColumn ("Notebooks", *ncr));
  c->add_attribute(*ncr, "id", tmc);
  c->add_attribute(*ncr, "notebook", n);
  return c;
}

LeftPaneView::LeftPaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {
  dbInitialized = false;
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
"		-unico-border-gradient: -gtk-gradient (linear, left top, left bottom, from (alpha (#fff, 0.3)), to (alpha (#fff, 0.30)));}"
"  .m_TreeView row:nth-child(odd) { color : red; } row:nth-child(even) { color : green; }";

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
  NotebookData* nbd = new NotebookData (-1, "Notebooks");
  notebooksRow[m_Columns.m_notebook_data] = *nbd;

  tagsRow = *(m_refTreeModel->append());
  tagsRow[m_Columns.m_col_id] = 2;
  tagsRow[m_Columns.m_col_name] = "Tags";
  nbd = new NotebookData (-1, "Tags");
  tagsRow[m_Columns.m_notebook_data] = *nbd;
/*
  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_id] = 13;
  childrow[m_Columns.m_col_name] = "\tTag1";

  childrow = *(m_refTreeModel->append(row.children()));
  childrow[m_Columns.m_col_id] = 14;
  childrow[m_Columns.m_col_name] = "\tTag2";
*/

  //Add the TreeView's view columns:
//  m_TreeView.append_column("Name", m_Columns.m_col_name);

  m_TreeView.append_column(*create_column (m_Columns.m_col_id, m_Columns.m_notebook_data));

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
  if (!dbInitialized)
    return;

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
      Gtk::TreeModel::Row row = *iter;
  //  app->npv->setWebViewContent (n.getSummary ());
      app->nlpv->fetchNotesForNotebook (row[m_Columns.m_col_id]);
      std::cout << "LeftPaneView::on_treeview_row_changed Name: " << row[m_Columns.m_col_id] << ", PKey: " << row[m_Columns.m_col_name] << std::endl;
    }
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

  std::string notebookName = "";
  int notebookId = atoi (argv[0]);
  notebookName += argv[1];

  NotebookData* nbd = new NotebookData (notebookId, notebookName);

  Gtk::TreeModel::Row childrow = *(p->m_refTreeModel->append(p->notebooksRow.children()));
  childrow[p->m_Columns.m_col_id] = notebookId;
  childrow[p->m_Columns.m_col_name] = notebookName;
  childrow[p->m_Columns.m_notebook_data] = *nbd;
  
  return 0;
}

int LeftPaneView::fillTagsCallback (void* lpv, int argc, char **argv, char **azColName) {
  std::cout << "LeftPaneView::fillTagsCallback" << std::endl;
  LeftPaneView* p = (LeftPaneView*) lpv;

  int tagId = atoi (argv[0]);

  std::string tagName = "";
  tagName += argv[1];
  NotebookData* nbd = new NotebookData (tagId, tagName);

  Gtk::TreeModel::Row childrow = *(p->m_refTreeModel->append(p->tagsRow.children()));
  childrow[p->m_Columns.m_col_id] = 4;
  childrow[p->m_Columns.m_col_name] = tagName;
  childrow[p->m_Columns.m_notebook_data] = *nbd;
  
  return 0;
}

void LeftPaneView::setApp (Notify* a) {
  app = a;
}

void LeftPaneView::newNotebook () {

/*
  Gtk::Window* popup = Gtk::manage (new Gtk::Window ());
  popup->set_default_size (640, 360);
  popup->show_all ();
*/
  popup = new Gtk::Dialog ("New Notebook", *app, true);
  std::string cssProperties = ".popup { background-color: #DDD; }";
  addCss (popup, "popup", cssProperties);

  Gtk::Box* contentBox = popup->get_content_area ();
  Gtk::Label* label = Gtk::manage (new Gtk::Label ("Enter Notebook Name: "));
  

  addCss (label, "label", ".label {\n color:#34393D;\n font: OpenSans light 12;"
                " background-image:none; background-color:white;\n}\n");

  contentBox->pack_start (*label, false, true, 0);

  notebookName = new Gtk::Entry ();
  notebookName->set_width_chars (5);
  contentBox->add (*notebookName);

  Gtk::Button* okButton = new Gtk::Button ("Ok");
  okButton->signal_clicked().connect(sigc::mem_fun(*this,
              &LeftPaneView::newNotebookOk) ); 
  contentBox->add (*okButton);

  contentBox->show_all ();
  popup->set_position (Gtk::WIN_POS_CENTER);
  popup->set_resizable (false);
  popup->get_content_area () ->set_margin_top (50);
  popup->run ();
}

void LeftPaneView::newNotebookOk () {
  if (notebookName->get_text().empty ()) { return;}
  popup->hide ();


  dbm->exec ("insert into notebooks values (NULL, '" + notebookName->get_text () + "', 0)", NULL,this);

  m_refTreeModel->clear ();

  //Fill the TreeView's model
  notebooksRow = *(m_refTreeModel->append());
  notebooksRow[m_Columns.m_col_id] = 0;
  notebooksRow[m_Columns.m_col_name] = "Notebooks";
  NotebookData* nbd = new NotebookData (-1, "Notebooks");
  notebooksRow[m_Columns.m_notebook_data] = *nbd;

  tagsRow = *(m_refTreeModel->append());
  tagsRow[m_Columns.m_col_id] = 2;
  tagsRow[m_Columns.m_col_name] = "Tags";
  nbd = new NotebookData (-1, "Tags");
  tagsRow[m_Columns.m_notebook_data] = *nbd;



  dbm->exec ("select * from notebooks where id = 0", &fillNotebooksCallback,this);
  dbm->exec ("select * from notebooks where id > 0 order by title", &fillNotebooksCallback,this);
  dbm->exec ("select * from tags", &fillTagsCallback,this);
  m_TreeView.expand_all ();
  m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0:0"));
  selectedPath = "0:0";
  notebookListSelected = true;
}