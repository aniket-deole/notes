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

#include <uuid/uuid.h>
#include <gtkmm.h>
#include <math.h>

#include "leftpaneview.hh"
#include "windowbody.hh"

#include "notebookdata.hh"

/**
  Context: Stacks Almost Working. Check saveNote. it fails when stack is selected.
  May be we will have to create a new ds for stacks.
**/


NotebookTreeStore::NotebookTreeStore()
{
  //We can't just call Gtk::TreeModel(m_Columns) in the initializer list
  //because m_Columns does not exist when the base class constructor runs.
  //And we can't have a static m_Columns instance, because that would be
  //instantiated before the gtkmm type system.
  //So, we use this method, which should only be used just after creation:
  set_column_types(m_Columns);
}

Glib::RefPtr<NotebookTreeStore> NotebookTreeStore::create(LeftPaneView* l)
{
  
  Glib::RefPtr<NotebookTreeStore> reference = Glib::RefPtr<NotebookTreeStore>( new NotebookTreeStore() );
  reference->lpv = l;
  return reference;
}

bool
NotebookTreeStore::row_draggable_vfunc(const Gtk::TreeModel::Path& path) const
{
  // Make the value of the "draggable" column determine whether this row can
  // be dragged:

  //TODO: Add a const version of get_iter to TreeModel:
  NotebookTreeStore* unconstThis = const_cast<NotebookTreeStore*>(this);
  const_iterator iter = unconstThis->get_iter(path);
  //const_iterator iter = get_iter(path);
  if(iter)
  {
    Row row = *iter;
    bool is_draggable = row[m_Columns.m_col_draggable];
    std::cout << "NotebookTreeStore::row_draggable_vfunc" << std::endl;
    lpv->notebookBeingDragged = new NotebookData (row[m_Columns.m_notebook_data]);
    return is_draggable;
  }
  return Gtk::TreeStore::row_draggable_vfunc(path);
}

