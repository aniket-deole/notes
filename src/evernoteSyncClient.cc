#include "evernoteSyncClient.hh"
#include <libevernote/evernote.hh>
#include <dlfcn.h>
#include <iostream>
#include <string>
#include <cstring>

std::string consumerKey 					= "analogx";
std::string consumerSecret 				= "953b9c5d5f64c09b";
std::string requestTokenUrl 			= "https://sandbox.evernote.com/oauth";
std::string requestTokenQueryArgs = "oauth_callback=sandbox.evernote.com";
std::string authorizeUrl 					= "https://sandbox.evernote.com/OAuth.action";
std::string accessTokenUrl 				= "https://sandbox.evernote.com/oauth";

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}
std::string replaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

void EvernoteSyncClient::fourthStageComplete (WebKitWebView* webView,
		WebKitWebFrame* webFrame, gpointer userData) {
	std::string tokenData = "";
	webkit_web_view_execute_script (webView,
			"document.title=document.documentElement.innerHTML;");
	tokenData.append (webkit_web_frame_get_title (webFrame));
	
	std::cout << "Fourth:" << tokenData << std::endl;

}
	
void EvernoteSyncClient::thirdStageComplete (WebKitWebView* webView,
		WebKitWebFrame* webFrame, gpointer userData) {
		
	std::string tokenData = "";
	webkit_web_view_execute_script (webView,
			"document.title=document.documentElement.innerHTML;");
	tokenData.append (webkit_web_frame_get_title (webFrame));

	tokenData = replaceString (tokenData, "%3D", "=");
	tokenData = replaceString (tokenData, "%3A", ":");

	tokenData = tokenData.substr (tokenData.find ("oauth_token"));
	tokenData = tokenData.replace (tokenData.find ("<"), tokenData.length (), "");

	tokenData = replaceString (tokenData, "&amp;", "&");
	std::cout << "Third Loading:" << tokenData << std::endl;

	EvernoteSyncClient* esc = (EvernoteSyncClient*) userData;
    OAuthManager_generateAccessToken_t* OAuthManager_generateAccessToken_p = 
      (OAuthManager_generateAccessToken_t*) dlsym (esc->handle, "OAuthManager_generateAccessToken");

    std::string authToken = OAuthManager_generateAccessToken_p (esc->oAuthManager, tokenData);

    // load the symbols.
    createUserStore_t* createUserStore_p = (createUserStore_t*) dlsym(esc->handle, "createUserStore");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
    }
    evernote::UserStore* userStore = createUserStore_p ("sandbox.evernote.com", 80, "/edam/user", authToken);

    UserStore_getNoteStoreUrl_t* UserStore_getNoteStoreUrl_p = (UserStore_getNoteStoreUrl_t*) dlsym (esc->handle,"UserStore_getNoteStoreUrl");

    createNoteStore_t* createNoteStore_p = (createNoteStore_t*) dlsym (esc->handle, "createNoteStore");
    evernote::NoteStore* noteStore = createNoteStore_p (UserStore_getNoteStoreUrl_p (userStore, authToken));

    // std::cout << "*. List all notebooks\n";
    // std::cout << "----------------------------------------------------------------\n";
    NoteStore_listNotebooks_t* NoteStore_listNotebooks_p = (NoteStore_listNotebooks_t*) dlsym (esc->handle, "NoteStore_listNotebooks");
    std::vector<evernote::Notebook*>* notebookList = NoteStore_listNotebooks_p (noteStore, authToken);

    for (int i = 0; i < notebookList->size (); i++) {
         std::cout << notebookList->at (i)->stack << ":" << notebookList->at (i)->name << std::endl;
    }

    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "----------------------------------------------------------------\n";

    NoteStore_createNoteFilter_t* NoteStore_createNoteFilter_p = (NoteStore_createNoteFilter_t*) dlsym (esc->handle, "NoteStore_createNoteFilter");
    evernote::NoteFilter* nf = NoteStore_createNoteFilter_p ();
    NoteStore_createNotesMetadataResultSpec_t* NoteStore_createNotesMetadataResultSpec_p = 
        (NoteStore_createNotesMetadataResultSpec_t*) dlsym (esc->handle, "NoteStore_createNotesMetadataResultSpec");
    evernote::NotesMetadataResultSpec* nmrs = NoteStore_createNotesMetadataResultSpec_p ();
    nmrs->includeTitle = true;

    NoteStore_findNotesMetadata_t* NoteStore_findNotesMetadata_p = (NoteStore_findNotesMetadata_t*) dlsym (esc->handle, "NoteStore_findNotesMetadata");

    evernote::NotesMetadataList* nml = NoteStore_findNotesMetadata_p (noteStore, authToken, nf, 0, 20, nmrs);
    // std::cout << nml->totalNotes << std::endl;
    for (int i = 0; i < nml->notes.size (); i++) {
        // std::cout << nml->notes[i]->title << std::endl;
        // // std::cout << noteStore->getNoteContent (authToken, nml->notes[i]->guid);
       NoteStore_getNote_t* NoteStore_getNote_p = (NoteStore_getNote_t*) dlsym (esc->handle, "NoteStore_getNote");

       evernote::Note* note = NoteStore_getNote_p (noteStore, authToken, nml->notes[i]->guid, true, true, false, false);
       
       Note_enmlToHtml_t* Note_enmlToHtml_p = (Note_enmlToHtml_t*) dlsym (esc->handle, "Note_enmlToHtml");
       Note_enmlToHtml_p (note);

       std::cout << note->contentEnml << std::endl;
       std::cout << "-----------------------------";
       std::cout << "\n" << note->contentHtml << std::endl;
       std::cout << "-----------------------------\n";
       std::cout << "-----------------------------\n";
        for (int j = 0; j < note->resources.size (); j++) {
//            std::ofstream myfile;
//                  myfile.open ("asd");
//              myfile << note->resources[j]->data->body;
//              myfile.close();
          std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(note->resources[j]->data->body.c_str ()), note->resources[j]->data->size);

         //std::cout << "encoded: " << encoded << std::endl;                
        }
    }

    // Create a Note.
    NoteStore_createNote_t* NoteStore_createNote_p = (NoteStore_createNote_t*) dlsym (esc->handle, "NoteStore_createNote");
    evernote::Note* note = NoteStore_createNote_p ();
    note->title = "First UpSync Note.i fromapplicatoin";
    note->content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE en-note SYSTEM \"http://xml.evernote.com/pub/enml2.dtd\"><en-note> HelloWorld </en-note>";

    NoteStore_createNote2_t* NoteStore_createNote2_p = (NoteStore_createNote2_t*) dlsym (esc->handle, "NoteStore_createNote2");

    NoteStore_createNote2_p (noteStore, authToken, note);


}

