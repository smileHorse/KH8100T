#ifndef WORKSTATIONSERVER_H
#define WORKSTATIONSERVER_H

#include "rdbdata.h"

#include <QObject>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>

class OperationInfo;
class WorkStationServerThread;

#pragma execution_character_set("utf-8")

class WorkStationServer : public QObject, public Ice::Application
{
	Q_OBJECT

public:
	WorkStationServer(int argc, char* argv[]);
	virtual int run(int argc, char* argv[]);

	void setThreadPtr(WorkStationServerThread* ptr);

private:
	// 测试接口 begin
	// 查询全部数据
	bool SelectCompleteData(RdbRealData::RdbDataOptPrx& proxy);
	// 查询树形结构
	bool GetEquipTree(RdbRealData::RdbDataOptPrx& proxy);
	// 测试接口 end

private slots:
	void startServer();

signals:
	void executeOperation(const OperationInfo& text);

private:
	int	m_argc;
	char**	m_argv;

	WorkStationServerThread*	m_threadPtr;

	Ice::CommunicatorPtr m_communicatorPtr;
};
#endif