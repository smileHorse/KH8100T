#ifndef ANALYSEFILETHREAD_H
#define ANALYSEFILETHREAD_H

#include <QtCore/QtCore>

class CAnalyseFileThread : public QThread 
{
	Q_OBJECT

public:
	CAnalyseFileThread(const QString& fileName, int timeSpan, QObject* parent = 0);

	void setStop(bool stop) { m_stop = stop; }

protected:
	virtual void run();

Q_SIGNALS:
	void outputInfo(const QString&);

private:
	QString m_fileName;
	int m_timeSpan;
	bool m_stop;
};
#endif