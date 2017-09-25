#include "AmsServerThread.h"
#include "AmsServer.h"
#include "OperationInfo.h"


void AmsServerThread::run()
{
	AmsServer server(this);
	server.main(m_argc, m_argv);
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
