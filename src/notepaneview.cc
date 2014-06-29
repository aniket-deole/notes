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
#include "notepaneview.hh"
/*
 * This files contains C Code as I was not able to find a good C++ gtkmm e
 * equivalent wrapper for webkit. Open to suggestions :)
 */

NotePaneView::NotePaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding) {
	set_orientation (Gtk::ORIENTATION_VERTICAL);

	Gtk::Box* toolbarBox = Gtk::manage (new Gtk::Box ());
	
	notebookButton = Gtk::manage (new Gtk::Button ());
    Gtk::Image* img = Gtk::manage (new Gtk::Image ("img/notebook.png"));
    notebookButton->set_image (*img);
	toolbarBox->pack_start (*notebookButton, false, false , 0);
	addCss (notebookButton, "notebookButton", " .notebookButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	notebookButton->set_size_request (30, 30);

	notebookName = Gtk::manage (new Gtk::Entry ());
	notebookName->set_text ("aniket's notebook");
	notebookName->set_has_frame (false);
	notebookName->set_editable (false);
	notebookName->set_can_focus (false);
	addCss (notebookName, "notebookName", ".notebookName {\n color:#000;\n font: OpenSans light 8; padding-top:10px;padding-bottom:10px; "
								" background-image:none; background-color:white;\n}\n"
								".noteTitle:selected {    background-color: #34393D; color:white ; }");
	toolbarBox->pack_start (*notebookName, false, false , 0);
	
	Gtk::Label* label = Gtk::manage (new Gtk::Label ("Delete"));
	addCss (label, "label", ".label {padding-right:10px; font: OpenSans light 8; color:#000;}");
	toolbarBox->pack_end (*label, false, false , 0);
	
	label = Gtk::manage (new Gtk::Label ("Info"));
	addCss (label, "label", ".label {padding-right:10px; font: OpenSans light 8; color:#000;}");
	toolbarBox->pack_end (*label, false, false , 0);
	
	label = Gtk::manage (new Gtk::Label ("Share"));
	addCss (label, "label", ".label {padding-right:10px; font: OpenSans light 8; color:#000;}");
	toolbarBox->pack_end (*label, false, false , 0);
#if HASPDF
	exportPdfButton = Gtk::manage (new Gtk::Button ("Export PDF"));
	addCss (exportPdfButton, "exportPdfButton", " .exportPdfButton{\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0; padding-right:10px; font: OpenSans light 8; color:#000;}");
  exportPdfButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::exportPdfButtonCallback) );
	toolbarBox->pack_end (*exportPdfButton, false, false , 0);
#else
	label = Gtk::manage (new Gtk::Label ("Reminder"));
	addCss (label, "label", ".label {padding-right:10px; font: OpenSans light 8; color:#000;}");
	toolbarBox->pack_end (*label, false, false , 0);
#endif /* HASPDF */

