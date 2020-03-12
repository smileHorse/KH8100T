#ifndef READFASTDBRUNNABLE_H
#define READFASTDBRUNNABLE_H

#include "FastdbHelper.h"
#include "zthread/Thread.h"
#include "zthread/Runnable.h"
using namespace ZThread;

class FastdbManager;

class ReadFastdbRunnable : public Runnable {
public:
	ReadFastdbRunnable(int ident, FastdbManager* dbManager)
		: id(ident), m_dbManager(dbManager), m_fastdbHelper(id, m_dbManager) {}

	void run();
	
private:
	int id;
	FastdbManager* m_dbManager;
	FastdbHelper m_fastdbHelper;
};
#endif