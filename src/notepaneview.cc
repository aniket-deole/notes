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
#include <fstream>
#include <assert.h>
#include <openssl/md5.h>
#include <notepaneview.hh>
#include <utils.hh>

NotePaneView::NotePaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding, Notify* a, DatabaseManager* d) 
  : Gtk::Box (Gtk::ORIENTATION_VERTICAL, padding) {
	dbm = d;
  app = a;
	set_orientation (Gtk::ORIENTATION_VERTICAL);

	toolbarBoxUpper = Gtk::manage (new Gtk::Box ());
	
	notebookButton = Gtk::manage (new Gtk::Button ());
    Gtk::Image* img = Gtk::manage (new Gtk::Image ("img/notebook.png"));
    notebookButton->set_image (*img);
	toolbarBoxUpper->pack_start (*notebookButton, false, false , 0);
	addCss (notebookButton, "notebookButton", " .notebookButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	notebookButton->set_size_request (30, 30);

	notebookName = Gtk::manage (new Gtk::Label (""));
	notebookName->set_text ("aniket's notebook");
/*	notebookName->set_has_frame (false);
	notebookName->set_editable (false);
	notebookName->set_can_focus (false); */
	addCss (notebookName, "notebookName", ".notebookName {\n color:#000;\n font: OpenSans light 8; padding-top:10px;padding-bottom:10px; "
								" background-image:none; background-color:white;\n}\n"
								".noteTitle:selected {    background-color: #34393D; color:white ; }");
	toolbarBoxUpper->pack_start (*notebookName, false, false , 0);
	
	Gtk::Label* label = Gtk::manage (new Gtk::Label ("Delete"));
	addCss (label, "label", ".label {padding-right:10px; font: OpenSans light 8; color:#000;}");
	toolbarBoxUpper->pack_end (*label, false, false , 0);
	
	label = Gtk::manage (new Gtk::Label ("Info"));
	addCss (label, "label", ".label {padding-right:10px; font: OpenSans light 8; color:#000;}");
	toolbarBoxUpper->pack_end (*label, false, false , 0);
	
	label = Gtk::manage (new Gtk::Label ("Share"));
	addCss (label, "label", ".label {padding-right:10px; font: OpenSans light 8; color:#000;}");
	toolbarBoxUpper->pack_end (*label, false, false , 0);
#if HASPDF
	exportPdfButton = Gtk::manage (new Gtk::Button ("Export PDF"));
	addCss (exportPdfButton, "exportPdfButton", " .exportPdfButton{\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0; padding-right:10px; font: OpenSans light 8; color:#000;}");
    exportPdfButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::exportPdfButtonCallback) );
	toolbarBoxUpper->pack_end (*exportPdfButton, false, false , 0);
#else
	label = Gtk::manage (new Gtk::Label ("Reminder"));
	addCss (label, "label", ".label {padding-right:10px; font: OpenSans light 8; color:#000;}");
	toolbarBoxUpper->pack_end (*label, false, false , 0);
#endif /* HASPDF */