bool
NotebookTreeStore::row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest,
        const Gtk::SelectionData& selection_data) const
{
  //Make the value of the "receives drags" column determine whether a row can be
  //dragged into it:

  //dest is the path that the row would have after it has been dropped:
  //But in this case we are more interested in the parent row:
  Gtk::TreeModel::Path dest_parent = dest;
  bool dest_is_not_top_level = dest_parent.up();
  if(!dest_is_not_top_level || dest_parent.empty())
  {
    //The user wants to move something to the top-level.
    //Let's always allow that.
    return true;
  }
  else
  {
    //Get an iterator for the row at this path:
    //We must unconst this. This should not be necessary with a future version
    //of gtkmm.
    //TODO: Add a const version of get_iter to TreeModel:
    NotebookTreeStore* unconstThis = const_cast<NotebookTreeStore*>(this);
    const_iterator iter_dest_parent = unconstThis->get_iter(dest_parent);
    //const_iterator iter_dest_parent = get_iter(dest);
    if(iter_dest_parent)
    {
      Row row = *iter_dest_parent;
      bool receives_drags = row[m_Columns.m_col_receivesdrags];
      NotebookData destNotebook = row[m_Columns.m_notebook_data];
      Glib::RefPtr<Gtk::TreeModel> refThis = Glib::RefPtr<Gtk::TreeModel>(const_cast<NotebookTreeStore*>(this));
      refThis->reference(); //, true /* take_copy */)

      Gtk::TreeModel::Path path_dragged_row;
      Gtk::TreeModel::Path::get_from_selection_data(selection_data, refThis,
        path_dragged_row);
      
      NotebookTreeStore* unconstThis = const_cast<NotebookTreeStore*>(this);
      const_iterator iter = unconstThis->get_iter(path_dragged_row);
      //const_iterator iter = get_iter(path);
      if(iter)
      {
        Row sourceRow = *iter;
        NotebookData nbd = row[m_Columns.m_notebook_data];
        NotebookData sNbd = sourceRow[m_Columns.m_notebook_data];
        std::cout << nbd.getGuid () << ":" << sNbd.getGuid () << std::endl;
        if (nbd.getGuid () == sNbd.getGuid ()) {
          return false;
        }
        lpv->notebookDestination = new NotebookData (row[m_Columns.m_notebook_data]);

      }
      return receives_drags;
    }
  }

  //You could also examine the row being dragged (via selection_data)
  //if you must look at both rows to see whether a drop should be allowed.
  //You could use
  //TODO: Add const version of get_from_selection_data(): Glib::RefPtr<const
  //Gtk::TreeModel> refThis = Glib::RefPtr<const Gtk::TreeModel>(this);
  //
  //Glib::RefPtr<Gtk::TreeModel> refThis =
  //Glib::RefPtr<Gtk::TreeModel>(const_cast<TreeModel_Dnd*>(this));
  //refThis->reference(); //, true /* take_copy */)
  //Gtk::TreeModel::Path path_dragged_row;
  //Gtk::TreeModel::Path::get_from_selection_data(selection_data, refThis,
  //path_dragged_row);

  return Gtk::TreeStore::row_drop_possible_vfunc(dest, selection_data);
}

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
    set_fixed_size (1, 20);
  }

  Pango::Rectangle* renderNotebook (const ::Cairo::RefPtr< ::Cairo::Context >& cr, Gtk::Widget& widget, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Pango::Rectangle* pr, int id) {
    Pango::FontDescription font_from;
    font_from.set_size (10 * Pango::SCALE);
    if (property_notebook_.get_value ().getGuid ().empty ()) {
      font_from.set_weight (Pango::WEIGHT_SEMIBOLD);
      cr->move_to (10, cell_area.get_y () + 3);
    } else {
      if (property_notebook_.get_value ().getStack ().empty ()) {
        cr->move_to (10, cell_area.get_y () + 3); 
      } else {
        cr->move_to (25, cell_area.get_y () + 3); 
      }
      font_from.set_weight (Pango::WEIGHT_NORMAL);
    }
    Glib::RefPtr <Pango::Layout> layout_from = widget.create_pango_layout ("");
    layout_from->set_font_description (font_from);
    layout_from->set_markup ("<span foreground='#444'>" + property_notebook_.get_value ().getTitle () + "</span>");
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
  notebookBeingDragged = NULL;
  set_orientation (Gtk::ORIENTATION_VERTICAL);

  set_size_request (200, -1);

  //Add the TreeView, inside a ScrolledWindow, with the button underneath:

  Glib::RefPtr<Gtk::StyleContext> sc = m_TreeView_Notebooks.get_style_context(); 
  sc->add_class("source-list");
  m_ScrolledWindow.add(m_TreeView_Notebooks);

  //Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  pack_start(m_ScrolledWindow);

  //Create the Tree model:
  m_refTreeModel = NotebookTreeStore::create(this);
  m_TreeView_Notebooks.set_model(m_refTreeModel);
  m_TreeView_Notebooks.set_headers_visible (false);
  m_TreeView_Notebooks.set_show_expanders (false);
  //All the items to be reordered with drag-and-drop:
  m_TreeView_Notebooks.set_reorderable(false);

  //Fill the TreeView's model
  notebooksRow = *(m_refTreeModel->append());
  notebooksRow[m_refTreeModel->m_Columns.m_col_id] = 0;
  notebooksRow[m_refTreeModel->m_Columns.m_col_name] = "Notebooks";
  notebooksRow[m_refTreeModel->m_Columns.m_col_draggable] = false;
  notebooksRow[m_refTreeModel->m_Columns.m_col_receivesdrags] = false;

  NotebookData* nbd = new NotebookData (-1, "Notebooks", "", "", 0, 0);
  notebooksRow[m_refTreeModel->m_Columns.m_notebook_data] = *nbd;

  m_TreeView_Notebooks.append_column(*create_column (m_refTreeModel->m_Columns.m_col_id, m_refTreeModel->m_Columns.m_notebook_data));

  //Connect signal:
  m_TreeView_Notebooks.signal_row_expanded().connect(sigc::mem_fun(*this,
              &LeftPaneView::on_treeview_row_expanded) );

//  m_TreeView_Notebooks.signal_row_activated().connect(sigc::mem_fun(*this,
//              &LeftPaneView::on_treeview_row_activated) );

  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView_Notebooks.get_selection ();

  ts->signal_changed().connect(sigc::mem_fun(*this,
              &LeftPaneView::on_treeview_row_changed) );

  m_TreeView_Notebooks.signal_button_press_event ().connect_notify (sigc::mem_fun(*this,
              &LeftPaneView::on_treeview_button_release_event) );

  m_TreeView_Notebooks.enable_model_drag_source();
  m_TreeView_Notebooks.enable_model_drag_dest();
  /* Select the All Notes item by default */ 
  m_TreeView_Notebooks.signal_drag_end().connect(sigc::mem_fun(*this,
            &LeftPaneView::nnDragFinished) );
  m_TreeView_Notebooks.signal_drag_begin().connect(sigc::mem_fun(*this,
            &LeftPaneView::nnDragStarted) );
  
  Gtk::MenuItem* item = Gtk::manage(new Gtk::MenuItem("_Edit", true));
  item->signal_activate().connect(
    sigc::mem_fun(*this, &LeftPaneView::on_menu_file_popup_edit_notebook_name) );
  m_Menu_Popup.append(*item);
  item = Gtk::manage(new Gtk::MenuItem("_Delete", true));
  item->signal_activate().connect(
    sigc::mem_fun(*this, &LeftPaneView::on_menu_file_popup_delete_notebook) );
  m_Menu_Popup.append(*item);
  m_Menu_Popup.accelerate(*this);
  m_Menu_Popup.show_all(); //Show all menu items when the menu pops up
  
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

  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView_Notebooks.get_selection ();
  Gtk::TreeModel::iterator iter = ts->get_selected ();
  Glib::RefPtr<Gtk::TreeModel> tm = ts->get_model ();

  if (iter) {
    Gtk::TreeModel::Path path = tm->get_path (iter);
    if (path.size () == 1) {
      if (path[0] == 0)
        ts->unselect_all ();
      else
        ts->select (Gtk::TreeModel::Path (selectedPath));
      
      /* Expand tree */
      if (m_TreeView_Notebooks.row_expanded (path)) {
        m_TreeView_Notebooks.collapse_row (path);
      } else {
        m_TreeView_Notebooks.expand_all ();
        ts->select (Gtk::TreeModel::Path (selectedPath));
      }
      std::cout << "Tree Header Clicked." << std::endl;
    } else {

      Gtk::TreeModel::Path path = tm->get_path (iter);
      std::cout << "Tree Child Clicked." << std::endl;
      selectedPath = tm->get_string (iter);

      /* Callback to fill up the notelistpane. */
      Gtk::TreeModel::Row row = *iter;
  //  app->npv->setWebViewContent (n.getSummary ());

      selectedNotebook = row[m_refTreeModel->m_Columns.m_notebook_data];

      /*
      if (selectedNotebook.getParentGuid () != "") {
        Expand tree 
        if (m_TreeView_Notebooks.row_expanded (path)) {
          m_TreeView_Notebooks.collapse_row (path);
        } else {
          m_TreeView_Notebooks.expand_to_path (path);
          ts->select (Gtk::TreeModel::Path (selectedPath));
        } 
      }
      */
      if (row [m_refTreeModel->m_Columns.m_col_is_stack]) {
        selectedNotebookGuids.clear ();
        Gtk::TreeModel::Children children = row.children ();
        selectedNotebookGuids.clear ();
        for (unsigned int i = 0; i < children.size (); i++) {
          Gtk::TreeModel::Row cr = children[i];
          NotebookData notebookDataItem = cr[m_refTreeModel->m_Columns.m_notebook_data];
          selectedNotebookGuids.push_back (notebookDataItem.getGuid ());  
        }
        m_TreeView_Notebooks.row_expanded (path);
        return;
      } else {
        selectedNotebookGuids.clear ();
        selectedNotebookGuids.push_back (selectedNotebook.getGuid ());
        app->nlpv->fetchNotesForNotebook (selectedNotebook.getGuid ());
        std::cout << "LeftPaneView::on_treeview_row_changed Name: " << row[m_refTreeModel->m_Columns.m_col_id] << ", PKey: " << row[m_refTreeModel->m_Columns.m_col_name] << std::endl;
      }
    }
  }
}

