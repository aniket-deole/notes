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

static std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}
class Note {
public:
    std::string title;
    std::string guid;
    std::string content;
    std::string notebook_guid;
    long int created;
    long int updated;
    bool deleted;
    Note (std::string t, std::string g, std::string c, std::string n_g, long int c_time, long int u_time, bool d = false) {
        title = t; guid = g; content = c; notebook_guid = n_g;
        created = c_time / 1000; updated = u_time / 1000; deleted = d;
    }

    void createInsertStatement () {
        std::string query = "INSERT INTO NOTES VALUES (NULL,'";
        query += ReplaceString (title, "'", "''");
        query += "','";
        query += ReplaceString (content, "'", "''");
        query += "',";
        query += NumberToString (created);
        query += ",";
        query += NumberToString (updated);
        query += ",'";
        query += guid;
        query += "','";
        query += notebook_guid;
        query += "');";
        std::cout << query << std::endl;        
    }
};

class Notebook {
public:
    std::string name;
    std::string guid;
    bool isDefaultNotebook;
    long int created;
    long int updated;
    Notebook (std::string n,std::string g,bool d ,long int c, long int u) {
        name = n; guid = g; isDefaultNotebook = d; created = c / 1000; updated = u / 1000;
    }

    void createInsertStatement () {
        std::string query = "INSERT INTO NOTEBOOKS VALUES (NULL,'";
        query += ReplaceString (name, "'", "''");
        query += "','";
        query += guid;
        query += "','";
        /* Parent GUID should go here. */
        query += "',";
        query += NumberToString (created);
        query += ",";
        query += NumberToString (updated);
        query += ");";
        std::cout << query << std::endl;        
    }
};

std::vector<Notebook> notebooks;
std::vector<Note> notes;
/*
int main () {

    EvernoteDataProvider edp;
    edp.login ();

    int notebookCount = edp.getNotebookCountPy ();
    edp.getNotebookDetails ();

    for (int i = 0; i < notebooks.size (); i++) {
        edp.getNotesForNotebook (notebooks[i].guid);
    }
  
    /* Print out insert statements 
    for (int i = 0; i < notes.size (); i++) {
        notes[i].createInsertStatement ();
    }
    for (int i = 0; i < notebooks.size (); i++) {
        notebooks[i].createInsertStatement ();
    }

    std::cout << notes.size () << ":" << notebooks.size () << std::endl;
    return 0;
}
*/
EvernoteDataProvider::EvernoteDataProvider () {
    hasOAuthToken = false;
    authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";

    /* Add Evernote python libs to PYTHONPATH */
    /* Check if module is working. Instantiate it. */
    putenv ("PYTHONPATH=./:evernote-sdk-python-master/:evernote-sdk-python-master/lib/");

    Py_Initialize();
    pName = PyString_FromString("interface");
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
}

EvernoteDataProvider::~EvernoteDataProvider () {
    Py_DECREF(pModule);
    Py_Finalize();
}

int EvernoteDataProvider::open () {
    /* Connect using auth */
    return 0;
}

int EvernoteDataProvider::close () {
    return 0;
}

int EvernoteDataProvider::sync () {
    return 0;
}

int EvernoteDataProvider::login () {
    if (pModule != NULL) {    
        pFunc = PyObject_GetAttrString(pModule, "login");
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(1);
            pValue = PyString_FromString(authToken.c_str ());
            PyTuple_SetItem(pArgs, 0, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", "login");
        }
        Py_XDECREF(pFunc);
    }
}

int EvernoteDataProvider::getNotebookCountPy () {
    int retVal = -1;
    if (pModule != NULL) {    
        pFunc = PyObject_GetAttrString(pModule, "getNotebookCount");
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(0);
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                retVal = PyInt_AsLong (pValue);
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", "login");
        }
        Py_XDECREF(pFunc);
    }
    notebookCount = retVal;
    return retVal;
}

