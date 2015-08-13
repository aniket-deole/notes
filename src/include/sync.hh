#ifndef _SYNC_HH_
#define _SYNC_HH_

#include <vector>
#include <notify.hh>

class SyncClient {
public:
	Notify* app;
	virtual int sync (int) = 0;
};


class SyncManager {
public:
	SyncManager () {
	}
	~SyncManager () {}
	void addSyncClient (SyncClient* sc) {
		clients.push_back (sc);
	}
	void removeSyncClient (SyncClient* sc) {
		
	}
	int sync (int domain) {
		for (unsigned int i = 0; i < clients.size (); i++) {
			clients[i]->sync (domain);
		}
		return 0;
	}
private:
	std::vector <SyncClient*> clients;
};


#endif
