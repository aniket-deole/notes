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

#include <gtkmm/box.h>
#include <gtkmm/widget.h>
#include <gtkmm.h>
#include <iostream>
#include <cstring>
#include "notelistpaneview.hh"
#include "windowbody.hh"
#include "notedata.hh"

class NoteCellRenderer : public Gtk::CellRenderer {
  public:
  Glib::PropertyProxy< int > property_id()
  {
    return property_id_.get_proxy();
  }
  
  Glib::PropertyProxy<NoteData> property_note()
  {
    return property_note_.get_proxy();
  }

  NoteCellRenderer () :
    Glib::ObjectBase( typeid(NoteCellRenderer) ),
    Gtk::CellRenderer(), 
    property_id_(*this, "id"),
    property_note_(*this, "note") {
    set_fixed_size (-1, 65);
  }

  Pango::Rectangle* renderNote (const ::Cairo::RefPtr< ::Cairo::Context >& cr, Gtk::Widget& widget, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Pango::Rectangle* pr, int id) {
    Pango::FontDescription font_from;
    font_from.set_size (10 * Pango::SCALE);
    font_from.set_weight (Pango::WEIGHT_SEMIBOLD);
    Glib::RefPtr <Pango::Layout> layout_from = widget.create_pango_layout ("");
    layout_from->set_font_description (font_from);
    layout_from->set_markup ("<span foreground='#555'>" + property_note_.get_value ().getTitle () + "</span>");
    layout_from->set_width(210 * Pango::SCALE);
    cr->move_to (10, 5 + cell_area.get_y ());
    layout_from->show_in_cairo_context (cr);

    font_from.set_size (8 * Pango::SCALE);
    font_from.set_weight (Pango::WEIGHT_NORMAL);
    layout_from = widget.create_pango_layout ("");
    layout_from->set_font_description (font_from);
    layout_from->set_alignment(Pango::ALIGN_RIGHT);
    layout_from->set_markup ("<span foreground='#AAA'>" + property_note_.get_value ().getRemaining () + "</span>");
    layout_from->set_width(90* Pango::SCALE);
    cr->move_to (cell_area.get_width () - 90, 10 + cell_area.get_y ());
    layout_from->show_in_cairo_context (cr);

    font_from.set_size (10 * Pango::SCALE);
    font_from.set_weight (Pango::WEIGHT_LIGHT);
    layout_from = widget.create_pango_layout ("");
    layout_from->set_font_description (font_from);
    layout_from->set_alignment(Pango::ALIGN_LEFT);
    std::string summary = property_note_.get_value ().getSummary ();
    layout_from->set_markup ("<span foreground='#555'>" + summary.substr (0, (cell_area.get_width () / 4.0) + 10) + "</span>");
    layout_from->set_width((cell_area.get_width () - 10) * Pango::SCALE);
    cr->move_to (10, 27 + cell_area.get_y ());

    layout_from->show_in_cairo_context (cr);
    return pr;
  }

  Glib::Property< int > property_id_;
  Glib::Property< NoteData > property_note_;
  
  protected:
   virtual void render_vfunc (const ::Cairo::RefPtr< ::Cairo::Context >& cr, Gtk::Widget& widget, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags) {
    Pango::Rectangle* pr = new Pango::Rectangle ();
    renderNote (cr, widget, background_area, cell_area, pr, property_id_);
  }
};

class NotebookCellRenderer2 : public Gtk::CellRenderer {
  public:
  Glib::PropertyProxy< int > property_id()
  {
    return property_id_.get_proxy();
  }
  
  Glib::PropertyProxy<NotebookData> property_note()
  {
    return property_notebook_.get_proxy();
  }

