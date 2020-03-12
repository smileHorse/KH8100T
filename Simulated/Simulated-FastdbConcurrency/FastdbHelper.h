#ifndef FASTDBHELPER_H
#define FASTDBHELPER_H

#include "FastdbCollection.h"
#include "FastdbManager.h"
#include "zthread/CountedPtr.h"

class FastdbHelper {
public:
	FastdbHelper(int ident, CountedPtr<FastdbCollection>& collection, FastdbManager* dbManager) 
		: id(ident), m_fastdbCollection(collection), m_dbManager(dbManager) {}

	FastdbHelper(int ident, FastdbManager* dbManager) 
		: id(ident), m_fastdbCollection(new FastdbCollection), m_dbManager(dbManager) {}

	void insertFastdbData();
	void updateFastdbData();
	void selectFastdbData();

private:
	void insertSubstation();
	void insertLine();
	void insertBreaker();
	void insertRemoteUnit();
	void insertAnalog();
	void insertDiscrete();
	
	void updateSubstation();
	void updateLine();
	void updateBreaker();
	void updateRemoteUnit();
	void updateAnalog();
	void updateDiscrete();

	void selectSubstation();
	void selectLine();
	void selectBreaker();
	void selectRemoteUnit();
	void selectAnalog();
	void selectDiscrete();

private:
	int id;
	CountedPtr<FastdbCollection> m_fastdbCollection;
	FastdbManager* m_dbManager;
};
#endif