//  ex_label->signal_clicked().connect(
//  sigc::mem_fun(*this, &NotePaneView::saveNote) );

	toolbarBoxUpper->set_size_request (30, -1);	
	Gtk::EventBox* eventBoxTop = Gtk::manage (new Gtk::EventBox ());
	eventBoxTop->add (*toolbarBoxUpper);


	/* Toolbar */
	toolbarBoxLower = Gtk::manage (new Gtk::Box ());
	
	boldButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/bold.png"));
	boldButton->set_image (*img);
  	boldButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::boldButtonCallback) );
	toolbarBoxLower->pack_start (*boldButton, false, false , 0);
	addCss (boldButton, "boldButton", " .boldButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	boldButton->set_size_request (30, 30);
	italicsButton->set_tooltip_text("Bold");

	italicsButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/italics.png"));
    italicsButton->set_image (*img);
  	italicsButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::italicsButtonCallback) );
	toolbarBoxLower->pack_start (*italicsButton, false, false , 0);
	addCss (italicsButton, "italicsButton", " .italicsButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	italicsButton->set_size_request (30, 30);
	italicsButton->set_tooltip_text("Italics");

	underlineButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/underline.png"));
    underlineButton->set_image (*img);
  	underlineButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::underlineButtonCallback) );
	toolbarBoxLower->pack_start (*underlineButton, false, false , 0);
	addCss (underlineButton, "underlineButton", " .underlineButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	underlineButton->set_size_request (30, 30);
	underlineButton->set_tooltip_text("Underline");

	strikeButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/strikethrough.png"));
    strikeButton->set_image (*img);
  	strikeButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::strikeButtonCallback) );
	toolbarBoxLower->pack_start (*strikeButton, false, false , 0);
	addCss (strikeButton, "strikeButton", " .strikeButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	strikeButton->set_size_request (30, 30);
	strikeButton->set_tooltip_text("Strikethrough");

	clearFormattingButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/strikethrough.png"));
    clearFormattingButton->set_image (*img);
  	clearFormattingButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::clearFormattingButtonCallback) );
	toolbarBoxLower->pack_start (*clearFormattingButton, false, false , 0);
	addCss (clearFormattingButton, "clearFormattingButton", " .clearFormattingButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	clearFormattingButton->set_size_request (30, 30);
	clearFormattingButton->set_tooltip_text("Clear Formatting");

	Gtk::Separator* separatorVertical = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_VERTICAL));
	toolbarBoxLower->pack_start (*separatorVertical, false, false, 0);

	tabOutButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/tabout.png"));
    tabOutButton->set_image (*img);
  	tabOutButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::tabOutButtonCallback) );
	toolbarBoxLower->pack_start (*tabOutButton, false, false , 0);
	addCss (tabOutButton, "tabOutButton", " .tabOutButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	tabOutButton->set_size_request (30, 30);
	tabOutButton->set_tooltip_text("Tab Out");

	tabInButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/tabin.png"));
    tabInButton->set_image (*img);
  	tabInButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::tabInButtonCallback));
	toolbarBoxLower->pack_start (*tabInButton, false, false , 0);
	addCss (tabInButton, "tabInButton", " .tabInButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	tabInButton->set_size_request (30, 30);
	tabInButton->set_tooltip_text("Tab In");

	justifyButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/justify.png"));
    justifyButton->set_image (*img);
  	justifyButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::justifyButtonCallback));
	toolbarBoxLower->pack_start (*justifyButton, false, false , 0);
	addCss (justifyButton, "justifyButton", " .justifyButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	justifyButton->set_size_request (30, 30);	
	justifyButton->set_tooltip_text("Justify");

	justifyCenterButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/justify.png"));
    justifyCenterButton->set_image (*img);
  	justifyCenterButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::justifyCenterButtonCallback));
	toolbarBoxLower->pack_start (*justifyCenterButton, false, false , 0);
	addCss (justifyCenterButton, "justifyCenterButton", " .justifyCenterButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	justifyCenterButton->set_size_request (30, 30);	
	justifyCenterButton->set_tooltip_text("Justify Center");

	justifyRightButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/justify.png"));
    justifyRightButton->set_image (*img);
  	justifyRightButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::justifyRightButtonCallback));
	toolbarBoxLower->pack_start (*justifyRightButton, false, false , 0);
	addCss (justifyRightButton, "justifyRightButton", " .justifyRightButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	justifyRightButton->set_size_request (30, 30);	
	justifyRightButton->set_tooltip_text("Justify Right");

	justifyFullButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/justify.png"));
    justifyFullButton->set_image (*img);
  	justifyFullButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::justifyFullButtonCallback));
	toolbarBoxLower->pack_start (*justifyFullButton, false, false , 0);
	addCss (justifyFullButton, "justifyFullButton", " .justifyFullButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	justifyFullButton->set_size_request (30, 30);	
	justifyFullButton->set_tooltip_text("Justify Full");

	separatorVertical = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_VERTICAL));
	toolbarBoxLower->pack_start (*separatorVertical, false, false, 0);

	olistButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/olist.png"));
    olistButton->set_image (*img);
  	olistButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::olistButtonCallback) );
	toolbarBoxLower->pack_start (*olistButton, false, false , 0);
	addCss (olistButton, "olistButton", " .olistButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	olistButton->set_size_request (30, 30);
	olistButton->set_tooltip_text("Ordered List");

	ulistButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/ulist.png"));
    ulistButton->set_image (*img);
  	ulistButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::ulistButtonCallback));
	toolbarBoxLower->pack_start (*ulistButton, false, false , 0);
	addCss (ulistButton, "ulistButton", " .ulistButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	ulistButton->set_size_request (30, 30);
	ulistButton->set_tooltip_text("Unordered List");

	clistButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/clist.png"));
    clistButton->set_image (*img);
  	clistButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::clistButtonCallback));
	toolbarBoxLower->pack_start (*clistButton, false, false , 0);
	addCss (clistButton, "clistButton", " .clistButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	clistButton->set_size_request (30, 30);	
	clistButton->set_tooltip_text("C List");

	separatorVertical = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_VERTICAL));
	toolbarBoxLower->pack_start (*separatorVertical, false, false, 0);

	insertImageButton = Gtk::manage (new Gtk::Button ());
    img = Gtk::manage (new Gtk::Image ("img/clist.png"));
    insertImageButton->set_image (*img);
  	insertImageButton->signal_clicked().connect(
    		sigc::mem_fun(*this, &NotePaneView::insertImageButtonCallback));
	toolbarBoxLower->pack_start (*insertImageButton, false, false , 0);
	addCss (insertImageButton, "insertImageButton", " .insertImageButton {\n background-color:white; background-image: none;  border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;}");
	insertImageButton->set_size_request (30, 30);	
	insertImageButton->set_tooltip_text("Insert Image");



	Gtk::EventBox* eventBoxBot = Gtk::manage (new Gtk::EventBox ());
	eventBoxBot->add (*toolbarBoxLower);
	
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
								"border-color:white; padding-left:7px; background-image:none; background-color:white;\n}\n"
								".noteTitle:selected {   border-color:white; background-color: #34393D; color:white ; }");

	Gtk::EventBox* ev = Gtk::manage (new Gtk::EventBox ());

	Gtk::Box* noteTitleAndSaveButtonBox = Gtk::manage (new Gtk::Box ());


	Gtk::Separator* sepVert = Gtk::manage (new Gtk::Separator (Gtk::ORIENTATION_VERTICAL));

	saveButton = Gtk::manage (new Gtk::Button ("  Save Note  "));
	addCss (saveButton, "saveButton", ".saveButton {"
		"border-radius: 0px; border: 0px solid; -unico-inner-stroke-width: 0px;"
    "	-unico-outer-stroke-width: 0px;-GtkButton-inner-border: 0;\n}\n");

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



