import hashlib  
import binascii
import evernote.edam.userstore.constants as UserStoreConstants
import evernote.edam.type.ttypes as Types

from evernote.edam.notestore.ttypes import NoteFilter, NotesMetadataResultSpec
from evernote.edam.type.ttypes import NoteSortOrder

from evernote.api.client import EvernoteClient

notes = 0
notebooks = 0
client = 0
user_store = 0
note_store = 0
auth_token = 0

def login (t):
    global client
    global user_store
    global note_store
    global auth_token

    auth_token = t
    client = EvernoteClient(token=auth_token, sandbox=True)
    user_store = client.get_user_store()
    version_ok = user_store.checkVersion(
        "Evernote EDAMTest (Python)",
        UserStoreConstants.EDAM_VERSION_MAJOR,
        UserStoreConstants.EDAM_VERSION_MINOR
    )

    if not version_ok:
        return -1
    else:
        return 0

def getNotebookCount():
    global notebooks
    global note_store

    note_store = client.get_note_store()

    # List all of the notebooks in the user's account
    notebooks = note_store.listNotebooks()
    print notebooks[0]
    print notebooks[1]
    print notebooks[2]
    return len(notebooks)

def getNotebookName (i):
    global notebooks
    return notebooks[i].name

def getNotebookGuid (i):
    global notebooks
    return notebooks[i]

def getNotebookServiceUpdated (i):
    return notebooks[i].serviceUpdated

def getNotebookServiceCreated (i):
    return notebooks[i].serviceCreated

def getNotebookIsDefault (i):
    return notebooks[i].defaultNotebook

def getNotesForNotebook (notebookGuida):
    global note_store
    updated_filter = NoteFilter(order=NoteSortOrder.UPDATED)
    updated_filter.notebookGuid = notebookGuida
    offset = 0
    max_notes = 10
    print notebookGuida
    result_spec = NotesMetadataResultSpec(includeTitle=True)
    result_list = note_store.findNotesMetadata(auth_token, updated_filter, offset, max_notes, result_spec)

    # note is an instance of NoteMetadata
    # result_list is an instance of NotesMetadataList
    for note in result_list.notes:
        print note.title
    return 2