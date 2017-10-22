#ifndef RDBALARMDATAI_H
#define RDBALARMDATAI_H

#include "rdbdata.h"

class WorkStationServerThread;

class RdbAlarmDataI : public RdbWarningData::RdbAlarmData
{
public:
	RdbAlarmDataI(WorkStationServerThread* threadPtr);

	virtual void SendAlarmData(const ::RdbWarningData::WarningInfoSeq& seq, const ::Ice::Current& /* = ::Ice::Current */);

private:
	WorkStationServerThread*	m_threadPtr;
};

#endif