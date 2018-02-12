#ifndef AMSSERVERTHREAD_H
#define AMSSERVERTHREAD_H

#include "AmsServer.h"

#include <QThread>

class OperationInfo;

class AmsServerThread : public QThread
{
	Q_OBJECT

public:
	AmsServerThread(QObject* parent = 0);

	void setParam(int argc, char* argv[]);
	void putMessage(const OperationInfo& info);

protected:
	virtual void run();

signals:
	void executeOperation(const OperationInfo& info);

public slots:
	void setRole(QString role);
	void changeRole(QString role);
	void configHostPort(QString host, int port);

private:
	int		m_argc;
	char**	m_argv;

	AmsServer	server;
};

#endif