void EvernoteSyncClient::secondStageComplete (WebKitWebView *webView,
		WebKitWebFrame* webFrame, gpointer userData) {

	std::string uri = webkit_web_view_get_uri (webView);
	
	if (uri.find ("oauth_verifier") == -1) {
		return;
	} else {
		std::cout << uri << std::endl;
	}

	EvernoteSyncClient* esc = (EvernoteSyncClient*) userData;
	g_signal_handler_disconnect (webView, esc->signalHandlerId);

	esc->signalHandlerId = g_signal_connect (webView, "document-load-finished",
			G_CALLBACK (&EvernoteSyncClient::thirdStageComplete), userData);

	// Extract pin from uri
	// i.e. extract oauth_verifier from
	// https://sandbox.evernote.com/sandbox.evernote.com?oauth_token=analogx.14E45C5D180.
	// 73616E64626F782E657665726E6F74652E636F6D.1E46AD4B8C702FB88068A3C66E410E08
	// &oauth_verifier=C910C86D82CF819E3E5DAF38683024FA&sandbox_lnb=false
	
	std::string pin = "";
	pin = uri.substr (uri.find ("oauth_verifier"));
	pin = pin.substr (pin.find ("=") + 1);
	pin = pin.replace (pin.find ("&"), pin.length (), "");

	std::cout << "Pin:" << pin << std::endl;

  OAuthManager_generateFinalAccessTokenUrl_t* OAuthManager_generateFinalAccessTokenUrl_p =
    (OAuthManager_generateFinalAccessTokenUrl_t*) dlsym (esc->handle, 
				"OAuthManager_generateFinalAccessTokenUrl");
  std::string fat = OAuthManager_generateFinalAccessTokenUrl_p (esc->oAuthManager, pin);

	esc->ewvb->hide ();
	esc->app->remove ();
	esc->app->add (*esc->app->windowBody);
	esc->app->show_all ();

	webkit_web_view_load_uri (webView, fat.c_str ());
	std::cout << "Second loading:" << fat << std::endl;	
	return;
}

