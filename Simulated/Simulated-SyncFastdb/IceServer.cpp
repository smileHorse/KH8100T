
#include "IceServer.h"

IceServer::IceServer()
{

}

int IceServer::run( int argc, char* argv[] )
{
	try
	{
		Ice::CommunicatorPtr communicatorPtr = communicator();
		Ice::ObjectAdapterPtr objectAdapterPtr = communicatorPtr->createObjectAdapterWithEndpoints("IceServer", "tcp -h localhost -p 10000");

		objectAdapterPtr->activate();
	}
	catch(const Ice::Exception& ex)
	{
		return -1;
	}

	return 1;
}