LeftPaneView::~LeftPaneView () {

}

void LeftPaneView::setDatabaseManager (DatabaseManager* d) {
  dbm = d;  
  refreshLeftPaneView ();
}

int LeftPaneView::fillNotebooksCallback (void* lpv, int argc, char **argv, char **azColName) {
  std::cout << "LeftPaneView::fillNotebooksCallback" << std::endl;
  LeftPaneView* p = (LeftPaneView*) lpv;

  std::string notebookName = "";
  std::string stack = "";
  int notebookId = atoi (argv[0]);
  notebookName += argv[1];
  stack += (argv[3] == NULL ? "" : argv[3]);
  /* id integer primary key, title text unique, guid text, stack text, created_time datetime, modified_time datetime */
  NotebookData* nbd = new NotebookData (atoi (argv[0]), argv[1], argv[2], (argv[3] == NULL ? "" : argv[3]), atoi (argv[4]), atoi (argv[5]));

  if (!stack.empty ()) {
    bool stackExists = false;
    for (unsigned int i = 0; i < p->stacks.size (); i++) {
      if (stack == p->stacks[i]) {
        stackExists = true;
        break;
      }
    }
    if (stackExists) {
      Gtk::TreeModel::Children children = p->notebooksRow.children ();
      /* Get Row and add a child row. */
      for (unsigned int i = 0; i < children.size (); i++) {
        std::cout << children[i] << std::endl;
        Gtk::TreeModel::Row cr = children[i];
        std::cout << cr[p->m_refTreeModel->m_Columns.m_col_name] << std::endl;
        NotebookData notebookDataItem = cr[p->m_refTreeModel->m_Columns.m_notebook_data];
        if (notebookDataItem.getTitle () == stack) {
            Gtk::TreeModel::Row ccr = *(p->m_refTreeModel->append(cr.children ()));
            ccr[p->m_refTreeModel->m_Columns.m_col_id] = notebookId;
            ccr[p->m_refTreeModel->m_Columns.m_col_name] = notebookName;
            ccr[p->m_refTreeModel->m_Columns.m_notebook_data] = *nbd;
            ccr[p->m_refTreeModel->m_Columns.m_col_draggable] = true;
            ccr[p->m_refTreeModel->m_Columns.m_col_receivesdrags] = false;
            ccr[p->m_refTreeModel->m_Columns.m_col_is_stack] = false;
            return 0;
        }
      }
    } else {
      /* Create a new Row with the name of the stack, and add this notebook as a child. */
      NotebookData* nbdStack = new NotebookData (0, stack, stack, "", 0, 0, true);
      Gtk::TreeModel::Row childrow = *(p->m_refTreeModel->append(p->notebooksRow.children()));
      childrow[p->m_refTreeModel->m_Columns.m_col_id] = -2 + (p->stacks.size ());
      childrow[p->m_refTreeModel->m_Columns.m_col_name] = stack;
      childrow[p->m_refTreeModel->m_Columns.m_notebook_data] = *nbdStack;
      childrow[p->m_refTreeModel->m_Columns.m_col_draggable] = false;
      childrow[p->m_refTreeModel->m_Columns.m_col_receivesdrags] = true;
      childrow[p->m_refTreeModel->m_Columns.m_col_is_stack] = true;
      
      Gtk::TreeModel::Row ccr = *(p->m_refTreeModel->append(childrow.children ()));
      ccr[p->m_refTreeModel->m_Columns.m_col_id] = notebookId;
      ccr[p->m_refTreeModel->m_Columns.m_col_name] = notebookName;
      ccr[p->m_refTreeModel->m_Columns.m_notebook_data] = *nbd;
      ccr[p->m_refTreeModel->m_Columns.m_col_draggable] = true;
      ccr[p->m_refTreeModel->m_Columns.m_col_receivesdrags] = true;
      ccr[p->m_refTreeModel->m_Columns.m_col_is_stack] = false;

      p->stacks.push_back (stack);

      std::cout << "NotebooksRow Size: " << p->notebooksRow.children ().size () << std::endl;
    }
  } else {

    Gtk::TreeModel::Children children = p->notebooksRow.children ();
  
    Gtk::TreeModel::Row childrow = *(p->m_refTreeModel->append(p->notebooksRow.children()));
    childrow[p->m_refTreeModel->m_Columns.m_col_id] = notebookId;
    childrow[p->m_refTreeModel->m_Columns.m_col_name] = notebookName;
    childrow[p->m_refTreeModel->m_Columns.m_notebook_data] = *nbd;
    childrow[p->m_refTreeModel->m_Columns.m_col_draggable] = true;
    childrow[p->m_refTreeModel->m_Columns.m_col_receivesdrags] = true;
    childrow[p->m_refTreeModel->m_Columns.m_col_is_stack] = false;
    std::cout << "NotebooksRow Size: " << p->notebooksRow.children ().size () << std::endl;
  }
  return 0;
}

