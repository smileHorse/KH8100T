#include "workstationserver.h"
#include "BaseIceStorm.h"
#include "AmlConst.h"
#include "OperationInfo.h"
#include "workstationserverthread.h"
#include "IceHelper.h"

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
		OperationInfo info(TYPE_CLIENT);

		// 获取配置信息
		Ice::PropertiesPtr props = communicator()->getProperties();
		Ice::Int maxSize = props->getPropertyAsIntWithDefault("Ice.MessageSizeMax", 1024);
		string strEndPoints = props->getPropertyWithDefault("WorkStation.Endpoints", "");
		string strProxyPoints = props->getPropertyWithDefault("WorkStation.Proxypoints", "");
		string iceStormIps = props->getPropertyWithDefault("IceStrom_Ip", "");
		string iceStormPorts = props->getPropertyWithDefault("IceStrom_Port", "");
		if (strEndPoints.empty() || iceStormIps.empty() || iceStormPorts.empty())
		{
			info.setOperationInfo("加载本地和IceStorm端口信息失败，退出!!!!");
			emit executeOperation(info);
			return EXIT_FAILURE;
		}

		BaseIceStorm::SetIceStormIpAndPort(iceStormIps, iceStormPorts);

		m_communicatorPtr = communicator();
		info.setOperationInfo("创建连接器成功");
		emit executeOperation(info);

		QString endPoints = QString::fromStdString(strEndPoints);
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

		// 设置Ice信息
		CIceHelper::instance()->setCommunicatorPtr(m_communicatorPtr);
		CIceHelper::instance()->setProxyPoints(strProxyPoints);

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
	main(m_argc, m_argv, "config.info");
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