int NotePaneView::getImageCallback (void* npv, int argc, char **argv, char **azColName) {
	NotePaneView* p = (NotePaneView*) npv;
  std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(argv[0]), atoi (argv[1]));
  std::string data = "data:image/png;base64,";
  data.append (encoded);
  encoded.clear ();
      char *rootNode = p->gDoc->allocate_string(data.c_str ()); 
	rapidxml::xml_attribute<> *attr = p->gDoc->allocate_attribute("src", rootNode);
	p->gRoot->append_attribute(attr);
	
	return 0;
}

void NotePaneView::getImageDataForNote (rapidxml::xml_node<>* root, rapidxml::xml_document<>* doc) {
    char* rootNodeName = root->name ();
    if (!strcmp (rootNodeName, "img")) {
        for (rapidxml::xml_attribute<> *attr = root->first_attribute(); attr; 
            attr = attr->next_attribute ()) {
            char* attrName= attr->name ();
            if (!strcmp (attrName, "hash")) {
            	/*
            	 Fetch Resource From DB 
            	 */
            	 gRoot = root;
            	 gDoc = doc;
            	 std::string query = "select body, size from resources where hash = '";

            	 query.append (attr->value ());
            	 query.append ("' limit 1");
            	 
        	 	 dbm->exec (query, &getImageCallback, this);
            }
        }
    }

    for (rapidxml::xml_node<> *child = root->first_node (); child;
            child = child->next_sibling ()) {
        getImageDataForNote (child,doc);
    }
}

int NotePaneView::getBodyCallback (void* npv, int argc, char **argv, char **azColName) {
  NotePaneView* p = (NotePaneView*) npv;
  p->noteBody = argv[0];
  return 0;
}