void LeftPaneView::nnDragFinished(  const Glib::RefPtr< Gdk::DragContext >&   context) {
  if (dragEnded)
    return;

  std::cout << "Drag Finished." << &context << std::endl;
  std::cout << notebookBeingDragged->getGuid () << ":" << notebookDestination->getGuid() << ":" << std::endl;

  if (!notebookDestination->getStack ().empty ()) {
    updateParentGuid (notebookBeingDragged->getGuid (), notebookDestination->getGuid (), notebookDestination->getStack ());
    dragEnded = true;
    return;
  }

  if (notebookDestination->getIsStack ()) {
    updateParentGuid (notebookBeingDragged->getGuid (), notebookDestination->getGuid (), notebookDestination->getTitle ());
    dragEnded = true;
    return;
  }

  if (notebookDestination->getGuid().empty ()) {
    std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX: Removed Stack from notebook." << std::endl;
    updateParentGuid (notebookBeingDragged->getGuid (), notebookDestination->getGuid (), "");
    dragEnded = true;
    return;  
  }

  /* show popup to get stack name */
  popup = new Gtk::MessageDialog (*app, "Enter notebook stack name: ", true, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK_CANCEL, false);
  Gtk::Box* contentBox = popup->get_content_area ();

  notebookName = new Gtk::Entry ();
  contentBox->pack_end (*notebookName);

  contentBox->show_all ();
  popup->set_resizable (false);
  popup->set_modal (true);

  int reply = popup->run ();
  popupW = -1; popupH = -1;
  if (reply == Gtk::RESPONSE_OK) {
    std::cout << "Resonse ok." << std::endl;
    popup->hide ();
    /* Update stacks */
    if (!notebookName->get_text().empty ()) {
      updateParentGuid (notebookBeingDragged->getGuid (), notebookDestination->getGuid (), notebookName->get_text ());
    }
  } else if (reply == Gtk::RESPONSE_CANCEL) {
    std::cout << "Resonse cancel." << std::endl;
    popup->hide ();
    refreshLeftPaneView ();
  } else {
    std::cout << "Resonse else." << std::endl;
    popup->hide ();
    refreshLeftPaneView ();
  }
  dragEnded = true;
}


