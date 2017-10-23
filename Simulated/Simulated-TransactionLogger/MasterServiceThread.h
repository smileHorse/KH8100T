#ifndef MASTERSERVICETHREAD_H
#define MASTERSERVICETHREAD_H

#include "fastdb.h"

#include <QtCore/QtCore>

class MasterServiceThread : public QThread
{
	Q_OBJECT

public:
	MasterServiceThread(dbDatabase* dbPtr, QObject* parent = 0);
	~MasterServiceThread();

public:
	void setStop(bool stop = true);
	
protected:
	virtual void run();

private:
	void close();

	bool isNeedDeal();

signals:
	void outputOperationInfo(QString text);

private:
	bool m_isStop;
	QDateTime	m_lastOperTime;
	
	dbDatabase* m_dbPtr;
};

#endif