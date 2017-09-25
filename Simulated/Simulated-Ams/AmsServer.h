#ifndef AMSSERVER_H
#define AMSSERVER_H

#include <Ice/ice.h>

class AmsServerThread;

#pragma execution_character_set("utf-8")

class AmsServer : public Ice::Application
{
public:
	AmsServer(AmsServerThread* threadPtr);
	virtual int run(int argc, char* argv[]);

private:
	AmsServerThread*	m_threadPtr;
};

#endif