void LeftPaneView::nnDragStarted(  const Glib::RefPtr< Gdk::DragContext >&   context) {
  dragEnded = false;
}

int LeftPaneView::fillTagsCallback (void* lpv, int argc, char **argv, char **azColName) {
  std::cout << "LeftPaneView::fillTagsCallback" << std::endl;
  LeftPaneView* p = (LeftPaneView*) lpv;

  int tagId = atoi (argv[0]);

  std::string tagName = "";
  tagName += argv[1];
  NotebookData* nbd = new NotebookData (tagId, tagName, "", "", 0, 0);

  Gtk::TreeModel::Row childrow = *(p->m_refTreeModel->append(p->tagsRow.children()));
  childrow[p->m_refTreeModel->m_Columns.m_col_id] = 4;
  childrow[p->m_refTreeModel->m_Columns.m_col_name] = tagName;
  childrow[p->m_refTreeModel->m_Columns.m_notebook_data] = *nbd;
  
  return 0;
}

void LeftPaneView::setApp (Notify* a) {
  app = a;
}

void LeftPaneView::newNotebook () {

  popup = new Gtk::MessageDialog (*app, "Enter Notebook Name", true, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK_CANCEL, false);
  Gtk::Box* contentBox = popup->get_content_area ();


  Gtk::Box* firstBox = Gtk::manage (new Gtk::Box ());

  notebookName = new Gtk::Entry ();
  firstBox->pack_start (*notebookName, true, false, 0);
  firstBox->set_size_request (400, -1);
  contentBox->set_size_request (-1, -1);
  contentBox->add (*firstBox);

  contentBox->show_all ();
  popup->set_resizable (false);
  popup->set_modal (true);

  int reply = popup->run ();
  popupW = -1; popupH = -1;
  if (reply == Gtk::RESPONSE_OK) {
    std::cout << "Resonse ok." << std::endl;
    newNotebookOk ();
    popup->hide ();
  } else if (reply == Gtk::RESPONSE_CANCEL) {
    std::cout << "Resonse cancel." << std::endl;
    popup->hide ();
  } else {
    std::cout << "Resonse else." << std::endl;
    popup->hide ();
  }

}

