#ifndef FEPSERVERTHREAD_H
#define FEPSERVERTHREAD_H

#include "fepdata.h"
#include "processDataDlg.h"

#include <QtCore/QtCore>
#include <Ice/ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>

class OperationInfo;

#pragma execution_character_set("utf-8")

class FepServerThread : public QThread
{
	Q_OBJECT

public:
	void setCommunicatorPtr(Ice::CommunicatorPtr ptr);

protected:
	virtual void run();

private:
	bool getFepDataPublisher();
	QString outputFepData(const FepData::DataPacket& packet);
	QString outputFepFault(const FepData::FaultPacket& packet);
	QString outputFepEvent(const FepData::EventPacket& packet);
	QString outputFepWave(const FepData::WavePacket& packet);

signals:
	void executeOperation(const OperationInfo& text);
	void publishFepData(const QString& text);

public slots:
	void processYxData();
	void processYcData();
	void processDdData();
	void processUnitStateData(bool isProcess);
	void processUnitStateDataImpl();
	void processDataPacket(SelfDataPacket packet);
	void processDLFault();
	void processJDFault();
	void processYxTypeEvent(bool isProcess);
	void processYxTypeEventImpl();
	void processSoeTypeEvent(bool isProcess);
	void processSoeTypeEventImpl();
	void processUnitTypeEvent();
	void processUnitChannelEvent();
	void processProTypeEvent();
	void processProTypeEvent(int unitNo, int moduleNo, int moduleType, int infoNo, int state);
	void processProTypeEvent(bool isProcess);
	void processProTypeEventImpl();
	void processWave();

private:
	int getRandomUnitNo();
	int getRandomIndex();
	::FepData::DiscreteValue getRandomDiscreteValue();

private:
	Ice::CommunicatorPtr m_communicatorPtr;
	FepData::FepDataManagerPrx	m_fepDataManagerPrx;
	QSharedPointer<QTimer>	m_unitStateTimer;
	QSharedPointer<QTimer>	m_yxTypeEventTimer;
	QSharedPointer<QTimer>	m_soeTypeEventTimer;
	QSharedPointer<QTimer>	m_proTypeEventTimer;
};

#endif