#ifndef RDBREALDATAI_H
#define RDBREALDATAI_H

#include "rdbdata.h"

class WorkStationServerThread;

using namespace RdbRealData;

class RdbRealDataRequestI : public RdbRealDataRequest
{
public:
	RdbRealDataRequestI(WorkStationServerThread* threadPtr);
	~RdbRealDataRequestI();

public:
	virtual void RequestDefaultData(const ::RdbRealData::RequestDefaultDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void RequestSpecificData(const ::RdbRealData::RequestSpecficDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void RequestCompleteData(const ::RdbRealData::RequestCompleteDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void SendTopoDataRequest(const ::RdbRealData::RequestTopoDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */);

private:
	WorkStationServerThread*	m_threadPtr;
};

class RdbRealDataRespondI : public RdbRealDataRespond
{
public:
	RdbRealDataRespondI(WorkStationServerThread* threadPtr);
	~RdbRealDataRespondI();

public:
	virtual void RespondDefaultData(const ::RdbRealData::RespondDefaultDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void RespondSpecificData(const ::RdbRealData::RespondSpecficDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void RespondCompleteData(const ::RdbRealData::RespondCompleteDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void SendTopoDataRespond(const ::RdbRealData::ReceiveTopoDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */);

private:
	WorkStationServerThread*	m_threadPtr;
};

#endif