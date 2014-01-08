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
result_list = 0

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
    notebooks = note_store.listNotebooks()
    return len(notebooks)

def getNotebook (i):
    global notebooks
    return notebooks[i]

def getNoteCountForNotebook (notebookGuid):
    global note_store
    global result_list

    updated_filter = NoteFilter(order=NoteSortOrder.UPDATED)
    updated_filter.notebookGuid = notebookGuid
    offset = 0
    max_notes = 10
    result_spec = NotesMetadataResultSpec(includeTitle=True)
    result_list = note_store.findNotesMetadata(auth_token, updated_filter, offset, max_notes, result_spec)

    # note is an instance of NoteMetadata
    # result_list is an instance of NotesMetadataList
    return len (result_list.notes)
    
def getNoteForNotebook (i):
    global result_list
    note = result_list.notes[0]
    idd = note.guid
    wholeNote = note_store.getNote(idd, True, False, False, False)
    print wholeNote
    return result_list.notes[i]