  NotebookCellRenderer2 () :
    Glib::ObjectBase( typeid(NotebookCellRenderer2) ),
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

static Gtk::TreeViewColumn* create_column (Gtk::TreeModelColumn<int> tmc, Gtk::TreeModelColumn<NoteData> n) {
  NoteCellRenderer* ncr = new NoteCellRenderer ();
  Gtk::TreeViewColumn* c = Gtk::manage (new Gtk::TreeViewColumn ("Notes", *ncr));
  c->add_attribute(*ncr, "id", tmc);
  c->add_attribute(*ncr, "note", n);
  return c;
}


NoteListPaneView::NoteListPaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {

  set_orientation (Gtk::ORIENTATION_VERTICAL);

  set_size_request (300, -1);

  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
  m_ScrolledWindow.add(m_TreeView);
  std::string cssProperties = ".m_TreeView { background-color: white; border-radius: 0; border-color: #CCC; border-style: solid; border-width: 0px 0 1px 0; } "
  " .m_TreeView:selected, .m_TreeView:prelight:selected {      background-color: #EEE; }"
  " .m_TreeView:prelight { background-color: shade (@bg_color, 1.10); }"
  ;

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
  m_TreeView.append_column(*create_column (m_Columns.m_col_id, m_Columns.m_note_data));


  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();

  ts->signal_changed().connect(sigc::mem_fun(*this,
              &NoteListPaneView::on_treeview_row_changed) );

  m_TreeView.signal_row_activated().connect(sigc::mem_fun(*this,
              &NoteListPaneView::on_treeview_row_activated) );

  show_all ();
}
void NoteListPaneView::treeviewcolumn_validated_on_cell_data(
  Gtk::CellRenderer* ,
  const Gtk::TreeModel::iterator& iter)
{
  std::cout << "render" << std::endl;
}
NoteListPaneView::~NoteListPaneView () {

}

void NoteListPaneView::setDatabaseManager (DatabaseManager* d) {
  dbm = d;
  dbm->exec ("select * from notes", &fillNotesCallback,this);
  m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
}


int NoteListPaneView::fillNotesCallback (void* nlpv, int argc, char **argv, char **azColName) {
  NoteListPaneView* p = (NoteListPaneView*) nlpv;

  Gtk::TreeModel::Row row = *(p->m_refTreeModel->append());
  row[p->m_Columns.m_col_id] = 1;
  row[p->m_Columns.m_col_name] = "id";
  NoteData n1 (atoi(argv[0]), argv[1], "14:53", argv[2]);
  row[p->m_Columns.m_note_data] = n1;
  
  std::cout << "NoteListPaneView::fillNotesCallback PKey: " << atoi(argv[0]) << std::endl;
  
  return 0;
}

void NoteListPaneView::on_treeview_row_activated (const Gtk::TreePath& tp, Gtk::TreeViewColumn* const& tvc){
  std::cout << "TreeView Row Activated" << std::endl;
}

void NoteListPaneView::on_treeview_row_changed () {
  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();
  Gtk::TreeModel::iterator iter = ts->get_selected ();
  Glib::RefPtr<Gtk::TreeModel> tm = ts->get_model ();

  if (iter) {
    Gtk::TreeModel::Path path = tm->get_path (iter);
    Gtk::TreeModel::Row row = *iter;
    NoteData n = row[m_Columns.m_note_data];
    std::cout << "NoteListPaneView::on_treeview_row_changed, Note, Title: " << n.getTitle () << ", PrimaryKey: " << n.getPrimaryKey () << std::endl;
    app->npv->setWebViewContent (n.getSummary ());
  }
}
template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}

void NoteListPaneView::fetchNotesForNotebook (int primaryKey) {
  m_refTreeModel->clear ();
 
  std::string query;
  if (primaryKey == 0)
    query = "select * from notes order by id";
  else
    query = "select * from notes where notebook_id = " + ::NumberToString(primaryKey) + " order by id";
  std::cout << query << std::endl;
  if (dbm)
    dbm->exec (query, & fillNotesCallback, this);
  
  m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();
  Gtk::TreeModel::iterator iter = ts->get_selected ();
  if (iter) {
    Gtk::TreeModel::Row row = *iter;

    NoteData n = row[m_Columns.m_note_data];
    app->npv->setWebViewContent (n.getSummary ());
  } else {
    if (app && primaryKey != 0)
      if (app->npv) {
        app->npv->setWebViewContent ("No Notes :(");
      }
  }
}

