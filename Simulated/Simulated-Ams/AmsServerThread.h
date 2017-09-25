#ifndef AMSSERVERTHREAD_H
#define AMSSERVERTHREAD_H

#include <QThread>

class OperationInfo;

#pragma execution_character_set("utf-8")

class AmsServerThread : public QThread
{
	Q_OBJECT

public:
	void setParam(int argc, char* argv[]);
	void putMessage(const OperationInfo& info);

protected:
	virtual void run();

signals:
	void executeOperation(const OperationInfo& info);

private:
	int		m_argc;
	char**	m_argv;
};

#endif