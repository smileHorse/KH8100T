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
	QString text = QString("%1-%2-%3 %4 %5\t%6").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(data.id).arg("data").arg(QString().fromStdString(data.fepNode));

	text.append(QString("unitNo: %1").arg(data.unitNo)).append("\n");
	text.append(QString("type: %1").arg(data.type)).append("\n");

	/*if (!data.analogs.empty()) {
	text.append("Analog:\n");
	for (::FepData::AnalogSeq::const_iterator iter = data.analogs.begin(); iter != data.analogs.end(); iter++)
	{
	text.append(QString("%1\t").arg(*iter));
	}
	}

	if (!data.discretes.empty()) {
	text.append("\nDiscrete:\n");
	for (::FepData::DiscreteSeq::const_iterator iter = data.discretes.begin(); iter != data.discretes.end(); iter++)
	{
	text.append(QString("%1\t").arg(*iter));
	}
	}

	if (!data.accmulators.empty()) {
	text.append("\nAccumulator:\n");
	for (::FepData::AccmulatorSeq::const_iterator iter = data.accmulators.begin(); iter != data.accmulators.end(); iter++)
	{
	text.append(QString("%1\t").arg(*iter));
	}
	}

	if (!data.changedAnalogs.empty()) {
	text.append("\nChangedAnalogSeq:\n");
	for (::FepData::ChangedAnalogSeq::const_iterator iter = data.changedAnalogs.begin(); iter != data.changedAnalogs.end(); iter++)
	{
	text.append(QString("timeStamp:%1, unitNo: %2, index: %3, value: %4").arg(iter->timeStamp).arg(iter->unitNo).arg(iter->index).arg(iter->value));
	}
	}*/

	if (!data.units.empty()) {
		text.append("\UnitSeq:\n");
		for (::FepData::UnitSeq::const_iterator iter = data.units.begin(); iter != data.units.end(); iter++)
		{
			if (iter->unitNo != 28) {
				continue;
			}
			text.append(QString("unitNo:%1,unitState:%2,channel1:%3,channel2:%4,num:%5,currentNo:%6,[gateWay-rtuAddr-unitState]:[%7-%8-%9;%10-%11-%12],errorRate:%13\n")
				.arg(iter->unitNo).arg(iter->unitState).arg(iter->channelState1).arg(iter->channelState2)
				.arg(iter->channelNum).arg(iter->currentNo).arg(QString::fromStdString(iter->gateWay))
				.arg(QString::fromStdString(iter->rtuAddr))
				.arg(iter->unitState1).arg(QString::fromStdString(iter->gateWay1))
				.arg(QString::fromStdString(iter->rtuAddr1)).arg(iter->unitState2)
				.arg(iter->errorRate));
		}

		m_threadPtr->outputOperationData(text);
	}

	//m_threadPtr->outputOperationData(text);
}

void FepDataManagerI::processFault( const ::FepData::FaultPacket& data, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1-%2-%3 %4 %5\t%6").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(data.id).arg("fault").arg(QString().fromStdString(data.fepNode));

	m_threadPtr->outputOperationData(text);
}

void FepDataManagerI::processEvent( const ::FepData::EventPacket& data, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1-%2-%3 %4 %5\t%6\n").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(data.id).arg("event").arg(QString().fromStdString(data.fepNode));

	if (data.type == ProType)
	{
		for (size_t i = 0; i < data.protects.size(); i++)
		{

		}
	}
	else if (data.type == UnitType) {
		for (size_t i = 0; i < data.units.size(); i++) {
			::FepData::ChangedUnit unitData = data.units.at(i);
			text.append(QString("unitNo:%1; unitState:%2, channel1:%3, channel2: %4\n").arg(unitData.unitNo).arg(unitData.unitState)
				.arg(unitData.channelState1).arg(unitData.channelState2));
		}
		m_threadPtr->outputOperationData(text);
	}
	else if (data.type == YxType) {
		for (size_t i = 0; i < data.digitals.size(); i++) {
			::FepData::ChangedDigital digital = data.digitals.at(i);
			text.append(QString("unitNo:%1; index:%2, value:%3\n").arg(digital.unitNo).arg(digital.index)
				.arg(digital.value));
		}
		m_threadPtr->outputOperationData(text);
	}
	else if (data.type == SoeType) {
		for (size_t i = 0; i < data.soes.size(); i++) {
			::FepData::Soe soe = data.soes.at(i);
			text.append(QString("unitNo:%1; index:%2, value:%3, isYK:%4\n").arg(soe.unitNo).arg(soe.index)
				.arg(soe.value).arg(soe.IsYK));
		}
		m_threadPtr->outputOperationData(text);
	}
	//m_threadPtr->outputOperationData(text);
}

void FepDataManagerI::processWave( const ::FepData::WavePacket&, const ::Ice::Current& /* = ::Ice::Current() */ )
{

}