static Gtk::TreeViewColumn* create_column2 (Gtk::TreeModelColumn<int> tmc, Gtk::TreeModelColumn<NotebookData> n) {
  NotebookCellRenderer2* ncr = new NotebookCellRenderer2 ();
  Gtk::TreeViewColumn* c = Gtk::manage (new Gtk::TreeViewColumn ("Notebooks", *ncr));
  c->add_attribute(*ncr, "id", tmc);
  c->add_attribute(*ncr, "notebook", n);
  return c;
}
void NoteListPaneView::newNote () {

/*
  Gtk::Window* popup = Gtk::manage (new Gtk::Window ());
  popup->set_default_size (640, 360);
  popup->show_all ();
*/

  popup = new Gtk::MessageDialog (*app, "Enter note title: ", true, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK_CANCEL, true);
  Gtk::Box* contentBox = popup->get_content_area ();

  noteName = new Gtk::Entry ();
  noteName->set_text ("Untitled");
  contentBox->pack_end (*noteName);

  m_refTreeModel_notebooks = Gtk::TreeStore::create(m_Columns_notebooks);
//  m_Combo.append_column(*create_column2 (m_Columns_notebooks.m_col_id, m_Columns_notebooks.m_notebook_data));
  m_Combo.set_model (m_refTreeModel_notebooks);

  dbm->exec ("select * from notebooks where id > 0 order by title", &fillNotebooksCallback,this);
  m_Combo.set_active (0);
  contentBox->pack_end (m_Combo);
  contentBox->show_all ();
  popup->set_resizable (false);
  popup->set_modal (true);
  int reply = popup->run ();
  
  if (reply == Gtk::RESPONSE_OK) {
    std::cout << "Resonse ok." << std::endl;
    newNoteOk ();
    popup->hide ();
    m_Combo.unparent ();
  } else if (reply == Gtk::RESPONSE_CANCEL) {
    std::cout << "Resonse cancel." << std::endl;
    popup->hide ();
    m_Combo.unparent ();
  } else {
    std::cout << "Resonse else." << std::endl;
    popup->hide ();
    m_Combo.unparent ();
  }
}

int NoteListPaneView::fillNotebooksCallback (void* lpv, int argc, char **argv, char **azColName) {
  std::cout << "NoteListPaneView::fillNotebooksCallback" << std::endl;
  NoteListPaneView* p = (NoteListPaneView*) lpv;

  std::string notebookName = "";
  int notebookId = atoi (argv[0]);
  notebookName += argv[1];

  NotebookData* nbd = new NotebookData (notebookId, notebookName);
  Gtk::TreeModel::Row childrow = *(p->m_refTreeModel_notebooks->append());
  childrow[p->m_Columns_notebooks.m_col_id] = notebookId;
  childrow[p->m_Columns_notebooks.m_col_name] = notebookName;
  childrow[p->m_Columns_notebooks.m_notebook_data] = *nbd;
  
  return 0;
}
void NoteListPaneView::newNoteOk () {

  Gtk::TreeModel::iterator iter = m_Combo.get_active ();

  NotebookData nbd;

  if (iter) {
    Gtk::TreeModel::Row row = *iter;
    nbd = row [m_Columns_notebooks.m_notebook_data];

    Gtk::TreeModel::Path path = m_refTreeModel_notebooks->get_path (iter);

    dbm->exec ("INSERT INTO notes values (NULL,'"+ noteName->get_text ()+"', '', " + NumberToString (nbd.getPrimaryKey ()) + ", 0, 0)", NULL,this);
    fetchNotesForNotebook (nbd.getPrimaryKey ());
    app->lpv->selectNotebookInPane (path[0]);
    app->npv->newNote ();
  } else {
    std::cout << "Invalid Selection" << std::endl;
  }

}