//  ex_label->signal_clicked().connect(
//  sigc::mem_fun(*this, &NotePaneView::saveNote) );

	toolbarBox->set_size_request (30, -1);	
	Gtk::EventBox* eventBoxTop = Gtk::manage (new Gtk::EventBox ());
	eventBoxTop->add (*toolbarBox);


	/* Toolbar */
	toolbarBox = Gtk::manage (new Gtk::Box ());
	
	boldButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/bold.png"));
	boldButton->set_image (*img);
  	boldButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::boldButtonCallback) );
	toolbarBox->pack_start (*boldButton, false, false , 0);
	addCss (boldButton, "boldButton", " .boldButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	boldButton->set_size_request (30, 30);

	italicsButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/italics.png"));
    italicsButton->set_image (*img);
  	italicsButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::italicsButtonCallback) );
	toolbarBox->pack_start (*italicsButton, false, false , 0);
	addCss (italicsButton, "italicsButton", " .italicsButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	italicsButton->set_size_request (30, 30);

	underlineButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/underline.png"));
    underlineButton->set_image (*img);
  	underlineButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::underlineButtonCallback) );
	toolbarBox->pack_start (*underlineButton, false, false , 0);
	addCss (underlineButton, "underlineButton", " .underlineButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	underlineButton->set_size_request (30, 30);

	strikeButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/strikethrough.png"));
    strikeButton->set_image (*img);
  	strikeButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::strikeButtonCallback) );
	toolbarBox->pack_start (*strikeButton, false, false , 0);
	addCss (strikeButton, "strikeButton", " .strikeButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	strikeButton->set_size_request (30, 30);

	clearFormattingButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/strikethrough.png"));
    clearFormattingButton->set_image (*img);
  	clearFormattingButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::clearFormattingButtonCallback) );
	toolbarBox->pack_start (*clearFormattingButton, false, false , 0);
	addCss (clearFormattingButton, "clearFormattingButton", " .clearFormattingButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	clearFormattingButton->set_size_request (30, 30);

	Gtk::Separator* separatorVertical = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_VERTICAL));
	toolbarBox->pack_start (*separatorVertical, false, false, 0);

	tabOutButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/tabout.png"));
    tabOutButton->set_image (*img);
  	tabOutButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::tabOutButtonCallback) );
	toolbarBox->pack_start (*tabOutButton, false, false , 0);
	addCss (tabOutButton, "tabOutButton", " .tabOutButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	tabOutButton->set_size_request (30, 30);

	tabInButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/tabin.png"));
    tabInButton->set_image (*img);
  	tabInButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::tabInButtonCallback));
	toolbarBox->pack_start (*tabInButton, false, false , 0);
	addCss (tabInButton, "tabInButton", " .tabInButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	tabInButton->set_size_request (30, 30);

	justifyButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/justify.png"));
    justifyButton->set_image (*img);
  	justifyButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::justifyButtonCallback));
	toolbarBox->pack_start (*justifyButton, false, false , 0);
	addCss (justifyButton, "justifyButton", " .justifyButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	justifyButton->set_size_request (30, 30);	

	justifyCenterButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/justify.png"));
    justifyCenterButton->set_image (*img);
  	justifyCenterButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::justifyCenterButtonCallback));
	toolbarBox->pack_start (*justifyCenterButton, false, false , 0);
	addCss (justifyCenterButton, "justifyCenterButton", " .justifyCenterButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	justifyCenterButton->set_size_request (30, 30);	

	justifyRightButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/justify.png"));
    justifyRightButton->set_image (*img);
  	justifyRightButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::justifyRightButtonCallback));
	toolbarBox->pack_start (*justifyRightButton, false, false , 0);
	addCss (justifyRightButton, "justifyRightButton", " .justifyRightButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	justifyRightButton->set_size_request (30, 30);	

	justifyFullButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/justify.png"));
    justifyFullButton->set_image (*img);
  	justifyFullButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::justifyFullButtonCallback));
	toolbarBox->pack_start (*justifyFullButton, false, false , 0);
	addCss (justifyFullButton, "justifyFullButton", " .justifyFullButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	justifyFullButton->set_size_request (30, 30);	

	separatorVertical = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_VERTICAL));
	toolbarBox->pack_start (*separatorVertical, false, false, 0);

	olistButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/olist.png"));
    olistButton->set_image (*img);
  	olistButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::olistButtonCallback) );
	toolbarBox->pack_start (*olistButton, false, false , 0);
	addCss (olistButton, "olistButton", " .olistButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	olistButton->set_size_request (30, 30);

	ulistButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/ulist.png"));
    ulistButton->set_image (*img);
  	ulistButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::ulistButtonCallback));
	toolbarBox->pack_start (*ulistButton, false, false , 0);
	addCss (ulistButton, "ulistButton", " .ulistButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	ulistButton->set_size_request (30, 30);

	clistButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/clist.png"));
    clistButton->set_image (*img);
  	clistButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::clistButtonCallback));
	toolbarBox->pack_start (*clistButton, false, false , 0);
	addCss (clistButton, "clistButton", " .clistButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	clistButton->set_size_request (30, 30);	

	Gtk::EventBox* eventBoxBot = Gtk::manage (new Gtk::EventBox ());
	eventBoxBot->add (*toolbarBox);
	
	addCss (eventBoxBot, "eventBoxBot", ".eventBoxBot { background-image:none; background-color:white;\n}\n");
	addCss (eventBoxTop, "eventBoxTop", ".eventBoxTop { background-image:none; background-color:white;\n}\n");
	pack_start (*eventBoxTop, false, false, 0);
	Gtk::Separator* sepTopmost = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_HORIZONTAL));
	pack_start (*sepTopmost, false, false, 0);
	pack_start (*eventBoxBot, false, false, 0);

	Gtk::Separator* sepTop = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_HORIZONTAL));
	pack_start (*sepTop, false, false, 0);

	noteTitle = Gtk::manage (new Gtk::Entry ());
	noteTitle->set_text ("NoteTitle");
	noteTitle->set_has_frame (false);
	noteTitle->set_editable (false);
	noteTitle->set_can_focus (false);
	noteTitle->signal_changed ().connect (sigc::mem_fun (*this, &NotePaneView::enableButtons));

	addCss (noteTitle, "noteTitle", ".noteTitle {\n color:#000;\n font: OpenSans light 18; padding-top:10px;padding-bottom:10px; "
								"padding-left:14px; background-image:none; background-color:white;\n}\n"
								".noteTitle:selected {    background-color: #34393D; color:white ; }");

	Gtk::EventBox* ev = Gtk::manage (new Gtk::EventBox ());

	Gtk::Box* noteTitleAndSaveButtonBox = Gtk::manage (new Gtk::Box ());


	Gtk::Separator* sepVert = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_VERTICAL));

	saveButton = Gtk::manage (new Gtk::Button ("  Save Note  "));
	addCss (saveButton, "saveButton", ".saveButton { background-image:none; background-color:white;"
		"border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;\n}\n");

  	saveButton->signal_clicked().connect(
    sigc::mem_fun(*this, &NotePaneView::saveNote) );

	noteTitleAndSaveButtonBox->pack_start (*noteTitle, true, true, 0);
	noteTitleAndSaveButtonBox->pack_start (*sepVert, false, false, 0);
	noteTitleAndSaveButtonBox->pack_end (*saveButton, false, false, 0);

	ev->add (*noteTitleAndSaveButtonBox);

	addCss (ev, "ev", ".ev { background-image:none; background-color:white;\n}\n");


	pack_start (*ev, false, false, 0);

	Gtk::Separator* sepBot = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_HORIZONTAL));
	pack_start (*sepBot, false, false, 0);

	webviewWrapper = Gtk::manage (new Gtk::ScrolledWindow ());

	webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
	
	GtkScrolledWindow* webviewWrapper_c = webviewWrapper->gobj ();
	gtk_container_add (GTK_CONTAINER (webviewWrapper_c), GTK_WIDGET (webview));

	webkit_web_view_set_editable(webview, false);
    
	webkit_web_view_load_string (webview, "Click the new note button to create a note.", "text/html", NULL, NULL);

	WebKitWebSettings* wkws = webkit_web_view_get_settings ( webview);
	g_object_set (G_OBJECT(wkws), "default-font-size", 10, NULL);	
	g_object_set (G_OBJECT(wkws), "enable-offline-web-application-cache", true, NULL);	
	g_object_set (G_OBJECT(wkws), "tab-key-cycles-through-elements", false, NULL);	


	webviewWrapper->set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	pack_start (*webviewWrapper);

	show_all_children ();
}

