#ifndef _EVERNOTE_SYNC_HH_
#define _EVERNOTE_SYNC_HH_

#include <libevernote/evernote.hh>
#include "sync.hh"
#include "notify.hh"

class EvernoteSyncClient : public SyncClient {

public:
	EvernoteSyncClient (Notify* app) {
		this->app = app;
	}
	int sync ();
	static void firstStageComplete(WebKitWebView  *web_view,
               WebKitWebFrame *web_frame,
               gpointer        user_data); 

	static void secondStageComplete(WebKitWebView  *web_view,
               WebKitWebFrame *web_frame,
               gpointer        user_data); 
	static void thirdStageComplete(WebKitWebView  *web_view,
               WebKitWebFrame *web_frame,
               gpointer        user_data); 
	static void fourthStageComplete(WebKitWebView  *web_view,
               WebKitWebFrame *web_frame,
               gpointer        user_data); 




	evernote::OAuthManager* oAuthManager;

	void* handle; // handle to the dll file.
	EvernoteWebViewBox* ewvb;
	int signalHandlerId;	
};

#endif