void EvernoteSyncClient::firstStageComplete (WebKitWebView  *webView,
               WebKitWebFrame *webFrame,
               gpointer        userData) {
	std::string tokenData = "";
	webkit_web_view_execute_script (webView,
			"document.title=document.documentElement.innerHTML;");
	tokenData.append (webkit_web_frame_get_title (webFrame));


	tokenData = tokenData.substr (tokenData.find ("oauth_token"));
	
	tokenData = tokenData.replace (tokenData.find ("<"), tokenData.length (), "");

	std::cout << tokenData << std::endl;

	EvernoteSyncClient* esc = (EvernoteSyncClient*) userData;
	
	OAuthManager_generateAuthorizationUrl_t* OAuthManager_generateAuthorizationUrl_p =
		(OAuthManager_generateAuthorizationUrl_t*) dlsym (esc->handle, "OAuthManager_generateAuthorizationUrl");

	tokenData = replaceString (tokenData, "&amp;", "&");
	
	std::string authUrl = OAuthManager_generateAuthorizationUrl_p (esc->oAuthManager, tokenData);

	g_signal_handler_disconnect (webView, esc->signalHandlerId);
	esc->signalHandlerId = g_signal_connect (webView, "document-load-finished",
			G_CALLBACK (&EvernoteSyncClient::secondStageComplete), userData);
	webkit_web_view_load_uri (webView, authUrl.c_str ());
	esc->ewvb->show ();
	return;
}


int EvernoteSyncClient::sync () {
    /* Check if library is loaded. Else ask the user to install it. */
    handle = dlopen ("libevernote.so", RTLD_LAZY);
    if (!handle) {
      const char* dlsym_error = dlerror();
      std::cout << "Please Install libevernote.so along with its dependencies.\nError: " << 
				dlsym_error << std::endl;
      return 1;
    }

		createOAuthManager_t* createOAuthManager_p = (createOAuthManager_t*) dlsym (handle, 
				"createOAuthManager");

		oAuthManager = createOAuthManager_p (consumerKey,
				consumerSecret, requestTokenUrl, requestTokenQueryArgs, authorizeUrl,
				accessTokenUrl);

		OAuthManager_generateRequestTokenUrl_t* OAuthManager_generateRequestTokenUrl_p =
			(OAuthManager_generateRequestTokenUrl_t*) dlsym (handle, "OAuthManager_generateRequestTokenUrl");

		std::string rqu = OAuthManager_generateRequestTokenUrl_p (oAuthManager);

		std::cout << rqu << std::endl;

		// Hide the normal ui and get permissions from evernote.
		app->remove ();
		ewvb = Gtk::manage (new EvernoteWebViewBox (false, 0, Gtk::PACK_SHRINK, 0, app));
		ewvb->hide ();
		app->add (*ewvb);		
		
		webkit_web_view_load_uri (ewvb->webview, rqu.c_str ());
		signalHandlerId = g_signal_connect (ewvb->webview, "document-load-finished", 
				G_CALLBACK (&EvernoteSyncClient::firstStageComplete), this);
    
    return 0;
}