void LeftPaneView::newNotebookOk () {
  if (notebookName->get_text().empty ()) { return;}

  uuid_t uuid;
  uuid_generate_time_safe(uuid);

  // unparse (to string)
  char uuid_str[37];      // ex. "1b4e28ba-2fa1-11d2-883f-0016d3cca427" + "\0"
  uuid_unparse_lower(uuid, uuid_str);
  std::string query = "INSERT INTO notebooks values (NULL, '" + notebookName->get_text () + "','";
  query += uuid_str;
  query += "', NULL, 0, 0, 0, 0)";

  dbm->exec (query.c_str (), NULL,this);

  //Fill the TreeView's model
  refreshLeftPaneView ();
  notebookListSelected = true;
}

void LeftPaneView::on_treeview_button_release_event (GdkEventButton* event) {
        
      /* single click with the right mouse button? */
    if (event->button == 3)
    {
      std::cout << "Single right click on the tree view." << std::endl;


  Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView_Notebooks.get_selection ();
  Gtk::TreeModel::iterator iter = ts->get_selected ();
  Glib::RefPtr<Gtk::TreeModel> tm = ts->get_model ();

  Gtk::TreeModel::Path path;
     m_TreeView_Notebooks.get_path_at_pos ((gint) event->x, (gint) event->y, path);
    if (path.size () == 2) {
      selectedPath = tm->get_string (iter);
      if (path[0] == 1)
        notebookListSelected = false;
      else
        notebookListSelected = true;

      if (path [0] == 0 && path[1] != 0) {
        /* Callback to fill up the notelistpane. */
        Gtk::TreeModel::Row row = *(tm->get_iter (path));
    //  app->npv->setWebViewContent (n.getSummary ());
        selectedNotebook = row[m_refTreeModel->m_Columns.m_notebook_data];
        
        notebookName = new Gtk::Entry ();
        notebookName->set_text (row[m_refTreeModel->m_Columns.m_col_name]);
         
        std::cout << "LeftPaneView::on_treeview_button_release_event Name: " << row[m_refTreeModel->m_Columns.m_col_id] << ", PKey: " << selectedNotebook.getTitle () << std::endl;
        m_Menu_Popup.popup(event->button, event->time);

        if (row [m_refTreeModel->m_Columns.m_col_is_stack]) {
          Gtk::TreeModel::Children children = row.children ();
          selectedNotebookGuids.clear ();
          for (unsigned int i = 0; i < children.size (); i++) {
            Gtk::TreeModel::Row cr = children[i];
            NotebookData notebookDataItem = cr[m_refTreeModel->m_Columns.m_notebook_data];
            selectedNotebookGuids.push_back (notebookDataItem.getGuid ());  
          }
          return;
        } else {
          selectedNotebookGuids.clear ();
          selectedNotebookGuids.push_back (selectedNotebook.getGuid ());
          app->nlpv->fetchNotesForNotebook (selectedNotebook.getGuid ());  
        }        
      }
    }

    } else if (event->button == 1) { 
      Glib::RefPtr<Gtk::TreeSelection> ts = m_TreeView_Notebooks.get_selection ();
      Glib::RefPtr<Gtk::TreeModel> tm = ts->get_model ();
      Gtk::TreeModel::Path path;
      m_TreeView_Notebooks.get_path_at_pos ((gint) event->x, (gint) event->y, path);

      std::cout << "Event Button: " << event->button << std::endl;
      std::cout << "Path: " << path << std::endl;
      if (path.size () < 2) {
        return;
      }

      /* We get the children of this stack */
      Gtk::TreeModel::Row row = *(tm->get_iter (path));
      Gtk::TreeModel::Children children = row.children ();
      std::string query = "select * from notes where notebook_guid = '";
      std::vector <std::string> childGuids;
      /* Get Row and add a child row. */
      for (unsigned int i = 0; i < children.size (); i++) {
        Gtk::TreeModel::Row cr = children[i];
        NotebookData notebookDataItem = cr[m_refTreeModel->m_Columns.m_notebook_data];
        childGuids.push_back (notebookDataItem.getGuid ());
      }

      if (children.size () != 0) {
        app->nlpv->fetchNotesForNotebooks (childGuids);
      }


      /* We expand the tree if required. 
      if (m_TreeView_Notebooks.row_expanded (path)) {
        m_TreeView_Notebooks.collapse_row (path);
      } else {
        m_TreeView_Notebooks.expand_to_path (path);
        ts->select (Gtk::TreeModel::Path (selectedPath));
      }
      */
    }

}
template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}

