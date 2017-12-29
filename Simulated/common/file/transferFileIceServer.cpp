
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>

#include "transferFileIceServer.h"

TransferFileIceServer::TransferFileIceServer()
	: iceIp(0), iceStormIp(0)
{

}

TransferFileIceServer::~TransferFileIceServer()
{

}

void TransferFileIceServer::startServer( int argc, char* argv[] )
{
	main(argc, argv);
}

int TransferFileIceServer::run( int argc, char* argv[] )
{
	try
	{
		//string endPoints = QString("tcp -h %1 -p %2").arg(QString::fromStdString(iceIp)).arg(icePort).toStdString();
		//Ice::ObjectAdapterPtr adapterPtr = communicator()->createObjectAdapterWithEndpoints(adapterIdentify, endPoints);


		//adapterPtr->activate();

		communicator()->waitForShutdown();
		
		return 0;
	}
	catch(const Ice::Exception& ex)
	{

		return -1;
	}
}
