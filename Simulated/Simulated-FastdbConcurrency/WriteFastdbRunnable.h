#ifndef WRITEFASTDBFUNNABLE_H
#define WRITEFASTDBFUNNABLE_H

#include "FastdbCollection.h"
#include "FastdbManager.h"
#include "FastdbHelper.h"
#include "zthread/Thread.h"

using namespace ZThread;

class WriteFastdbRunnable : public Runnable {
public:
	WriteFastdbRunnable(int ident, CountedPtr<FastdbCollection>& collection, 
		FastdbManager* dbManager)
		: id(ident), m_dbManager(dbManager), m_fastdbCollection(collection), 
		m_fastdbHelper(id, collection, dbManager) {}

	void run();
	
private:
	int id;
	CountedPtr<FastdbCollection> m_fastdbCollection;
	FastdbManager* m_dbManager;
	FastdbHelper m_fastdbHelper;
};

#endif