void LeftPaneView::on_menu_file_popup_edit_notebook_name() {

  popup = new Gtk::MessageDialog (*app, "Enter new name for Notebook", true, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK_CANCEL, true);
  Gtk::Box* contentBox = popup->get_content_area ();
  Gtk::Box* firstBox = Gtk::manage (new Gtk::Box ());

  firstBox->pack_start (*notebookName, true, false, 0);
  firstBox->set_size_request (400, -1);
  contentBox->set_size_request (-1, -1);
  contentBox->add (*firstBox);

  contentBox->show_all ();
  popup->set_resizable (false);
  popup->set_modal (true);
  int reply = popup->run ();
  
  if (reply == Gtk::RESPONSE_OK) {
    std::cout << "Resonse ok." << std::endl;
    notebookEdit ();
    popup->hide ();
  } else if (reply == Gtk::RESPONSE_CANCEL) {
    std::cout << "Resonse cancel." << std::endl;
    popup->hide ();
  } else {
    std::cout << "Resonse else." << std::endl;
    popup->hide ();
  }

}

void LeftPaneView::on_menu_file_popup_delete_notebook () {

  popup = new Gtk::MessageDialog (*app, "Delete Notebook ? (This will delete all notes from that notebook !)", true, Gtk::MESSAGE_OTHER, Gtk::BUTTONS_OK_CANCEL, true);
  Gtk::Box* contentBox = popup->get_content_area ();
  contentBox->show_all ();
  popup->set_resizable (false);
  popup->set_modal (true);
  int reply = popup->run ();
  
  if (reply == Gtk::RESPONSE_OK) {
    std::cout << "Resonse ok." << std::endl;
    notebookDelete ();
    popup->hide ();
  } else if (reply == Gtk::RESPONSE_CANCEL) {
    std::cout << "Resonse cancel." << std::endl;
    notebookDeleteCancel ();
    popup->hide ();
  } else {
    std::cout << "Resonse else." << std::endl;
    popup->hide ();
  }
}


