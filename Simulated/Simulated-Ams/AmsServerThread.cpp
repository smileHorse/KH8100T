#include "AmsServerThread.h"
#include "OperationInfo.h"

AmsServerThread::AmsServerThread( QObject* parent /*= 0*/ )
	: QThread(parent), server(this)
{

}

void AmsServerThread::run()
{
	server.main(m_argc, m_argv, "config.server");
}


void AmsServerThread::setRole(QString role)
{
	server.setRole(role.toStdString());
}

void AmsServerThread::setParam( int argc, char* argv[] )
{
	m_argc = argc;
	m_argv = argv;
}

void AmsServerThread::putMessage( const OperationInfo& info )
{
	emit executeOperation(info);
}
