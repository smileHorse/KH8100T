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
		info.setOperationInfo("创建连接器成功");
		emit executeOperation(info);

		QString endPoints("default -h 192.168.3.25 -p %1");
		endPoints = endPoints.arg(WORKSTATION_ADAPTER_PORT);
		Ice::ObjectAdapterPtr adapter = m_communicatorPtr->createObjectAdapterWithEndpoints("workStationAdatpter", 
			endPoints.toStdString());
		info.setOperationInfo("获取适配器成功");
		emit executeOperation(info);
		
		// 建立子线程用于发送事项和数据
		if (m_threadPtr)
		{
			m_threadPtr->setCommunicatorPtr(m_communicatorPtr);
			m_threadPtr->setObjectAdapterPtr(adapter);
			m_threadPtr->start();
		}

		adapter->activate();
		info.setOperationInfo("激活适配器");
		emit executeOperation(info);

		m_communicatorPtr->waitForShutdown();
		info.setOperationInfo("关闭Ice服务");
		emit executeOperation(info);

		communicator()->shutdown();
		communicator()->destroy();
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo("启动Ice服务", QDateTime(), false, ex.what());
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
	string deviceRid = "fb14623d-9d1c-4572-8e4f-803f5675ad30";
	RdbRealData::EquipTreeSequence treeSeq;
	proxy->GetEquipTree(deviceType, deviceRid, treeSeq);	
	proxy->GetSpecificEquipTree(deviceType, deviceRid, "Breaker", treeSeq);
	
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
