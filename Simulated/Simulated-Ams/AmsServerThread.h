#ifndef AMSSERVERTHREAD_H
#define AMSSERVERTHREAD_H

#include <QThread>

class OperationInfo;

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