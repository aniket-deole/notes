#include <dlfcn.h>
#include <iostream>
#include <string>
#include <cstring>
#include <libevernote/evernote.hh>
#include <evernoteSyncClient.hh>

#include <utils.hh>

std::string consumerKey 					= "analogx";
std::string consumerSecret 				= "953b9c5d5f64c09b";
std::string requestTokenUrl 			= "https://www.evernote.com/oauth";
std::string requestTokenQueryArgs = "oauth_callback=www.evernote.com";
std::string authorizeUrl 					= "https://www.evernote.com/OAuth.action";
std::string accessTokenUrl 				= "https://www.evernote.com/oauth";
std::string evernoteServer        = "www.evernote.com";

std::string consumerKeySandbox 					  = "analogx";
std::string consumerSecretSandbox 				= "953b9c5d5f64c09b";
std::string requestTokenUrlSandbox 			  = "https://sandbox.evernote.com/oauth";
std::string requestTokenQueryArgsSandbox  = "oauth_callback=sandbox.evernote.com";
std::string authorizeUrlSandbox 					= "https://sandbox.evernote.com/OAuth.action";
std::string accessTokenUrlSandbox 				= "https://sandbox.evernote.com/oauth";
std::string evernoteServerSandbox         = "sandbox.evernote.com";

void EvernoteSyncClient::getNoteDataObject (evernote::Note* note) {

}

void EvernoteSyncClient::updateUpdateSequenceNumInDatabase (long updateSequenceNumber) {
  sqlite3_stmt *pStmt;
  const char *zSql = "INSERT or replace INTO system_parameters (parameter, value) VALUES('evernoteUpdateSequenceNumber',?)";
  int rc = sqlite3_prepare_v2(app->dbm->db, zSql, -1, &pStmt, 0);

  sqlite3_bind_int (pStmt, 1, updateSequenceNumber);

  /* Call sqlite3_step() to run the virtual machine. Since the SQL being
   ** executed is not a SELECT statement, we assume no data will be returned.
   */
  rc = sqlite3_step(pStmt);
  assert( rc!=SQLITE_ROW );

  /* Finalize the virtual machine. This releases all memory and other
   ** resources allocated by the sqlite3_prepare() call above.
   */
  rc = sqlite3_finalize(pStmt);

}

