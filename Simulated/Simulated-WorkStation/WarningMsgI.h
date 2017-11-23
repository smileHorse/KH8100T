#ifndef WARNINGMSGI_H
#define WARNINGMSGI_H

#include "warningmsg.h"

using namespace WarningMsg;

class WorkStationServerThread;

class SendWarningMsgI : public SendWarningMsg
{
public:
	SendWarningMsgI(WorkStationServerThread* threadPtr);

	virtual void sendWarningMsgBinary(const ::std::string& title, const ::WarningMsg::Bytes& bytes, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual void sendFinished(const ::std::string& title, const ::Ice::Current& /* = ::Ice::Current() */);
	
private:
	WorkStationServerThread*	m_threadPtr;
};
#endif