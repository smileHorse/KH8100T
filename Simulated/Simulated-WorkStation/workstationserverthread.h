#ifndef WORKSTATIONSERVERTHREAD_H
#define WORKSTATIONSERVERTHREAD_H

#include "rdbdata.h"

#include <QThread>
#include <Ice/ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>

class OperationInfo;

#pragma execution_character_set("utf-8")

class WorkStationServerThread : public QThread
{
	Q_OBJECT

public:
	void setCommunicatorPtr(Ice::CommunicatorPtr ptr);

protected:
	virtual void run();

private:
	bool getRdbRealDataRequestPublisher();
	bool subscribeRdbRealData();

signals:
	void executeOperation(const OperationInfo& text);

private slots:

private:
	Ice::CommunicatorPtr m_communicatorPtr;
	RdbRealData::RdbRealDataRequestPrx	m_rdbRealDataRequestPrx;
};

#endif