void EvernoteSyncClient::syncNotes (long updateSequenceNumber) {
  NoteStore_createNoteFilter_t* NoteStore_createNoteFilter_p = (NoteStore_createNoteFilter_t*) dlsym (handle, "NoteStore_createNoteFilter");
  evernote::NoteFilter* nf = NoteStore_createNoteFilter_p ();
  NoteStore_createNotesMetadataResultSpec_t* NoteStore_createNotesMetadataResultSpec_p = 
    (NoteStore_createNotesMetadataResultSpec_t*) dlsym (handle, "NoteStore_createNotesMetadataResultSpec");
  evernote::NotesMetadataResultSpec* nmrs = NoteStore_createNotesMetadataResultSpec_p ();
  nmrs->includeTitle = true;
  nmrs->includeUpdateSequenceNum = true;

  NoteStore_findNotesMetadata_t* NoteStore_findNotesMetadata_p = (NoteStore_findNotesMetadata_t*) dlsym (handle, "NoteStore_findNotesMetadata");

  evernote::NotesMetadataList* nml = NoteStore_findNotesMetadata_p (noteStore, authToken, nf, 0, 20, nmrs);
  for (int i = 0; i < (int) nml->notes.size (); i++) {
    NoteStore_getNote_t* NoteStore_getNote_p = (NoteStore_getNote_t*) dlsym (handle, "NoteStore_getNote");

    evernote::Note* note = NoteStore_getNote_p (noteStore, authToken, nml->notes[i]->guid, true, true, false, false);

    Note_enmlToHtml_t* Note_enmlToHtml_p = (Note_enmlToHtml_t*) dlsym (handle, "Note_enmlToHtml");
    Note_enmlToHtml_p (note);


    sqlite3_stmt *pStmt;
    const char *zSql = "INSERT INTO notes (title, body, created_time, modified_time, guid, notebook_guid,usn, dirty) VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
    int rc = sqlite3_prepare_v2(app->dbm->db, zSql, -1, &pStmt, 0);


    sqlite3_bind_text(pStmt, 1, replaceSingleQuote (note->title).c_str (), -1, SQLITE_STATIC);
    std::string contentHtml = replaceSingleQuote (note->contentHtml);
    sqlite3_bind_text(pStmt, 2, contentHtml.c_str (), -1, SQLITE_STATIC);
    sqlite3_bind_int(pStmt, 3, 0);
    sqlite3_bind_int(pStmt, 4, 0);
    sqlite3_bind_text(pStmt, 5, note->guid->guid.c_str (), -1, SQLITE_STATIC);
    sqlite3_bind_text(pStmt, 6, note->notebookGuid.c_str (), -1, SQLITE_STATIC);
    sqlite3_bind_int(pStmt, 7, 0);
    sqlite3_bind_int(pStmt, 8, 0);

    /* Call sqlite3_step() to run the virtual machine. Since the SQL being
     ** executed is not a SELECT statement, we assume no data will be returned.
     */
    rc = sqlite3_step(pStmt);
    assert( rc!=SQLITE_ROW );

    updateUpdateSequenceNumInDatabase (nml->notes[i]->updateSequenceNum);

    /* Finalize the virtual machine. This releases all memory and other
     ** resources allocated by the sqlite3_prepare() call above.
     */
    rc = sqlite3_finalize(pStmt);


    /*    std::cout << note->contentEnml << std::endl;
          std::cout << "-----------------------------";
          std::cout << "\n" << note->contentHtml << std::endl;
          std::cout << "-----------------------------\n";
          std::cout << "-----------------------------\n";
          */    for (int j = 0; j < (int) note->resources.size (); j++) {
            std::string encoded = 
              base64_encode(reinterpret_cast<const unsigned char*>(note->resources[j]->data->body.c_str ()), 
                  note->resources[j]->data->size);
          }
          delete note;
  }
  /*
  // Create a Note.
  NoteStore_createNote_t* NoteStore_createNote_p = (NoteStore_createNote_t*) dlsym (handle, "NoteStore_createNote");
  evernote::Note* note = NoteStore_createNote_p ();
  note->title = "First UpSync Note.i fromapplicatoin";
  note->content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE en-note SYSTEM \"http://xml.evernote.com/pub/enml2.dtd\"><en-note> HelloWorld </en-note>";

  NoteStore_createNote2_t* NoteStore_createNote2_p = (NoteStore_createNote2_t*) dlsym (handle, "NoteStore_createNote2");

  NoteStore_createNote2_p (noteStore, authToken, note);
  */
  app->nlpv->fetchNotesForNotebook ("");

}

void EvernoteSyncClient::syncNotebooks (long updateSequenceNumber) {

  getNoteStore ();

  NoteStore_listNotebooks_t* NoteStore_listNotebooks_p = (NoteStore_listNotebooks_t*) dlsym (handle, "NoteStore_listNotebooks");
  std::vector<evernote::Notebook*>* notebookList = NoteStore_listNotebooks_p (noteStore, authToken);

  for (int i = 0; i < (int) notebookList->size (); i++) {
    NotebookData* notebook = new NotebookData (0, notebookList->at(i)->name, 
        notebookList->at (i)->guid->guid, notebookList->at (i)->stack, 
        0, 0, 0);
    app->dbm->exec (notebook->getInsertStatement (), NULL, this);
    updateUpdateSequenceNumInDatabase (notebookList->at (i)->updateSequenceNum);
    delete notebook;
  }


  app->lpv->refreshLeftPaneView ();
}

