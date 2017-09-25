#ifndef FEPSERVER_H
#define FEPSERVER_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>
#include <QObject>

class FepServerThread;

#pragma execution_character_set("utf-8")

class FepServer : public QObject, public Ice::Application
{
	Q_OBJECT

public:
	FepServer(int argc, char* argv[]);

	virtual int run(int argc, char* argv[]);

	void setThreadPtr(FepServerThread* ptr);
	
private slots:
	void startServer();
	
signals:
	void executeOperation(const QString& text);

private:
	int	m_argc;
	char**	m_argv;

	FepServerThread*	m_threadPtr;

	Ice::CommunicatorPtr m_communicatorPtr;
};

#endif