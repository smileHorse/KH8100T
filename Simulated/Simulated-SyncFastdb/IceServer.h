
#ifndef ICESERVER_H
#define ICESERVER_H

#include <Ice/Ice.h>

class IceServer : public Ice::Application
{
public:
	IceServer();

	virtual int run(int argc, char* argv[]);
};

#endif