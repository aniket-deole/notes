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
#include "webviewpopup.hh"

static void destroyWindowCb (WebKitWebView  *webview,
    WebKitWebFrame *web_frame, gpointer user_data) {
  std::string body = "";
  webkit_web_view_execute_script (webview, "document.title=document.documentElement.innerHTML;");
  body.append (webkit_web_frame_get_title ((webkit_web_view_get_main_frame (webview))));
  std::cout << webkit_web_view_get_load_status (webview) << std::endl;
//  std::cout << body << std::endl;

}

WebViewPopup::WebViewPopup () : Gtk::Dialog ("", true){
  webview = WEBKIT_WEB_VIEW (webkit_web_view_new ());

  Gtk::Box* contentBox = get_content_area ();

  gtk_container_add (GTK_CONTAINER (contentBox->gobj ()), GTK_WIDGET (webview));

  webkit_web_view_set_editable (webview, false);
//  webkit_web_view_load_uri(webview, "https://sandbox.evernote.com/oauth?oauth_callback=sandbox.evernote.com&oauth_consumer_key=analogx&oauth_nonce=142836553319a254d7&oauth_signature=953b9c5d5f64c09b&&oauth_signature_method=PLAINTEXT&oauth_timestamp=1428365533&oauth_version=1.0");


  webkit_web_view_load_uri (webview, "http://www.google.com");

  contentBox->set_size_request (1024, 400);
  contentBox->show_all_children ();

  g_signal_connect(webview, "document-load-finished", G_CALLBACK(destroyWindowCb), NULL);

}

WebViewPopup::~WebViewPopup () {

}
