#ifndef AMSSERVER_H
#define AMSSERVER_H

#include "aml.h"
#include "AmsSrvI.h"

#include <Ice/ice.h>
#include <string>

using namespace std;

class AmsServerThread;

class AmsServer : public Ice::Application
{
public:
	AmsServer(AmsServerThread* threadPtr);
	virtual int run(int argc, char* argv[]);

public:
	void setRole(const string& role);
	void changeRole(const string& role);
	void setHostPort(const string& host, int port);

private:
	AmsServerThread*	m_threadPtr;
	Amssrv::CAmsAppPtr	amsApp;

	string	m_dataSrvHost;
	int		m_dataSrvPort;
};

#endif