NotePaneView::~NotePaneView () {}


void NotePaneView::setDatabaseManager (DatabaseManager* d) {
    dbm = d;
}

void NotePaneView::setNote (NoteData n) {
	setWebViewContent (n.getBody ());
	setNoteTitleEntryText (n.getTitle ());
	notebookName->set_text (n.getNotebookName ());
	nd = n;
}

void NotePaneView::setNoteTitleEntryText (std::string content) {
	noteTitle->set_text (content);
}

void NotePaneView::setNoteNotebookTitleEntryText (std::string content) {
	notebookName->set_text (content);
}
void NotePaneView::setWebViewContent (std::string content) {
	webkit_web_view_load_string (webview, content.c_str (), "text/html", NULL, NULL);
}

void NotePaneView::newNote () {
	webkit_web_view_load_string (webview, "", "text/html", NULL, NULL);
}

void NotePaneView::disableButtons () {
	saveButton->set_sensitive (false);
	noteTitle->set_editable (false);
	webkit_web_view_set_editable(webview, false);
	noteTitle->set_can_focus (false);
}

void NotePaneView::enableButtons () {
	if (saveButton->get_sensitive () && noteTitle->get_editable ())
		return;
	saveButton->set_sensitive (true);
	noteTitle->set_editable (true);
	webkit_web_view_set_editable(webview, true);
	noteTitle->set_can_focus (true);

}

