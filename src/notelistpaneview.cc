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
#include <uuid/uuid.h>
#include <iostream>
#include <cstring>
#include <vector>

#include "notelistpaneview.hh"
#include "windowbody.hh"
#include "notedata.hh"
#include "querygenerator.hh"

  template <typename T>
std::string NumberToString(T pNumber)
{
  std::ostringstream oOStrStream;
  oOStrStream << pNumber;
  return oOStrStream.str();
}

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
        set_fixed_size (1, 65);
      }

    Pango::Rectangle* renderNote (const ::Cairo::RefPtr< ::Cairo::Context >& cr, 
        Gtk::Widget& widget, const Gdk::Rectangle& background_area, 
        const Gdk::Rectangle& cell_area, Pango::Rectangle* pr, int id) {
      Pango::FontDescription font_from;
      font_from.set_size (10 * Pango::SCALE);
      font_from.set_weight (Pango::WEIGHT_SEMIBOLD);
      Glib::RefPtr <Pango::Layout> layout_from = widget.create_pango_layout ("");
      layout_from->set_font_description (font_from);

      if (property_note_.get_value ().getTitle ().length () > 25)
        layout_from->set_markup ("<span foreground='#555'>" + 
            property_note_.get_value ().getTitle ().substr (0, 25) + "...</span>");
      else
        layout_from->set_markup ("<span foreground='#555'>" + 
            property_note_.get_value ().getTitle ().substr (0, 25) + "</span>");

      layout_from->set_width(210 * Pango::SCALE);
      cr->move_to (10, 5 + cell_area.get_y ());
      layout_from->show_in_cairo_context (cr);

      font_from.set_size (8 * Pango::SCALE);
      font_from.set_weight (Pango::WEIGHT_NORMAL);
      layout_from = widget.create_pango_layout ("");
      layout_from->set_font_description (font_from);
      layout_from->set_alignment(Pango::ALIGN_RIGHT);
      layout_from->set_markup ("<span foreground='#BBB'>" + 
          property_note_.get_value ().getRemaining () + "</span>");
      layout_from->set_width(90* Pango::SCALE);
      cr->move_to (cell_area.get_width () - 90, 10 + cell_area.get_y ());
      layout_from->show_in_cairo_context (cr);

      font_from.set_size (9 * Pango::SCALE);
      font_from.set_weight (Pango::WEIGHT_LIGHT);
      layout_from = widget.create_pango_layout ("");
      layout_from->set_font_description (font_from);
      layout_from->set_alignment(Pango::ALIGN_LEFT);
      std::string summary = property_note_.get_value ().getSummary ();

      if (summary.length () > (cell_area.get_width () / 4.0) + 10)
        layout_from->set_markup ("<span foreground='#888'>" + 
            summary.substr (0, (cell_area.get_width () / 4.0) + 10) + "...</span>");
      else
        layout_from->set_markup ("<span foreground='#888'>" + 
            summary.substr (0, (cell_area.get_width () / 4.0) + 10) + "</span>");
      layout_from->set_width((cell_area.get_width () - 10) * Pango::SCALE);
      cr->move_to (10, 27 + cell_area.get_y ());

      layout_from->show_in_cairo_context (cr);
      return pr;
    }

    Glib::Property< int > property_id_;
    Glib::Property< NoteData > property_note_;

  protected:
    virtual void render_vfunc (const ::Cairo::RefPtr< ::Cairo::Context >& cr, 
        Gtk::Widget& widget, const Gdk::Rectangle& background_area, 
        const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags) {
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
      if (property_notebook_.get_value ().getGuid ().empty ()) {
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


NoteListPaneView::NoteListPaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding, Notify* a, DatabaseManager* d) 
  : Gtk::Box (Gtk::ORIENTATION_VERTICAL, padding) {

    app = a;
    dbm = d;
    firstRowOfResultset = false;

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

    m_TreeView.signal_button_press_event ().connect_notify (sigc::mem_fun(*this,
          &NoteListPaneView::on_treeview_button_release_event) );

    Gtk::MenuItem* item = Gtk::manage(new Gtk::MenuItem("_Delete Note", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &NoteListPaneView::on_menu_file_popup_delete_note) );
    m_Menu_Popup.append(*item);
    m_Menu_Popup.accelerate(*this);
    m_Menu_Popup.show_all(); //Show all menu items when the menu pops up

    show_all ();
    firstRowOfResultset = true;
    dbm->exec ("select a.id, a.title, substr (a.body, 0, 300), a.created_time, a.modified_time, a.guid, a.notebook_guid, a.usn, a.dirty, b.title from notes a, notebooks b where a.notebook_guid = b.guid order by a.modified_time desc, a.id", &fillNotesCallback,this);

  }
void NoteListPaneView::treeviewcolumn_validated_on_cell_data(
    Gtk::CellRenderer* ,
    const Gtk::TreeModel::iterator& iter)
{

}
NoteListPaneView::~NoteListPaneView () {

}

int NoteListPaneView::fillNotesCallback (void* nlpv, int argc, char **argv, char **azColName) {
  NoteListPaneView* p = (NoteListPaneView*) nlpv;

  Gtk::TreeModel::Row row = *(p->m_refTreeModel->append());
  row[p->m_Columns.m_col_id] = 1;
  row[p->m_Columns.m_col_name] = "id";
  /*id integer primary key, title text, body text, created_time datetime, modified_time datetime, guid text, notebook_guid text*/
  /* a.id, a.title, a.body, a.created_time, a.modified_time, a.guid, a.notebook_guid, a.usn, a.dirty, b.title */
  NoteData n1 (atoi(argv[0]), argv[1], argv[2], atoi(argv[3]), atoi (argv[4]), argv[5], argv[6], argv[9]);
  row[p->m_Columns.m_note_data] = n1;
  //  
  p->m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
  return 0;
}

void NoteListPaneView::on_treeview_row_activated (const Gtk::TreePath& tp, Gtk::TreeViewColumn* const& tvc){

}

void NoteListPaneView::on_treeview_row_changed () {
}

void NoteListPaneView::fetchNotesForNotebook (std::string n_guid) {
  m_refTreeModel->clear ();

  std::string query;
  if (n_guid.empty () || n_guid == "_")
    query = "select a.id, a.title, substr (a.body, 0, 300), a.created_time, a.modified_time, a.guid, a.notebook_guid, a.usn, a.dirty, b.title from notes a, notebooks b where a.notebook_guid = b.guid order by a.modified_time desc, a.id";
  else
    query = "select a.id, a.title, substr (a.body, 0, 300), a.created_time, a.modified_time, a.guid, a.notebook_guid, a.usn, a.dirty, b.title from notes a, notebooks b where a.notebook_guid = b.guid and a.notebook_guid = '" + n_guid + "' order by a.modified_time desc, a.id";

  if (dbm) {
    firstRowOfResultset = true;
    dbm->exec (query, & fillNotesCallback, this);
  }

  m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();
  Gtk::TreeModel::iterator iter = ts->get_selected ();
  if (iter) {
    Gtk::TreeModel::Row row = *iter;

    NoteData n = row[m_Columns.m_note_data];
    app->npv->setNote (n);
    app->npv->enableButtons ();
    m_TreeView.set_cursor (Gtk::TreeModel::Path ("0"));
    m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
  } else {
    if (app && !n_guid.empty ())
      if (app->npv) {
        app->npv->setWebViewContent ("Click the new note button to create a note.");
        app->npv->setNoteTitleEntryText ("Untitled");
        app->npv->setNoteNotebookTitleEntryText ("");
        app->npv->disableButtons ();

      }
  }
}


void NoteListPaneView::fetchNotesForNotebooks (std::vector<std::string> guids) {
  m_refTreeModel->clear ();

  std::string query;
  if (guids.empty () || (guids.size () == 1 && guids[0] == "_")) {
    query = "select a.id, a.title, substr (a.body, 0, 300), a.created_time, a.modified_time, a.guid, a.notebook_guid, a.usn, a.dirty, b.title from notes a, notebooks b where a.notebook_guid = b.guid ";
  } else {
    query = "select a.id, a.title, substr (a.body, 0, 300), a.created_time, a.modified_time, a.guid, a.notebook_guid, a.usn, a.dirty, b.title from notes a, notebooks b where a.notebook_guid = b.guid and a.notebook_guid = ";
    for (unsigned int i = 0; i < guids.size (); i++) {
      query += "'";
      query += guids[i];
      query += "'";
      if (i != (guids.size () -1)) {
        query += " or a.notebook_guid = ";
      }
    }
  }
  query += " order by a.modified_time desc, a.id";


  if (dbm){
    firstRowOfResultset = true;
    dbm->exec (query, & fillNotesCallback, this);
  }

  m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();
  Gtk::TreeModel::iterator iter = ts->get_selected ();
  if (iter) {
    Gtk::TreeModel::Row row = *iter;

    NoteData n = row[m_Columns.m_note_data];
    app->npv->setNote (n);
    app->npv->enableButtons ();
    m_TreeView.set_cursor (Gtk::TreeModel::Path ("0"));
    m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
  } else {
    if (app && !guids.empty ())
      if (app->npv) {
        app->npv->setWebViewContent ("Click the new note button to create a note.");
        app->npv->setNoteTitleEntryText ("Untitled");
        app->npv->setNoteNotebookTitleEntryText ("");
        app->npv->disableButtons ();

      }
  }
}

/*
   static Gtk::TreeViewColumn* create_column2 (Gtk::TreeModelColumn<int> tmc, Gtk::TreeModelColumn<NotebookData> n) {
   NotebookCellRenderer2* ncr = new NotebookCellRenderer2 ();
   Gtk::TreeViewColumn* c = Gtk::manage (new Gtk::TreeViewColumn ("Notebooks", *ncr));
   c->add_attribute(*ncr, "id", tmc);
   c->add_attribute(*ncr, "notebook", n);
   return c;
   }
   */
void NoteListPaneView::newNote () {

  //  db->exec (QueryGenerator::get Query (QueryGenerator::getNotebookCount, ""), 
  //      &getNotebookCountCallback, this);


  popup = new Gtk::MessageDialog (*app, "Select Notebook and Enter Note Title", 
      true, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK_CANCEL, true);
  Gtk::Box* contentBox = popup->get_content_area ();

  Gtk::Box* firstBox = Gtk::manage (new Gtk::Box ());
  Gtk::Box* secondBox = Gtk::manage (new Gtk::Box ());

  noteName = new Gtk::Entry ();
  noteName->set_text ("Untitled");
  firstBox->pack_start (*noteName, true, false, 0);
  firstBox->set_size_request (400, -1);

  m_refTreeModel_notebooks = Gtk::TreeStore::create(m_Columns_notebooks);
  m_Combo.set_model (m_refTreeModel_notebooks);

  notebookCount = 0;

  dbm->exec (QueryGenerator::getQuery (QueryGenerator::getAllNotebooks, ""), &fillNotebooksCallback,this);

  if (notebookCount == 0) {
    std::cout << "NLPV:newNote: Notebook Count 0. First create notebook." << std::endl;
    m_Combo.set_active (0);
    contentBox->set_size_request (-1, -1);
    
    Gtk::Box* thirdBox = Gtk::manage (new Gtk::Box ());
    Gtk::Label* label = Gtk::manage (new Gtk::Label ("First create some notebooks.", false));

    thirdBox->pack_start (*label, true, false, 0);

    contentBox->add (*thirdBox);
    
    contentBox->show_all ();
  
  } else {

    m_Combo.set_active (0);
    contentBox->set_size_request (-1, -1);
    secondBox->pack_start (m_Combo, true, false, 0);
    secondBox->set_size_request (400, -1);

    contentBox->add (*secondBox);
    contentBox->add (*firstBox);

    contentBox->show_all ();


    addCss (contentBox, "contentBox", ".contentBox { padding-left:15px; margin-left:50px; \n}\n");

    popup->set_resizable (false);
    popup->set_modal (true);
  }
  int reply = popup->run ();

  if (reply == Gtk::RESPONSE_OK) {

    newNoteOk ();
    popup->hide ();
    m_Combo.unparent ();
  } else if (reply == Gtk::RESPONSE_CANCEL) {

    popup->hide ();
    m_Combo.unparent ();
  } else {

    popup->hide ();
    m_Combo.unparent ();
  }
}

/*
   int NoteListPaneView::getNotebookCountCallback (void* nlpv, int argc, char** argv, char** azColName) {
   NoteListPaneView* p = (NoteListPaneView*) p;

   return 0;
   }
   */

int NoteListPaneView::fillNotebooksCallback (void* lpv, int argc, char **argv, char **azColName) {

  NoteListPaneView* p = (NoteListPaneView*) lpv;

  std::string notebookName = "";
  int notebookId = atoi (argv[0]);
  notebookName += argv[1];
  /* id integer primary key, title text unique, guid text, 
   * parent_guid text, created_time datetime, modified_time datetime 
   * */
  NotebookData* nbd = new NotebookData (atoi (argv[0]), 
      argv[1], argv[2], (argv[3] == NULL ? "" : argv[3]), atoi (argv[4]), atoi (argv[5]));
  Gtk::TreeModel::Row childrow = *(p->m_refTreeModel_notebooks->append());
  childrow[p->m_Columns_notebooks.m_col_id] = notebookId;
  childrow[p->m_Columns_notebooks.m_col_name] = notebookName;
  childrow[p->m_Columns_notebooks.m_notebook_data] = *nbd;

  p->notebookCount++;
  return 0;
}
void NoteListPaneView::newNoteOk () {

  Gtk::TreeModel::iterator iter = m_Combo.get_active ();

  NotebookData nbd;

  if (iter) {
    Gtk::TreeModel::Row row = *iter;
    nbd = row [m_Columns_notebooks.m_notebook_data];

    Gtk::TreeModel::Path path = m_refTreeModel_notebooks->get_path (iter);

    uuid_t uuid;
    uuid_generate_time_safe(uuid);

    // unparse (to string)
    char uuid_str[37];      // ex. "1b4e28ba-2fa1-11d2-883f-0016d3cca427" + "\0"
    uuid_unparse_lower(uuid, uuid_str);
    std::string query = "INSERT INTO notes values (NULL,'";
    query += noteName->get_text ();
    query += "', '', strftime ('%s','now'), strftime ('%s','now'), '";
    query += uuid_str;
    query += "', '"+ nbd.getGuid () + "', 0, 0)";

    dbm->exec (query, NULL,this);
    fetchNotesForNotebook (nbd.getGuid ());
    app->lpv->selectNotebookInPane (path[0]);
    app->npv->newNote ();
  } else {

  }

}



void NoteListPaneView::on_treeview_button_release_event (GdkEventButton* event) {
  /* single click with the right mouse button? */
  std::cout << event->button << std::endl;
  if (event->button == 3)
  {



    Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();
    Glib::RefPtr<Gtk::TreeModel> tm = ts->get_model ();

    Gtk::TreeModel::Path path;
    m_TreeView.get_path_at_pos ((gint) event->x, (gint) event->y, path);

    if (path.size () == 1) {

      Gtk::TreeModel::Row row = *(tm->get_iter (path));
      selectedNote = row[m_Columns.m_note_data];


      m_Menu_Popup.popup(event->button, event->time);
    }
  } else {  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();
    Gtk::TreeModel::iterator iter = ts->get_selected ();
    Glib::RefPtr<Gtk::TreeModel> tm = ts->get_model ();

    if (iter) {
      Gtk::TreeModel::Path path;
      m_TreeView.get_path_at_pos ((gint) event->x, (gint) event->y, path);
      if (path) {
        Gtk::TreeModel::Row row = *(tm->get_iter (path));
        NoteData n = row[m_Columns.m_note_data];

        app->npv->setNote (n);
        app->npv->enableButtons ();
      }
    }
  }

}

void NoteListPaneView::on_menu_file_popup_delete_note () {

  popup = new Gtk::MessageDialog (*app, "Delete Note ?", true, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK_CANCEL, true);
  Gtk::Box* contentBox = popup->get_content_area ();
  contentBox->show_all ();
  popup->set_resizable (false);
  popup->set_modal (true);
  int reply = popup->run ();

  if (reply == Gtk::RESPONSE_OK) {

    noteDelete ();
    popup->hide ();
  } else {

    popup->hide ();
  }
}


void NoteListPaneView::noteDelete () {
  std::string note_id = selectedNote.getGuid ();
  dbm->exec ("delete from notes where guid = '" + note_id + "'", NULL, this);
  fetchNotesForNotebooks (app->lpv->selectedNotebookGuids);
}

void NoteListPaneView::noteSearch (std::string str) {
  m_refTreeModel->clear ();

  std::string query;
  query = "select a.id, a.title, substr (a.body, 0, 300), a.created_time, a.modified_time, a.guid, a.notebook_guid, a.usn, a.dirty, b.title from notes a, notebooks b where a.notebook_guid = b.guid ";
  query += "and (a.title like '%" + str + "%' or a.body like '%" + str + "%') order by a.modified_time desc, a.id";


  if (dbm) {
    firstRowOfResultset = true;
    dbm->exec (query, & fillNotesCallback, this);
  }

  m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView.get_selection ();
  Gtk::TreeModel::iterator iter = ts->get_selected ();
  if (iter) {
    Gtk::TreeModel::Row row = *iter;

    NoteData n = row[m_Columns.m_note_data];
    app->npv->setNote (n);
    app->npv->enableButtons ();
    m_TreeView.set_cursor (Gtk::TreeModel::Path ("0"));
    m_TreeView.get_selection ()->select (Gtk::TreeModel::Path ("0"));
  } else {
    if (app->npv) {
      app->npv->setWebViewContent ("Click the new note button to create a note.");
      app->npv->setNoteTitleEntryText ("Untitled");
      app->npv->setNoteNotebookTitleEntryText ("");
      app->npv->disableButtons ();
    }
  }
}
