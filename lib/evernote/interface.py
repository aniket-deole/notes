import hashlib
import binascii
import evernote.edam.userstore.constants as UserStoreConstants
import evernote.edam.type.ttypes as Types

from evernote.api.client import EvernoteClient

note = 0
notebooks = 0

def login (token):
    client = EvernoteClient(token=auth_token, sandbox=True)
    user_store = client.get_user_store()
    version_ok = user_store.checkVersion(
        "Evernote EDAMTest (Python)",
        UserStoreConstants.EDAM_VERSION_MAJOR,
        UserStoreConstants.EDAM_VERSION_MINOR
    )

    if not version_ok:
        return -1
    else
        return 0
