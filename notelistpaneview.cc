#include <gtkmm/box.h>
#include <gtkmm/widget.h>
#include <gtkmm.h>
#include <iostream>
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
    font_from.set_size (12 * Pango::SCALE);
    Glib::RefPtr <Pango::Layout> layout_from = widget.create_pango_layout ("");
    layout_from->set_font_description (font_from);
    layout_from->set_markup ("<span foreground='black'>" + property_note_.get_value ().getTitle () + "</span>");
    layout_from->set_width(210 * Pango::SCALE);
    cr->move_to (10, 5 + cell_area.get_y ());
    layout_from->show_in_cairo_context (cr);

    font_from.set_size (8 * Pango::SCALE);
    layout_from = widget.create_pango_layout ("");
    layout_from->set_font_description (font_from);
    layout_from->set_alignment(Pango::ALIGN_RIGHT);
    layout_from->set_markup ("<span foreground='#AAA'>" + property_note_.get_value ().getRemaining () + "</span>");
    layout_from->set_width(90* Pango::SCALE);
    cr->move_to (cell_area.get_width () - 90, 10 + cell_area.get_y ());
    layout_from->show_in_cairo_context (cr);

    font_from.set_size (10 * Pango::SCALE);
    layout_from = widget.create_pango_layout ("");
    layout_from->set_font_description (font_from);
    layout_from->set_alignment(Pango::ALIGN_LEFT);
    layout_from->set_markup ("<span foreground='#555'>" + property_note_.get_value ().getSummary () + "</span>");
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


Gtk::TreeViewColumn* create_column (Gtk::TreeModelColumn<int> tmc, Gtk::TreeModelColumn<NoteData> n) {
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
  std::string cssProperties = ".m_TreeView { background-color: white;     border-radius: 0;     color: white; } "
" .m_TreeView:selected, .m_TreeView:prelight:selected, .m_TreeView.category-expander:hover {      color: white;     border-style: solid;     border-width: 1px 0 1px 0; "
"     -unico-inner-stroke-width: 1px 0 1px 0; background-image: -gtk-gradient (linear, left top, left bottom, from (alpha (#000, 0.11)), to (alpha (#000, 0.07)));     " 
"   -unico-border-gradient: -gtk-gradient (linear,                     left top, left bottom,                     "
"     from (alpha (#fff, 0.070)), "
"   to (alpha (#fff, 0.10)));      -unico-inner-stroke-gradient: -gtk-gradient (linear,                     left top, left bottom, "
"   from (alpha (#000, 0.03)),                     to (alpha (#000, 0.10))); }  "
" .m_TreeView:selected:backdrop, .m_TreeView:prelight:selected:backdrop {     background-image: -gtk-gradient (linear,"
"   left top,                     left bottom,                     from (alpha (#000, 0.08)),                     to (alpha (#000, 0.04)));      -unico-border-gradient: -gtk-gradient (linear,       "
"   left top, left bottom,                     from (alpha (#000, 0.19)),                     to (alpha (#fff, 0.25)));      "
"   -unico-inner-stroke-gradient: -gtk-gradient (linear,                     left top, left bottom,                     from (alpha (#000, 0.03)),                     to (alpha (#000, 0.10)));  } "
" .m_TreeView:prelight { background-color: shade (@bg_color, 1.10); }"
" .m_TreeView:hover { color: white; border-style: solid; border-width: 1px 0 1px 0; -unico-inner-stroke-width: 1px 0 1px 0; "
"   background-image: -gtk-gradient (linear, left top, left bottom, from (alpha (#FFF, 0.2)), to (alpha (#FFF, 0.2))); "
"   -unico-border-gradient: -gtk-gradient (linear, left top, left bottom, from (alpha (#fff, 0.3)), to (alpha (#fff, 0.30)));}";

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
  row[m_Columns.m_col_name] = "id";
  NoteData n1 ("First", "14:53", "Summary");
  row[m_Columns.m_note_data] = n1;


  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 2;
  row[m_Columns.m_col_name] = "Tags";
  NoteData n2 ("Second", "11:09", "Summary");
  row[m_Columns.m_note_data] = n2;
  
  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 3;
  row[m_Columns.m_col_name] = "NTags";
  NoteData n3 ("Third", "Yesterday", "Summary");
  row[m_Columns.m_note_data] = n3;

  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_id] = 4;
  row[m_Columns.m_col_name] = "NTagsa";
  NoteData n4("Fourth", "4 Days Ago", "Summary");
  row[m_Columns.m_note_data] = n4;  

  m_TreeView.append_column(*create_column (m_Columns.m_col_id, m_Columns.m_note_data));

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