void LeftPaneView::notebookEdit () {
  if (notebookName->get_text().empty ()) { return;}

  std::string notebook_name = notebookName->get_text ();
  std::string notebook_id = selectedNotebook.getGuid ();

  if (notebook_id == "_")
  	return;

  dbm->exec ("update notebooks set title = '" + notebook_name + "' where guid = '" + notebook_id + "'", NULL, this);
  dbm->exec ("update notebooks set parent_guid = '"+ notebook_name + "' where parent_guid = '" + notebook_id + "'", NULL, this);
  refreshLeftPaneView ();
}


void LeftPaneView::notebookDelete () {
  std::string notebook_id = selectedNotebook.getGuid ();
  if (notebook_id == "_")
  	return;


  /* Ugly but works for now */
  dbm->exec ("delete from notes where notebook_guid = '" + notebook_id + "'", NULL, this);
  dbm->exec ("delete from notebooks where guid = '" + notebook_id + "'", NULL, this);
  dbm->exec ("update notebooks set parent_guid = '' where parent_guid = '" + notebook_id + "'", NULL, this);

  refreshLeftPaneView ();
}

void LeftPaneView::notebookDeleteCancel () {
  return;
}

void LeftPaneView::selectNotebookInPane (int pathIndex) {
  m_TreeView_Notebooks.get_selection ()->select (Gtk::TreeModel::Path ("0:" + NumberToString (pathIndex + 1)));
} 

void LeftPaneView::refreshLeftPaneView () {
  m_refTreeModel->clear ();  //Fill the TreeView's model
  notebooksRow = *(m_refTreeModel->append());
  notebooksRow[m_refTreeModel->m_Columns.m_col_id] = 0;
  notebooksRow[m_refTreeModel->m_Columns.m_col_name] = "Notebooks";
  /* id integer primary key, title text unique, guid text, stack text, created_time datetime, modified_time datetime */
  NotebookData* nbd = new NotebookData (-1, "Notebooks", "", "", 0, 0);
  notebooksRow[m_refTreeModel->m_Columns.m_notebook_data] = *nbd;
  notebooksRow[m_refTreeModel->m_Columns.m_col_draggable] = false;
  notebooksRow[m_refTreeModel->m_Columns.m_col_receivesdrags] = false;

  nbd = new NotebookData (0, "All Notebooks", "_", "", 0, 0);

  Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(notebooksRow.children()));
  childrow[m_refTreeModel->m_Columns.m_col_id] = 0;
  childrow[m_refTreeModel->m_Columns.m_col_name] = "All Notebooks";
  childrow[m_refTreeModel->m_Columns.m_notebook_data] = *nbd;
  childrow[m_refTreeModel->m_Columns.m_col_draggable] = false;
  childrow[m_refTreeModel->m_Columns.m_col_receivesdrags] = false;
  
  stacks.clear ();

  dbm->exec ("select * from notebooks where id > 0 order by title  ", &fillNotebooksCallback,this);
   m_TreeView_Notebooks.expand_all ();
  m_TreeView_Notebooks.get_selection ()->select (Gtk::TreeModel::Path ("0:0"));
  selectedPath = "0:0";
  notebookListSelected = true;
}

void LeftPaneView::updateParentGuid (std::string child, std::string child2, std::string stack) {
   dbm->exec ("update notebooks set parent_guid = '" + stack + "' where guid = '" + child + "' or guid = '" + child2 + "'", NULL, this);
   refreshLeftPaneView ();
}