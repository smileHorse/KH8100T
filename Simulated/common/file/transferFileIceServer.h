#ifndef TRANSFERFILESERVER_H
#define TRANSFERFILESERVER_H

#include <Ice/Application.h>
#include <string>

using namespace std;

class TransferFileIceServer : public Ice::Application
{
public:
	TransferFileIceServer();
	virtual ~TransferFileIceServer();

	void startServer(int argc, char* argv[]);

protected:
	virtual int run(int argc, char* argv[]);

private:
	string	adapterIdentify;
	string	iceIp;
	int		icePort;
	string	iceStormIp;
	int		iceStormPort;
};

#endif