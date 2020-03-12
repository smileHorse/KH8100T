#include "FepDataManagerI.h"
#include "workstationserverthread.h"

#include <QDateTime>

FepDataManagerI::FepDataManagerI( WorkStationServerThread* threadPtr )
	: m_threadPtr(threadPtr)
{

}

FepDataManagerI::~FepDataManagerI()
{

}

void FepDataManagerI::processData( const ::FepData::DataPacket& data, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1-%2-%3 %4\t%5").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(data.id).arg(QString().fromStdString(data.fepNode));

	m_threadPtr->outputOperationData(text);
}

void FepDataManagerI::processFault( const ::FepData::FaultPacket& data, const ::Ice::Current& /* = ::Ice::Current() */ )
{
}

void FepDataManagerI::processEvent( const ::FepData::EventPacket& data, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1-%2-%3 %4\t%5").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(data.id).arg(QString().fromStdString(data.fepNode));

	if (data.type == ProType)
	{
		for (size_t i = 0; i < data.protects.size(); i++)
		{

		}
	}
	m_threadPtr->outputOperationData(text);
}

void FepDataManagerI::processWave( const ::FepData::WavePacket&, const ::Ice::Current& /* = ::Ice::Current() */ )
{

}
