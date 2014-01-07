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
#include <cstdlib>
#include <vector>
#include "evernotedataprovider.hh"

class Note {
    std::string name;
    std::string guid;
    std::string body;
    Note (std::string n, std::string g, std::string b) {
        name = n; guid = g; body = b;
    }
};

class Notebook {
public:
    std::string name;
    std::string guid;
    bool isDefaultNotebook;
    long int serviceCreated;
    long int serviceUpdated;
    Notebook (std::string n,std::string g,bool d ,long int c, long int u) {
        name = n; guid = g; isDefaultNotebook = d; serviceCreated = c; serviceUpdated = u;
    }
    void print () {
        std::cout << name << ":" << guid << ":" << isDefaultNotebook << ":" << serviceCreated << ":" << serviceUpdated << std::endl;
    }
    std::vector <Note> notes;
};

std::vector<Notebook> notebooks;

int main () {

    EvernoteDataProvider edp;
    edp.login ();

    int notebookCount = edp.getNotebookCountPy ();
    std::cout << "NotebookCount: " << notebookCount << std::endl;
    edp.getNotebookDetails ();
    for (int i = 0; i < notebookCount; i++)
        notebooks[i].print ();

    return 0;
}

EvernoteDataProvider::EvernoteDataProvider () {
    hasOAuthToken = false;
    authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";

    /* Add Evernote python libs to PYTHONPATH */
    /* Check if module is working. Instantiate it. */
    putenv ("PYTHONPATH=./:evernote-sdk-python-master/:evernote-sdk-python-master/lib/");
    char* pPath;
    pPath = getenv ("PYTHONPATH");
    if (pPath!=NULL)
        std::cout << "The current path is: " << pPath << std::endl;

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

bool EvernoteDataProvider::getNotesForNotebook (std::string guid) {
    int argc = 0;
    int noteCount;
    pFunc = PyObject_GetAttrString(pModule, "getNoteCountForNotebook");
    /* pFunc is a new reference */

    if (pFunc && PyCallable_Check(pFunc)) {
        pArgs = PyTuple_New(1);
        pValue = PyString_FromString(guid.c_str ());
        PyTuple_SetItem(pArgs, 0, pValue);
        
        pValue = PyObject_CallObject(pFunc, pArgs);
        
        Py_DECREF(pArgs);
        if (pValue != NULL) {
            noteCount = PyInt_FromLong (pValue);
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

    
    return retVal;
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
                obj = PyObject_GetAttrString (pValue, "name");
                name += PyString_AsString (obj);
                obj = PyObject_GetAttrString (pValue, "defaultNotebook");
                if (obj == Py_True)
                    isDefault = true;
                else
                    isDefault = false;
                obj = PyObject_GetAttrString (pValue, "serviceCreated");
                serviceCreated = PyLong_AsLong (obj);
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
        /* Get Notes for this note. */
        std::cout << n.guid << std::endl;
        getNotesForNotebook (n.guid);
    }
    return 0;
}

int EvernoteDataProvider::logout () {

}