int EvernoteSyncClient::getUserStore (int domain) {
  createUserStore_t* createUserStore_p = (createUserStore_t*) dlsym(handle, "createUserStore");
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
    return -1;
  }
  if (domain == 2) 
    userStore = createUserStore_p (evernoteServer, 80, "/edam/user", authToken);
  else if (domain == 1)
    userStore = createUserStore_p (evernoteServerSandbox, 80, "/edam/user", authToken);

  return 0;
}

int EvernoteSyncClient::getNoteStore () {
  if (noteStore == NULL) {
    UserStore_getNoteStoreUrl_t* UserStore_getNoteStoreUrl_p = (UserStore_getNoteStoreUrl_t*) 
      dlsym (handle,"UserStore_getNoteStoreUrl");
    createNoteStore_t* createNoteStore_p = (createNoteStore_t*) dlsym (handle, "createNoteStore");
    try {
      std::string noteStoreUrl = UserStore_getNoteStoreUrl_p (userStore, authToken);
      noteStore = createNoteStore_p (server, noteStoreUrl);
    } catch (evernote::edam::EDAMSystemException e) {
      std::cout << "Exception occrerd."<< std::endl;
      exit (0);
    }

    NoteStore_listNotebooks_t* NoteStore_listNotebooks_p = (NoteStore_listNotebooks_t*) dlsym (handle, "NoteStore_listNotebooks");
    std::vector<evernote::Notebook*>* notebookList = NoteStore_listNotebooks_p (noteStore, authToken);
  }
  return 0; 
}

void EvernoteSyncClient::saveResource (evernote::Resource* resource) {
  sqlite3_stmt *pStmt;
  const char *zSql = "INSERT INTO resources (noteguid, hash, size, body, mime) VALUES(?, ?, ?, ?, ?)";
  int rc = sqlite3_prepare_v2(app->dbm->db, zSql, -1, &pStmt, 0);
  std::string* asciiHash = new std::string ("");
  convert_md5_sum((unsigned char*) resource->data->bodyHash.c_str (), asciiHash);

  sqlite3_bind_text(pStmt, 1, resource->noteGuid->guid.c_str (), -1, SQLITE_STATIC);
  sqlite3_bind_text(pStmt, 2, 
      asciiHash->c_str (), -1, SQLITE_STATIC);
  sqlite3_bind_int(pStmt, 3, 
      resource->data->size);
  sqlite3_bind_blob(pStmt, 4, resource->data->body.c_str (), 
      resource->data->size, SQLITE_STATIC);
  sqlite3_bind_text(pStmt, 5, resource->mime.c_str (), -1, SQLITE_STATIC);

  /* Call sqlite3_step() to run the virtual machine. Since the SQL being
   ** executed is not a SELECT statement, we assume no data will be returned.
   */
  rc = sqlite3_step(pStmt);
  assert( rc!=SQLITE_ROW );

  /* Finalize the virtual machine. This releases all memory and other
   ** resources allocated by the sqlite3_prepare() call above.
   */
  rc = sqlite3_finalize(pStmt);


}

