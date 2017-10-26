#ifndef EXECTHREAD_H
#define EXECTHREAD_H

#include "sql/SqlExec.h"

#include <QtCore/QtCore>

class CExecThread : public QThread
{
	Q_OBJECT

public:
	CExecThread(QObject* parent = 0);

public:
	void setStop(bool stop);

protected:
	virtual void run();

private:
	bool selectRecord();

	QString	getOperInfo(const QString& text);

signals:
	void outputOperInfo(QString);

private:
	bool	m_stop;
	QMutex	m_mutex;

	QSharedPointer<CSqlExec>	m_sqlExecPtr;
};
#endif