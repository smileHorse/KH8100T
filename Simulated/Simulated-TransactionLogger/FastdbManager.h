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
	bool openDatabase();
	bool reopenDatabase();
	bool closeDatabase();
	bool hasSetTransactionLogger();
	void setTransactionLogger(const string& path, int flags = dbFile::no_sync, bool crc = true);
	void setTransactionLogger(dbTransactionLogger* logger);
	bool openTransactionLogger(const string& path, int flags = dbFile::no_sync, bool crc = true);
	bool closeTransactionLogger();

	void commit();

	dbDatabase* getDbDatabase();
	dbFileTransactionLogger*	getDbFileTransactionLogger();
	
signals:
	void outputOperationInfo(QString text);

private:
	dbDatabase	m_db;
	dbFileTransactionLogger	m_tl;

	bool	m_bSetTransactionLogger;

};
#endif