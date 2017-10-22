#ifndef FEPSERVERTHREAD_H
#define FEPSERVERTHREAD_H

#include "fepdata.h"
#include "processDataDlg.h"

#include <QThread>
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

private slots:
	void processYxData();
	void processYcData();
	void processDdData();
	void processUnitStateData();
	void processDataPacket(SelfDataPacket packet);
	void processDLFault();
	void processJDFault();
	void processYxTypeEvent();
	void processSoeTypeEvent();
	void processUnitTypeEvent();
	void processProTypeEvent();
	void processWave();

private:
	Ice::CommunicatorPtr m_communicatorPtr;
	FepData::FepDataManagerPrx	m_fepDataManagerPrx;
};

#endif