void EvernoteSyncClient::actualSync (std::string authToken, long updateSequenceNumber,
    int syncStateUSN) {
  int domain = 0;
  if (!server.compare ("www.evernote.com"))
    domain = 2;
  else if (!server.compare ("sandbox.evernote.com"))
    domain = 1;
  else
    domain = -1;

  if (getUserStore (domain)) {
    return;
  }
  this->authToken = authToken;
  //  syncNotebooks (updateSequenceNumber);
  //  syncNotes (updateSequenceNumber);

  if (getNoteStore ()) {
    return;
  }

  if (syncStateUSN == -1) {
    // Get latest SyncState and get the most recent USN
    evernote::SyncState* syncState = getSyncState ();
    syncStateUSN = syncState->updateCount;
  }

  evernote::SyncChunkFilter* filter = new evernote::SyncChunkFilter (); 

  filter->includeNotes = true;
  filter->includeNotebooks = true;

  while (updateSequenceNumber < syncStateUSN) {
    evernote::SyncChunk* syncChunk = getFilteredSyncChunk (updateSequenceNumber, 
        5000, filter);

    for (int i = 0; i < (int) syncChunk->notebooks.size (); i++) {
      NotebookData* notebook = new NotebookData (0, syncChunk->notebooks.at(i)->name, 
          syncChunk->notebooks.at (i)->guid->guid, syncChunk->notebooks.at (i)->stack, 
          0, 0, 0);
      app->dbm->exec (notebook->getInsertStatement (), NULL, this);
      delete notebook;
    }

    for (int i = 0; i < (int) syncChunk->notes.size (); i++) {
      evernote::Note* note = syncChunk->notes.at (i);

      NoteStore_getNote_t* NoteStore_getNote_p = (NoteStore_getNote_t*) dlsym (handle, "NoteStore_getNote");
      
      evernote::Note* noteWithContent = NoteStore_getNote_p (noteStore, authToken, 
      syncChunk->notes[i]->guid, true, true, false, false);

      Note_enmlToHtml_t* Note_enmlToHtml_p = (Note_enmlToHtml_t*) dlsym (handle, "Note_enmlToHtml");
      Note_enmlToHtml_p (noteWithContent);

 
      sqlite3_stmt *pStmt;
      const char *zSql = "INSERT INTO notes (title, body, created_time, modified_time, guid, notebook_guid,usn, dirty) VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
      int rc = sqlite3_prepare_v2(app->dbm->db, zSql, -1, &pStmt, 0);

      if (note->updated->timestamp == 0) {
        note->updated->timestamp = note->created->timestamp;
      }
      std::string title = replaceSingleQuote (noteWithContent->title).c_str ();
      sqlite3_bind_text(pStmt, 1, title.c_str (), -1, SQLITE_STATIC);
      std::string contentHtml = replaceSingleQuote (noteWithContent->contentHtml);
      sqlite3_bind_text(pStmt, 2, contentHtml.c_str (), -1, SQLITE_STATIC);
      sqlite3_bind_int64(pStmt, 3, note->created->timestamp);
      sqlite3_bind_int64(pStmt, 4, note->updated->timestamp);
      sqlite3_bind_text(pStmt, 5, noteWithContent->guid->guid.c_str (), -1, SQLITE_STATIC);
      sqlite3_bind_text(pStmt, 6, noteWithContent->notebookGuid.c_str (), -1, SQLITE_STATIC);
      sqlite3_bind_int(pStmt, 7, 0);
      sqlite3_bind_int(pStmt, 8, 0);

      /* Call sqlite3_step() to run the virtual machine. Since the SQL being
       ** executed is not a SELECT statement, we assume no data will be returned.
       */
      rc = sqlite3_step(pStmt);
      assert( rc!=SQLITE_ROW );

      /* Finalize the virtual machine. This releases all memory and other
       ** resources allocated by the sqlite3_prepare() call above.
       */
      rc = sqlite3_finalize(pStmt);

      // Get the resources for this note.
      if (noteWithContent->resources.size () != 0) {
        for (unsigned int j = 0; j < noteWithContent->resources.size (); j++) {
          evernote::Resource* resource = noteWithContent->resources[j];
          saveResource (resource); 
        }
      }
      delete noteWithContent;
      delete note;
    }

    updateUpdateSequenceNumInDatabase (syncChunk->chunkHighUSN);

    updateSequenceNumber += syncChunk->chunkHighUSN;
  }

  app->lpv->refreshLeftPaneView ();
  app->nlpv->fetchNotesForNotebook ("");

  app->mainToolbar->progressBar->hide ();
  app->mainToolbar->progressBarStarted = false;
  app->mainToolbar->syncButton->set_label ("Sync");
  app->mainToolbar->set_subtitle ("Connected to Evernote");
  app->mainToolbar->evernoteConnectionInProgress = false;
}

