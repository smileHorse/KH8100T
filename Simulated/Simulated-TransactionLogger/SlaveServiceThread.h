#ifndef SLAVESERVICETHREAD_H
#define SLAVESERVICETHREAD_H

#include "fastdb.h"

#include <QtCore/QtCore>

class SlaveServiceThread : public QThread
{
	Q_OBJECT

public:
	SlaveServiceThread(QObject* parent = 0);
	~SlaveServiceThread();

public:
	void setStop(bool stop = true);
	
protected:
	virtual void run();

private:
	void close();

	void restoreDb();

	bool isNeedDeal();

	QString getRestoreStatus(dbFileTransactionLogger::RestoreStatus status);

signals:
	void outputOperationInfo(QString text);

private:
	bool m_isStop;
	QDateTime	m_lastOperTime;

	dbDatabase m_db;
	dbFileTransactionLogger tl;
};

#endif