std::string
replaceSingleQuote( std::string const& original )
{
    std::string results(
        original.size() 
            + std::count( original.begin(), original.end(), '\''),
        '\'' );
    std::string::iterator dest = results.begin();
    for ( std::string::const_iterator current = original.begin();
            current != original.end();
            ++ current ) {
        if ( *current == '\'' ) {
            ++ dest;
        }
        *dest = *current;
        ++ dest;
    }
    return results;
}

template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}

void NotePaneView::saveNote () {

	std::string body = "";
	webkit_web_view_execute_script (webview, "document.title=document.documentElement.innerHTML;");
	body.append (webkit_web_frame_get_title ((webkit_web_view_get_main_frame (webview))));
	body = replaceSingleQuote (body);

#if 0
	// Create temporary file for saving to PDF
	std::ofstream temp_html_file;
	std::string temp_file_name = "/tmp/temp_"+nd.getGuid()+".html";
	temp_html_file.open(temp_file_name.c_str());
	temp_html_file << body;
	temp_html_file.close();
#endif /* HASPDF */
	std::string title = replaceSingleQuote (noteTitle->get_text ());
	std::cout << "saved: " << "update notes set title = '" + title + "', body = '" + body + "' where guid = '" << nd.getGuid () << "'" << std::endl;
  	dbm->exec ("update notes set title = '" + title + "', body = '" + body + "', modified_time = strftime('%s','now') where guid = '" + (nd.getGuid ()) + "'", NULL, this);

  	app->nlpv->fetchNotesForNotebooks (app->lpv->selectedNotebookGuids);
}

void NotePaneView::boldButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "bold", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::italicsButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "italic", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::underlineButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "underline", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}

void NotePaneView::strikeButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "strikeThrough", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}

void NotePaneView::clearFormattingButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "removeFormat", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::tabOutButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "indent", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}

void NotePaneView::tabInButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "outdent", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::justifyButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "justifyLeft", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::justifyCenterButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "justifyCenter", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::justifyRightButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "justifyRight", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::justifyFullButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "justifyFull", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::olistButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "insertOrderedList", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::ulistButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "insertUnorderedList", false, "");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
void NotePaneView::clistButtonCallback() {
    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
    webkit_dom_document_exec_command (dom, "insertHTML", false, "<input type='checkbox'></input>");
    gtk_widget_grab_focus (GTK_WIDGET (webview));
}
#if HASPDF
/* Print out loading progress information */
void progress_changed(wkhtmltopdf_converter * c, int p) {
	printf("%3d%%\r",p);
	fflush(stdout);
}

/* Print loading phase information */
void phase_changed(wkhtmltopdf_converter * c) {
	int phase = wkhtmltopdf_current_phase(c);
	printf("%s\n", wkhtmltopdf_phase_description(c, phase));
}