void NotePaneView::setNote (NoteData n) {
	/*
   * Get Note from the db
   */
  std::string query = "select body from notes where guid = '";
  query.append (n.getGuid ());
  query.append ("'");
  dbm->exec (query, &getBodyCallback, this);
  n.setBody (noteBody);

  /*
	 Get Resources from the db.
	 Check if html is appended or no. 
	 */
	std::string htmlTag = "<html>";
	if (n.getBodyPointer ()->substr (0, htmlTag.size ()) != htmlTag) {
		n.getBodyPointer ()->insert (0, htmlTag);
		n.getBodyPointer ()->append ("</html>");
	}

	rapidxml::xml_document<> doc;
    char *cstr = new char[n.getBody ().length() + 1];
    strcpy(cstr, n.getBody ().c_str());
    doc.parse<0> (cstr);
    char *rootNode = doc.allocate_string("html");        // Allocate string and copy name into it
    doc.first_node ()->name (rootNode);


    getImageDataForNote
     (doc.first_node (), &doc);

    std::string bodyContent = "";
    rapidxml::print(std::back_inserter(bodyContent), doc);
    n.setBody (bodyContent);
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

void removeImageDataFromNotes (rapidxml::xml_node<>* root) {
    char* rootNodeName = root->name ();
    if (!strcmp (rootNodeName, "img")) {
        for (rapidxml::xml_attribute<> *attr = root->first_attribute(); attr; 
            attr = attr->next_attribute ()) {
            char* attrName= attr->name ();
            if (!strcmp (attrName, "src")) {
            	root->remove_attribute (attr);
            	
            }
        }
    }

    for (rapidxml::xml_node<> *child = root->first_node (); child;
            child = child->next_sibling ()) {
        removeImageDataFromNotes (child);
    }
}


void NotePaneView::saveNote () {

	std::string body = "";
	webkit_web_view_execute_script (webview, "document.title=document.documentElement.innerHTML;");
	body.append (webkit_web_frame_get_title ((webkit_web_view_get_main_frame (webview))));
  
  #if 0
	// Create temporary file for saving to PDF
	std::ofstream temp_html_file;
	std::string temp_file_name = "/tmp/temp_"+nd.getGuid()+".html";
	temp_html_file.open(temp_file_name.c_str());
	temp_html_file << body;
	temp_html_file.close();
#endif /* HASPDF */
	std::string title = replaceSingleQuote (noteTitle->get_text ());
	std::string htmlTag = "<html>";
	if (body.substr (0, htmlTag.size ()) != htmlTag) {
		body.insert (0, htmlTag);
		body.append ("</html>");
	}
	// remove the base64 hashed out images.

	rapidxml::xml_document<> doc;
    char *cstr = new char[body.length() + 1];
    strcpy(cstr, body.c_str());
    doc.parse<0> (cstr);
    char *rootNode = doc.allocate_string("html");        // Allocate string and copy name into it
    doc.first_node ()->name (rootNode);


    removeImageDataFromNotes (doc.first_node ());

    body.clear ();

    rapidxml::print(std::back_inserter(body), doc);

	body = replaceSingleQuote (body);

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

void convert_md5_sum(unsigned char* md, std::string* md5Result) {
    int i;
    char buf[32];
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
	    sprintf(buf, "%02x", md[i]);
	    md5Result->append( buf );
    }
}

void NotePaneView::insertImageButtonCallback () {
	Gtk::FileChooserDialog dialog("Please choose an image",
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*app);  
	//Add response buttons the the dialog:
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("_Open", Gtk::RESPONSE_OK);

  int result = dialog.run();
  Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);
  //Handle the response:
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      
		//Notice that this is a std::string, not a Glib::ustring.
		std::string filename = dialog.get_filename();
		

		std::ifstream file(filename.c_str (), std::ios::in | std::ios::binary);
		if (!file) {
		    std::cerr << "An error occurred opening the file\n";
		}
		file.seekg(0, std::ifstream::end);
		std::streampos size = file.tellg();
		file.seekg(0);

		char* buffer = new char[size];
		file.read(buffer, size);


  		sqlite3_stmt *pStmt;
  		const char *zSql = "INSERT INTO resources (noteguid, hash, size, body, mime) VALUES(?, ?, ?, ?, ?)";
 		int rc = sqlite3_prepare_v2(dbm->db, zSql, -1, &pStmt, 0);

		// Calculate md5
        unsigned char result[MD5_DIGEST_LENGTH];
        std::string* md5Result = new std::string ("");
		MD5(reinterpret_cast<const unsigned char*>(buffer), size, result);
     	convert_md5_sum(result, md5Result);
     	

		sqlite3_bind_text(pStmt, 1, nd.getGuid ().c_str (), -1, SQLITE_STATIC);
		sqlite3_bind_text(pStmt, 2, md5Result->c_str (), -1, SQLITE_STATIC);
		sqlite3_bind_int(pStmt, 3, size);
		sqlite3_bind_blob(pStmt, 4, buffer, size, SQLITE_STATIC);
		sqlite3_bind_text(pStmt, 5, "image/png", -1, SQLITE_STATIC);

		/* Call sqlite3_step() to run the virtual machine. Since the SQL being
		** executed is not a SELECT statement, we assume no data will be returned.
		*/
		rc = sqlite3_step(pStmt);
		assert( rc!=SQLITE_ROW );

		/* Finalize the virtual machine. This releases all memory and other
		** resources allocated by the sqlite3_prepare() call above.
		*/
		rc = sqlite3_finalize(pStmt);

		// Encode to base64
		std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(buffer), size);

     	// Insert the new image in the note body.
		std::string insertImageHtml;
		insertImageHtml.append ("<img alt=\"Hello!\" hash=\"");
		insertImageHtml.append (*md5Result);
		insertImageHtml.append ("\" src=\"data:image/jpeg;base64,");
		insertImageHtml.append (encoded);
		insertImageHtml.append ("\"></img>");

	    WebKitDOMDocument* dom = webkit_web_view_get_dom_document (webview);
	    webkit_dom_document_exec_command (dom, "insertHTML", false, insertImageHtml.c_str ());
	    gtk_widget_grab_focus (GTK_WIDGET (webview));


      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      
      break;
    }
    default:
    {
      
      break;
    }
  } 
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
	
}
#endif /* HASPDF */
