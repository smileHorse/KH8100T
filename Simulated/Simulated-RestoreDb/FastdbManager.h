#ifndef FASTDBMANAGER_H
#define FASTDBMANAGER_H

#include "fastdb.h"

#include <QtCore/QtCore>
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

class FastdbManager : public QObject
{
	Q_OBJECT

	friend class FastdbManagerInstance;

public:
	FastdbManager(QObject* parent = 0);
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

	
signals:
	void outputOperationInfo(QString text);

private:
	dbDatabase	m_db;
	dbFileTransactionLogger	m_tl;

	bool	m_bSetTransactionLogger;
	string	m_dbName;
};
#endif