#ifndef _EVERNOTE_SYNC_HH_
#define _EVERNOTE_SYNC_HH_

#include "sync.hh"

class EvernoteSyncClient : public SyncClient {
public:
	int sync ();
};

#endif