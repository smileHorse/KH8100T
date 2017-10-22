#ifndef RANDOMOPERATEDB_H
#define RANDOMOPERATEDB_H

#include "fastdb.h"

#include <QtCore/QtCore>

enum OperateMode
{
	InsertMode,
	UpdateMode,
	DeleteMode,
	SelectMode,
	ModeCount
};

class RandomOperateDb
{
public:
	RandomOperateDb(dbDatabase* dbPtr = NULL);
	~RandomOperateDb();

public:
	void startOperate(OperateMode mode = ModeCount);
	void insertData();
	void updateData();
	void deleteData();
	void selectData();

	QString getOperateInfo();

private:
	int getRandValue();

private:
	dbDatabase*	m_dbPtr;
	QString m_operInfo;	// ²Ù×÷ÐÅÏ¢

	QMutex	m_mutex;
};

#endif