void EvernoteSyncClient::actualSync (std::string authToken) {
  actualSync (authToken, 0, -1);
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

  EvernoteSyncClient* esc = (EvernoteSyncClient*) userData;
  OAuthManager_generateAccessToken_t* OAuthManager_generateAccessToken_p = 
    (OAuthManager_generateAccessToken_t*) dlsym (esc->handle, "OAuthManager_generateAccessToken");

  std::string authToken = OAuthManager_generateAccessToken_p (esc->oAuthManager, tokenData);

  esc->app->dbm->exec ("insert into system_parameters values ('evernoteAuthToken', '" + authToken +"')",
      NULL, NULL);

  esc->app->dbm->exec ("insert into system_parameters values ('evernoteServer', '" + esc->server +"')",
      NULL, NULL);

  esc->actualSync (authToken, 0, -1);
  esc->sync (-1);
}



void EvernoteSyncClient::secondStageComplete (WebKitWebView *webView,
    WebKitWebFrame* webFrame, gpointer userData) {

  std::string uri = webkit_web_view_get_uri (webView);

  if ( (int) uri.find ("oauth_verifier") == -1) {
    return;
  } else {
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


  OAuthManager_generateFinalAccessTokenUrl_t* OAuthManager_generateFinalAccessTokenUrl_p =
    (OAuthManager_generateFinalAccessTokenUrl_t*) dlsym (esc->handle, 
        "OAuthManager_generateFinalAccessTokenUrl");
  std::string fat = OAuthManager_generateFinalAccessTokenUrl_p (esc->oAuthManager, pin);

  esc->ewvb->hide ();
  esc->app->remove ();
  esc->app->add (*esc->app->windowBody);
  esc->app->show_all ();
  esc->app->mainToolbar->evernoteConnectionInProgress = false;
  esc->app->mainToolbar->newNoteButton->show ();
  esc->app->mainToolbar->newNotebookButton->show ();
  esc->app->mainToolbar->searchEntry->show ();
  esc->app->mainToolbar->syncButton->set_label ("Sync");
  esc->app->mainToolbar->connectedToEvernote = true;
  webkit_web_view_load_uri (webView, fat.c_str ());
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

int EvernoteSyncClient::checkUpdateSequenceNumberCallback (void* p, int argc, char** argv, char** azColName) {
  EvernoteSyncClient* esc = (EvernoteSyncClient*) p;

  if (argc == 1) {
    esc->updateSequenceNumber = atol (argv[0]);
  }

  esc->updateSequenceNumberQueryDone = true;
  return 0;
}

int EvernoteSyncClient::checkEvernoteServerCallback
    (void* p, int argc, char** argv, char** azColName) {
  EvernoteSyncClient* esc = (EvernoteSyncClient*) p;

  if (argc == 1) {
    esc->server = argv[0];
  }

  return 0;
}

int EvernoteSyncClient::checkAuthTokenCallback (void* p, int argc, char** argv, char** azColName) {
  EvernoteSyncClient* esc = (EvernoteSyncClient*) p;

  if (argc == 1) {
    esc->authToken = argv[0];
  }

  esc->authTokenQueryDone = true;
  return 0;
}

evernote::SyncState* EvernoteSyncClient::getSyncState () {
  NoteStore_getSyncState_t* NoteStore_getSyncState_p = (NoteStore_getSyncState_t*) 
    dlsym (handle, "NoteStore_getSyncState");

  evernote::SyncState* syncState = NoteStore_getSyncState_p (noteStore, authToken);
  return syncState;
}

evernote::SyncChunk* EvernoteSyncClient::getFilteredSyncChunk (
    int afterUSN, int maxEntries, evernote::SyncChunkFilter* filter) {
  NoteStore_getFilteredSyncChunk_t* NoteStore_getFilteredSyncChunk_p = (NoteStore_getFilteredSyncChunk_t*)
    dlsym (handle, "NoteStore_getFilteredSyncChunk");

  evernote::SyncChunk* syncChunk = NoteStore_getFilteredSyncChunk_p (noteStore, authToken,
      afterUSN, maxEntries, filter);
  return syncChunk;
}

int EvernoteSyncClient::sync (int domain) {
  /* Check if library is loaded. Else ask the user to install it. */
  handle = dlopen ("libevernote.so", RTLD_LAZY);
  if (!handle) {
    return 1;
  }

  userStore = NULL;
  noteStore = NULL;

  /* Check if we already have the access Token. */
  authTokenQueryDone = false;
  authToken = "";
  app->dbm->exec ("select value from system_parameters where parameter='evernoteAuthToken';",
      &checkAuthTokenCallback, (void*) this);

  server = "";
  /* Check whether we are currently connected to prod or sandbox. */
  app->dbm->exec ("select value from system_parameters where parameter='evernoteServer';", &checkEvernoteServerCallback, (void*) this);

  if (authToken.length () != 0) {
    updateSequenceNumber = -1;
    app->dbm->exec ("select value from system_parameters where parameter='evernoteUpdateSequenceNumber';",
        &checkUpdateSequenceNumberCallback, (void*) this);
    
    if (server.length () != 0) {
      if (!server.compare ("www.evernote.com"))
        domain = 2;
      else if (!server.compare ("sandbox.evernote.com"))
        domain = 1;
      else
        domain = -1;
    }

    if (updateSequenceNumber != -1) {
      getUserStore (domain);
      getNoteStore (); 

      evernote::SyncState* syncState = getSyncState ();

      if (syncState->updateCount > updateSequenceNumber) {
        actualSync (authToken, updateSequenceNumber, syncState->updateCount);
      } else {
        app->mainToolbar->progressBar->hide ();
        app->mainToolbar->progressBarStarted = false;
        app->mainToolbar->set_subtitle ("Connected to Evernote");
        app->mainToolbar->evernoteConnectionInProgress = false;
        app->mainToolbar->newNoteButton->show ();
        app->mainToolbar->newNotebookButton->show ();
        app->mainToolbar->searchEntry->show ();
        app->mainToolbar->syncButton->set_label ("Sync");
        app->mainToolbar->connectedToEvernote = true;
      }

    } else {
      actualSync (authToken);
    }
    return 0;
  } else {
    if (domain == 2)
      server = "www.evernote.com";
    else
      server = "sandbox.evernote.com";


    createOAuthManager_t* createOAuthManager_p = (createOAuthManager_t*) dlsym (handle, 
        "createOAuthManager");

    if (domain == 2) 
      oAuthManager = createOAuthManager_p (consumerKey,
          consumerSecret, requestTokenUrl, requestTokenQueryArgs, authorizeUrl,
          accessTokenUrl);
    else if (domain == 1){ 
      oAuthManager = createOAuthManager_p (consumerKeySandbox,
          consumerSecretSandbox, requestTokenUrlSandbox, requestTokenQueryArgsSandbox,
          authorizeUrlSandbox, accessTokenUrlSandbox);
    }

    OAuthManager_generateRequestTokenUrl_t* OAuthManager_generateRequestTokenUrl_p =
      (OAuthManager_generateRequestTokenUrl_t*) dlsym (handle, "OAuthManager_generateRequestTokenUrl");

    std::string rqu = OAuthManager_generateRequestTokenUrl_p (oAuthManager);


    // Hide the normal ui and get permissions from evernote.
    app->remove ();
    ewvb = Gtk::manage (new EvernoteWebViewBox (false, 0, Gtk::PACK_SHRINK, 0, app));
    ewvb->hide ();
    app->add (*ewvb);		

    webkit_web_view_load_uri (ewvb->webview, rqu.c_str ());
    signalHandlerId = g_signal_connect (ewvb->webview, "document-load-finished", 
        G_CALLBACK (&EvernoteSyncClient::firstStageComplete), this);

  }

  return 0;
}
