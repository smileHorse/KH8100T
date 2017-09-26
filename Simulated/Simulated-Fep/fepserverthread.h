#ifndef FEPSERVERTHREAD_H
#define FEPSERVERTHREAD_H

#include "fepdata.h"

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

signals:
	void executeOperation(const OperationInfo& text);

private slots:
	void processData();
	void processFault();
	void processEvent();
	void processWave();

private:
	Ice::CommunicatorPtr m_communicatorPtr;
	FepData::FepDataManagerPrx	m_fepDataManagerPrx;
};

#endif