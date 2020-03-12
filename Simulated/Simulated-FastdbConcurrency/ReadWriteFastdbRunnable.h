#ifndef READWRITEFASTDBRUNNABLE_H
#define READWRITEFASTDBRUNNABLE_H

#include "FastdbCollection.h"
#include "FastdbManager.h"
#include "FastdbHelper.h"
#include "zthread/Thread.h"

class ReadWriteFastdbRunnable : public Runnable {
public:
	ReadWriteFastdbRunnable(int ident, CountedPtr<FastdbCollection>& collection, 
		FastdbManager* dbManager)
		: id(ident), m_fastdbCollection(collection), m_dbManager(dbManager),
		m_fastdbHelper(id, collection, dbManager) {}

	void run();

private:

private:
	int id;
	CountedPtr<FastdbCollection> m_fastdbCollection;
	FastdbManager* m_dbManager;
	FastdbHelper m_fastdbHelper;
};
#endif