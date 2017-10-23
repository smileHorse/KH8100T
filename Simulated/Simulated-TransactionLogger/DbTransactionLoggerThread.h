#ifndef DBTRANSACTIONLOGGERMANAGER_H
#define DBTRANSACTIONLOGGERMANAGER_H

#include <QtCore/QtCore>

class FastdbManager;

class DbTransactionLoggerThread : public QThread
{
	Q_OBJECT

public:
	DbTransactionLoggerThread(QObject* parent = 0);
	~DbTransactionLoggerThread();

public:
	void setStop(bool stop = true);

protected:
	virtual void run();

private:
	bool isNeedChangeTransLogger();

	QString getTransLoggerName();

signals:
	void outputOperationInfo(QString text);

private:
	FastdbManager*	m_fastdbManager;

	bool	m_isStop;
	QDateTime	m_lastChangeLoggerTime;	
};
#endif