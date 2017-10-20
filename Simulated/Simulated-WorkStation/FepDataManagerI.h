#ifndef FEPDATAMANAGERI_H
#define FEPDATAMANAGERI_H

#include "fepdata.h"

class WorkStationServerThread;

using namespace FepData;

class FepDataManagerI : public FepDataManager
{
public:
	FepDataManagerI(WorkStationServerThread* threadPtr);
	~FepDataManagerI();

public:
	virtual void processData(const ::FepData::DataPacket&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void processFault(const ::FepData::FaultPacket&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void processEvent(const ::FepData::EventPacket&, const ::Ice::Current& /* = ::Ice::Current() */);
	virtual void processWave(const ::FepData::WavePacket&, const ::Ice::Current& /* = ::Ice::Current() */);

private:
	WorkStationServerThread*	m_threadPtr;
};
#endif