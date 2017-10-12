#include "workstationserver.h"
#include "BaseIceStorm.h"
#include "AmlConst.h"
#include "OperationInfo.h"
#include "workstationserverthread.h"

#include <string>
using namespace std;

WorkStationServer::WorkStationServer( int argc, char* argv[] )
	: m_argc(argc), m_argv(argv)
{

}

int WorkStationServer::run( int argc, char* argv[] )
{
	try 
	{
		m_communicatorPtr = communicator();
		OperationInfo info(TYPE_CLIENT);
		info.setOperation("创建连接器成功");
		info.setOperTime();
		info.setResult(true);
		emit executeOperation(info);

		QString endPoints("default -h localhost -p %1");
		endPoints = endPoints.arg(WORKSTATION_ADAPTER_PORT);
		Ice::ObjectAdapterPtr adapter = m_communicatorPtr->createObjectAdapterWithEndpoints("workStationAdatpter", 
			endPoints.toStdString());
		info.setOperation("获取适配器成功");
		info.setOperTime();
		info.setResult(true);
		emit executeOperation(info);

		RdbRealData::RdbDataOptPrx rdbOptPrx = RdbRealData::RdbDataOptPrx::checkedCast(
			communicator()->stringToProxy("rdb-opt:default -h 192.168.3.25 -p 10003 -t 5000"));
		if (rdbOptPrx)
		{
			// 查询树
			GetEquipTree(rdbOptPrx);
		}

		// 建立子线程用于发送事项和数据
		if (m_threadPtr)
		{
			m_threadPtr->setCommunicatorPtr(m_communicatorPtr);
			m_threadPtr->setObjectAdapterPtr(adapter);
			m_threadPtr->start();
		}

		adapter->activate();
		info.setOperation("激活适配器");
		info.setOperTime();
		info.setResult(true);
		emit executeOperation(info);

		m_communicatorPtr->waitForShutdown();
		info.setOperation("关闭Ice服务");
		info.setOperTime();
		info.setResult(true);
		emit executeOperation(info);

		communicator()->shutdown();
		communicator()->destroy();
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperation("启动Ice服务");
		info.setOperTime();
		info.setResult(false);
		info.setReason(ex.what());
		emit executeOperation(info);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void WorkStationServer::setThreadPtr( WorkStationServerThread* ptr )
{
	m_threadPtr = ptr;
}

void WorkStationServer::startServer()
{
	main(m_argc, m_argv);
}

// 查询树形结构
bool WorkStationServer::GetEquipTree( RdbRealData::RdbDataOptPrx& proxy )
{
	string deviceType = "SubGeographicalRegion";
	string deviceRid = "{a1d602d4-5ef7-4031-bf86-0c44682161bb}";
	RdbRealData::EquipTreeSequence treeSeq;
	proxy->GetEquipTree(deviceType, deviceRid, treeSeq);
	return true;
}

// 查询全部数据
bool WorkStationServer::SelectCompleteData( RdbRealData::RdbDataOptPrx& proxy )
{
	RdbRealData::RequestDefaultDataSeq requestSeq;
	requestSeq.id = 0;
	requestSeq.requestId = 0;
	requestSeq.requestNode = "workstation";
	requestSeq.isStop = false;
	requestSeq.refreshFreq = 0;
	RdbRealData::RequestDefaultData	request;
	request.tableName = "SubGeographicalRegion";

	RdbRealData::RespondDefaultDataSeq respondSeq;
	proxy->SelectDefaultData(requestSeq, respondSeq);

	return true;
}
