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
#include <sstream>
#include <cstdlib>
#include <vector>
#include <protocol/TBinaryProtocol.h>
#include <transport/THttpClient.h>
#include <transport/TSSLSocket.h>

#include "UserStore.h"
#include "NoteStore.h"

#include "evernotedataprovider.hh"

/*
Note(contentHash='\xbbS\x8ad\x05\xd0%\x98\xf9\xa6L[\xc2\xce\x8bY', 
    updated=1388741518000, created=1388741518000, deleted=None, contentLength=221, 
    title='Test note from EDAMTest.py', notebookGuid='dafa6b30-402c-4920-b251-2827881d2ac3', 
    content='<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE en-note SYSTEM "http://xml.evernote.com/pub/enml2.dtd"><en-note>Here is the Evernote logo:<br/><en-media type="image/png" hash="a54fe8bcd146e20a8a5742834558543c"/></en-note>', 
    tagNames=None, updateSequenceNum=53, tagGuids=None, active=True, 
    attributes=NoteAttributes(lastEditorId=None, placeName=None, sourceURL=None, 
        classifications=None, creatorId=None, author=None, reminderTime=None, altitude=None, 
        reminderOrder=None, shareDate=None, reminderDoneTime=None, longitude=None, lastEditedBy=None, 
        source=None, applicationData=None, sourceApplication=None, latitude=None, contentClass=None, subjectDate=None), 
    guid='c3aab5ac-947f-4d94-8e8c-a54f11674ec0', 
    resources=[Resource(noteGuid='c3aab5ac-947f-4d94-8e8c-a54f11674ec0', height=60, 
        width=60, alternateData=None, mime='image/png', updateSequenceNum=54, 
        duration=None, attributes=ResourceAttributes(recoType=None, sourceURL=None, cameraMake=None, 
        timestamp=None, altitude=None, clientWillIndex=None, longitude=None, fileName=None, 
        attachment=None, latitude=None, applicationData=None, cameraModel=None), 
        guid='05ac4079-b3e5-4648-bc45-26fbe834d88b', data=Data(body=None, bodyHash='\xa5O\xe8\xbc\xd1F\xe2\n\x8aWB\x83EXT<', size=6035), 
        active=True, recognition=Data(body=None, bodyHash='\x91G\x01\xa5\x11\\Xw\xca\xc1\x00"\x9d\xf3\x16Y', size=291))]
)
*/


std::vector<evernote::Notebook> gNotebooks;
std::vector<evernote::Note> gNotes;

evernote::EvernoteDataProvider::EvernoteDataProvider (Notify* n) {
    app = n;
    
    hasOAuthToken = false;
    authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";

    std::string site = "sandbox.evernote.com";
    boost::shared_ptr<apache::thrift::transport::THttpClient> auth_http( new apache::thrift::transport::THttpClient("sandbox.evernote.com", 80, "/edam/user") );
    auth_http->open();
    boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> userStoreProt( new apache::thrift::protocol::TBinaryProtocol(auth_http) );
    evernote::edam::UserStoreClient userStore(userStoreProt, userStoreProt );

    std::string  noteStoreUrl = "";
    userStore.getNoteStoreUrl (noteStoreUrl, authToken);
    auth_http->close();   
    std::cout << noteStoreUrl << std::endl;

    noteStoreUrl = "/shard/s1/notestore";


    lastUpdateCount = 0;
}

evernote::EvernoteDataProvider::~EvernoteDataProvider () {
}

int evernote::EvernoteDataProvider::open () {
    /* Connect using auth */
    return 0;
}

int evernote::EvernoteDataProvider::close () {
    return 0;
}

int evernote::EvernoteDataProvider::login () {
    return 0;
}

int evernote::EvernoteDataProvider::getNotebookCountPy () {
    return 0;
}

bool evernote::EvernoteDataProvider::getNotesForNotebook (std::string g) {
    return 0;
}

int evernote::EvernoteDataProvider::getNotebookDetails () {
    return 0;
}

int evernote::EvernoteDataProvider::logout () {
    return 0;
}

int evernote::EvernoteDataProvider::sync () {
/*
    1. get Last Update Count from db. If it does not exist. Initialize it to 0.
    2. Get AuthToken. For now, we have the sandbox evernote token.
    // INCOMING
    3. Check whether to full sync. i.e. First run or not.
    // FULL SYNC
    4. getSyncChunk for a 100 notes.
    5. For All Notebooks
        ProcessNotebook ()
    6. For All Notes
        ProcessNote ()
    7. lastUpdateCount = UpdateCount From Server
    // INCREMENTAL SYNC
    8. getSyncChunk but afterUSN = lastUpdateCount.
 2   9. For All Notebooks
        ProcessNotebook ()
    10. For All Notes 
        ProcessNotes ()
    11. Process Expunged Items.
    // SEND CHANGES
    12. For resources with dirty tags.
        If USN == 0
        createResource
        else 
        updateResource
    13. END
*/ 
    std::string noteStoreUrl = "/shard/s1/notestore";

    boost::shared_ptr<apache::thrift::transport::TSSLSocketFactory> sslSocketFactory = boost::shared_ptr<apache::thrift::transport::TSSLSocketFactory>(new apache::thrift::transport::TSSLSocketFactory());;

    boost::shared_ptr<apache::thrift::transport::TSocket> sslSocket = sslSocketFactory->createSocket("sandbox.evernote.com", 443);
    boost::shared_ptr<apache::thrift::transport::TBufferedTransport> bufferedTransport(new apache::thrift::transport::TBufferedTransport(sslSocket));
    boost::shared_ptr<apache::thrift::transport::THttpClient> userStoreHttpClient = boost::shared_ptr<apache::thrift::transport::THttpClient>(new apache::thrift::transport::THttpClient(bufferedTransport, "sandbox.evernote.com", noteStoreUrl));

    userStoreHttpClient->open();

    boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> noteStoreProt(new apache::thrift::protocol::TBinaryProtocol(userStoreHttpClient) );
    evernote::edam::NoteStoreClient noteStore(noteStoreProt, noteStoreProt );

    std::vector<evernote::edam::Notebook> notebooks;
    std::vector<evernote::edam::Note> notes;

    noteStore.listNotebooks(notebooks, authToken);

    evernote::edam::NoteList notesMetadataList;


    for (unsigned int i = 0; i < notebooks.size (); i++) {
      std::cout << notebooks[i].guid << ":" << notebooks[i].stack << std::endl;
      evernote::Notebook n(notebooks[i].name, notebooks[i].guid, notebooks[i].defaultNotebook, notebooks[i].serviceCreated, notebooks[i].serviceUpdated);
      gNotebooks.push_back (n);
    }
    std::cout << "==========================" << std::endl;
    evernote::edam::NoteFilter noteFilter;

    noteStore.findNotes (notesMetadataList, authToken, noteFilter, 0, 20);

    for (unsigned int i = 0; i < notesMetadataList.notes.size (); i++) {
        evernote::edam::Note note = notesMetadataList.notes[i];
        std::cout << note.guid << std::endl;
        std::cout << "==========================" << std::endl;
        std::string content;
        noteStore.getNoteContent (content, authToken, note.guid);
        evernote::Note n(note.title, note.guid, content, note.notebookGuid, note.created, note.updated);
        gNotes.push_back (n);
        std::cout << content << std::endl;
        std::cout << "==========================" << std::endl;
    }

    userStoreHttpClient->flush ();
    userStoreHttpClient->close();
    userStoreHttpClient->close();

    std::cout << notebooks.size () << notesMetadataList.notes.size () << std::endl;

    return 0;
}