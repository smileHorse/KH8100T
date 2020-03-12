#ifndef FASTDBMANAGER_H
#define FASTDBMANAGER_H

#include "fastdb.h"

#include <string>

using namespace std;

class FastdbManager;
class FastdbManagerInstance
{
public:
	static FastdbManager*	getFastdbManagerInstance();
	static void releaseFastdbManagerInstance();

private:
	static FastdbManager*	m_fastdbManager;
};

class FastdbManager
{
	friend class FastdbManagerInstance;

public:
	FastdbManager();
	~FastdbManager();
	
public:
	bool openDatabase(const string& dbName);
	bool reopenDatabase(const string& dbName);
	bool closeDatabase();
	void setTransactionLogger(const string& path, int flags = dbFile::no_sync, bool crc = true);
	void setTransactionLogger(dbTransactionLogger* logger);
	bool openTransactionLogger(const string& path, int flags = dbFile::no_sync, bool crc = true);
	bool closeTransactionLogger();

	bool restoreDatabase(const string& logger);

	void commit();

	dbDatabase* getDbDatabase();
	dbFileTransactionLogger*	getDbFileTransactionLogger();

private:
	bool hasSetTransactionLogger();

	string getRestoreStatus( dbFileTransactionLogger::RestoreStatus status );

private:
	dbDatabase	m_db;
	dbFileTransactionLogger	m_tl;

	bool	m_bSetTransactionLogger;
	string	m_dbName;
};
#endif