bool EvernoteDataProvider::getNotesForNotebook (std::string g) {
    int argc = 0;
    int noteCount;
    std::string name, guid;
    bool isDefault;
    long int serviceUpdated, serviceCreated;
    pFunc = PyObject_GetAttrString(pModule, "getNoteCountForNotebook");
    /* pFunc is a new reference */

    if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(1);
        pValue = PyString_FromString(g.c_str ());
        PyTuple_SetItem(pArgs, 0, pValue);
        
        pValue = PyObject_CallObject(pFunc, pArgs);
        
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            noteCount = PyInt_AsLong (pValue);
            Py_DECREF(pValue);
        }
        else {
            Py_DECREF(pFunc);
            Py_DECREF(pModule);
            PyErr_Print();
            fprintf(stderr,"Call failed\n");
        }
    }
    else {
        if (PyErr_Occurred())
            PyErr_Print();
        fprintf(stderr, "Cannot find function getNotesForNotebook\n");
    }
    Py_XDECREF(pFunc);

    for (int i = 0; i < noteCount; i++) {
        std::string title = "";
        std::string guid = "";
        std::string content = "";
        std::string notebook_guid = "";
        long int created;
        long int updated;
        
        pFunc = PyObject_GetAttrString(pModule, "getNoteForNotebook");
        /* pFunc is a new reference */
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(1);
            pValue = PyInt_FromLong(i);
            PyTuple_SetItem(pArgs, 0, pValue);
            
            pValue = PyObject_CallObject(pFunc, pArgs);
            
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                PyObject* obj  = PyObject_GetAttrString (pValue, "guid");
                guid += PyString_AsString (obj);
                Py_DECREF (obj);
                obj = PyObject_GetAttrString (pValue, "title");
                title += PyString_AsString (obj);
                Py_DECREF (obj);
                obj = PyObject_GetAttrString (pValue, "content");
                if (obj != NULL) {
                    content += PyString_AsString (obj);
                    Py_DECREF (obj);
                }
                if (obj != NULL) {
                    obj = PyObject_GetAttrString (pValue, "notebookGuid");
                    notebook_guid += PyString_AsString (obj);
                    Py_DECREF (obj);
                }
                if (obj != NULL) {
                    obj = PyObject_GetAttrString (pValue, "created");
                    created = PyInt_AsLong (obj);
                    Py_DECREF (obj);
                }
                if (obj != NULL) {
                    obj = PyObject_GetAttrString (pValue, "updated");
                    updated = PyInt_AsLong (obj);
                    Py_DECREF (obj);
                }
                Note n (title, guid, content, notebook_guid, created, updated);
                notes.push_back (n);          
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function getNotesForNotebook\n");
        }
        Py_XDECREF(pFunc);        
    }
    
    return true;
}

int EvernoteDataProvider::getNotebookDetails () {
    for (int i = 0; i < notebookCount; i++) {
        std::string name, guid;
        bool isDefault;
        long int serviceUpdated, serviceCreated;

        pFunc = PyObject_GetAttrString(pModule, "getNotebook");
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(1);
            pValue = PyInt_FromLong(i);
            PyTuple_SetItem(pArgs, 0, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != NULL) {
                PyObject* obj  = PyObject_GetAttrString (pValue, "guid");
                guid += PyString_AsString (obj);
                Py_DECREF (obj);
                obj = PyObject_GetAttrString (pValue, "name");
                name += PyString_AsString (obj);
                Py_DECREF (obj);
                obj = PyObject_GetAttrString (pValue, "defaultNotebook");
                if (obj == Py_True)
                    isDefault = true;
                else
                    isDefault = false;
                Py_DECREF (obj);
                obj = PyObject_GetAttrString (pValue, "serviceCreated");
                serviceCreated = PyLong_AsLong (obj);
                Py_DECREF (obj);
                obj = PyObject_GetAttrString (pValue, "serviceUpdated");
                serviceUpdated = PyLong_AsLong (obj);
                Py_DECREF (obj);
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function getNotesForNotebook\n");
        }
        Py_XDECREF(pFunc);

        Notebook n(name, guid, isDefault, serviceCreated, serviceUpdated);
        notebooks.push_back (n);
    }
    return 0;
}

int EvernoteDataProvider::logout () {

}