/* Print a message to stderr when an error occures */
void error(wkhtmltopdf_converter * c, const char * msg) {
	fprintf(stderr, "Error: %s\n", msg);
}

/* Print a message to stderr when a warning is issued */
void warning(wkhtmltopdf_converter * c, const char * msg) {
	fprintf(stderr, "Warning: %s\n", msg);
}

void NotePaneView::exportPdfButtonCallback() {
//    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
//    webkit_dom_document_exec_command (dom, "insertHTML", false, "<input type='checkbox'></input>");
//    gtk_widget_grab_focus (GTK_WIDGET (webview));

	std::cout << "Export Pdf Button clicked!" << std::endl;
	std::cout << "Current node guid: "<< nd.getGuid() << std::endl;
	wkhtmltopdf_global_settings * gs;
	wkhtmltopdf_object_settings * os;
	wkhtmltopdf_converter * c;

	std::string body = "";
	webkit_web_view_execute_script (webview, "document.title=document.documentElement.innerHTML;");
	body.append (webkit_web_frame_get_title ((webkit_web_view_get_main_frame (webview))));
	body = replaceSingleQuote (body);
	std::ofstream temp_html_file;
	std::string temp_file_name = "/tmp/temp_"+nd.getGuid()+".html";
	temp_html_file.open(temp_file_name.c_str());
	temp_html_file << body;
	temp_html_file.close();

	/* Init wkhtmltopdf in graphics less mode */
	wkhtmltopdf_init(false);
	std::string note_name = "/tmp/temp_" + nd.getGuid() + ".html";
	std::string note_title = noteTitle->get_text ()+".pdf";
	/*
	 * Create a global settings object used to store options that are not
	 * related to input objects, note that control of this object is parsed to
	 * the converter later, which is then responsible for freeing it
	 */
	gs = wkhtmltopdf_create_global_settings();
	/* We want the result to be storred in the file called test.pdf */
	wkhtmltopdf_set_global_setting(gs, "out", note_title.c_str());

	/*
	 * Create a input object settings object that is used to store settings
	 * related to a input object, note again that control of this object is parsed to
	 * the converter later, which is then responsible for freeing it
	 */
	os = wkhtmltopdf_create_object_settings();
	/* We want to convert to convert the qstring documentation page */
	// wkhtmltopdf_set_object_setting(os, "page", "http://www.google.com");
	// wkhtmltopdf_set_object_setting(os, "page", "/tmp/temp_5bac4912-faa3-11e3-aa14-000c29ee52ed.html");
	wkhtmltopdf_set_object_setting(os, "page", note_name.c_str());

	/* Create the actual converter object used to convert the pages */
	c = wkhtmltopdf_create_converter(gs);

	/* Call the progress_changed function when progress changes */
	wkhtmltopdf_set_progress_changed_callback(c, progress_changed);

	/* Call the phase _changed function when the phase changes */
	wkhtmltopdf_set_phase_changed_callback(c, phase_changed);

	/* Call the error function when an error occures */
	wkhtmltopdf_set_error_callback(c, error);

	/* Call the warning function when a warning is issued */
	wkhtmltopdf_set_warning_callback(c, warning);

	/*
	 * Add the the settings object describing the qstring documentation page
	 * to the list of pages to convert. Objects are converted in the order in which
	 * they are added
	 */
	wkhtmltopdf_add_object(c, os, NULL);

	/* Perform the actual convertion */
	if (!wkhtmltopdf_convert(c))
		fprintf(stderr, "Convertion failed!");

	/* Output possible http error code encountered */
	printf("httpErrorCode: %d\n", wkhtmltopdf_http_error_code(c));

	/* Destroy the converter object since we are done with it */
	wkhtmltopdf_destroy_converter(c);

	/* We will no longer be needing wkhtmltopdf funcionality */
	wkhtmltopdf_deinit();
	std::cout << "Export Pdf ended" << std::endl;
}
#endif /* HASPDF */
