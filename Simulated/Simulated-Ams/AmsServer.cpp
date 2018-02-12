
#include "aml.h"
#include "AmsServer.h"
#include "AmsServerThread.h"
#include "OperationInfo.h"

using namespace Aml;
using namespace Amssrv;

AmsServer::AmsServer(AmsServerThread* threadPtr)
	: Ice::Application(Ice::NoSignalHandling),
	  m_threadPtr(threadPtr), m_dataSrvHost("192.168.3.25"), m_dataSrvPort(10003)
{

}

int AmsServer::run( int argc, char* argv[] )
{
	try 
	{
		callbackOnInterrupt();

		OperationInfo info(TYPE_AMS);

		Ice::CommunicatorPtr communicatorPtr = communicator();
		info.setOperationInfo("建立连接器");
		m_threadPtr->putMessage(info);
		
		::Ice::PropertiesPtr props = communicator()->getProperties();
		string adapterName = "AmsApp";
		string amsAppName = "amsApp";
		if (props)
		{
			adapterName = props->getProperty("AdapterName");
			amsAppName = props->getProperty("AmsAppName");
		}
		
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter(adapterName);
		info.setOperationInfo("获取对象适配器");
		m_threadPtr->putMessage(info);

		amsApp = new CAmsAppI(m_threadPtr);
		adapter->add(amsApp, communicator()->stringToIdentity(amsAppName));


		// 通过属性设置数据服务器节点信息
		if (props)
		{
			string dataSrvIp = props->getPropertyWithDefault("DataSrvIp", "192.168.3.25");
			int dataSrvPort = props->getPropertyAsIntWithDefault("DataSrvPort", DATASRV_ADAPTER_PORT);
			((CAmsAppI*)amsApp.get())->setDataSrvInfo(dataSrvIp, dataSrvPort);

			ServerNodeSeq serverNodes;
			ServerNode node;
			node.type = TYPE_AMS;
			node.ip = props->getPropertyWithDefault("Ams1_ip", "192.168.3.25");
			node.port = props->getPropertyAsIntWithDefault("Ams1_port", 10002);
			node.runRole = props->getPropertyWithDefault("Ams1_runRole", "master");
			serverNodes.push_back(node);

			node.ip = props->getPropertyWithDefault("Ams2_ip", "192.168.3.15");
			node.port = props->getPropertyAsIntWithDefault("Ams2_port", 10002);
			node.runRole = props->getPropertyWithDefault("Ams2_runRole", "slave");
			serverNodes.push_back(node);

			node.type = TYPE_DATASRV;
			node.ip = props->getPropertyWithDefault("Datasvr1_ip", "192.168.3.25");
			node.port = props->getPropertyAsIntWithDefault("Datasvr1_port", 10003);
			node.runRole = props->getPropertyWithDefault("Datasvr1_runRole", "master");
			serverNodes.push_back(node);


			node.ip = props->getPropertyWithDefault("Datasvr2_ip", "192.168.3.15");
			node.port = props->getPropertyAsIntWithDefault("Datasvr2_port", 10003);
			node.runRole = props->getPropertyWithDefault("Datasvr2_runRole", "slave");
			serverNodes.push_back(node);

			node.type = TYPE_ICESTORM;
			node.ip = props->getPropertyWithDefault("IceStrom1_ip", "192.168.3.25");
			node.port = props->getPropertyAsIntWithDefault("IceStrom1_port", 10000);
			node.runRole = props->getPropertyWithDefault("IceStrom1_runRole", "master");
			serverNodes.push_back(node);
			amsApp->setServerNodeList(serverNodes);
		}

		adapter->activate();
		info.setOperationInfo("激活对象适配器");
		m_threadPtr->putMessage(info);
	}
	catch(const ::Ice::Exception& ex)
	{
		OperationInfo info(TYPE_AMS);
		info.setOperationInfo("启动Ice服务", QDateTime(), false, ex.what());
		m_threadPtr->putMessage(info);
		return EXIT_FAILURE;
	}

	communicator()->waitForShutdown();
	OperationInfo info(TYPE_AMS);
	info.setOperationInfo("AmsServer服务退出!");
	m_threadPtr->putMessage(info);
	
	return EXIT_SUCCESS;
}

void AmsServer::setRole( const string& role )
{
	((CAmsAppI*)amsApp.get())->setRole(role);
}

void AmsServer::setHostPort( const string& host, int port )
{
	m_dataSrvHost = host;
	m_dataSrvPort = port;
	((CAmsAppI*)amsApp.get())->setDataSrvInfo(host, port);
}

void AmsServer::changeRole( const string& role )
{
	OperationInfo info(TYPE_AMS);
	info.setOperationInfo("开始切换角色!");
	m_threadPtr->putMessage(info);

	try
	{
		string amlRoleProxy = QString("role: tcp -h %1 -p %2")
			.arg(QString().fromStdString(m_dataSrvHost)).arg(m_dataSrvPort).toStdString();
		CAmlRolePrx amlRolePrx = CAmlRolePrx::checkedCast(communicator()->stringToProxy(amlRoleProxy));
		CAmlRolePrx timeoutAmlRolePrx = CAmlRolePrx::uncheckedCast(amlRolePrx->ice_timeout(ICE_TIMEOUT));

		timeoutAmlRolePrx->SetRole(role);
		//string role = timeoutAmlRolePrx->GetRole();

		info.setOperationInfo("切换角色成功!");
	}
	catch (Ice::ConnectTimeoutException& ex) 
	{
		info.setOperationInfo("切换角色失败：ConnectTimeoutException!");
	}
	catch (Ice::TimeoutException) 
	{
		info.setOperationInfo("切换角色失败：TimeoutException!");
	}
	catch (Ice::Exception& ex)
	{
		info.setOperationInfo("切换角色失败：Exception!");
	}

	m_threadPtr->putMessage(info);
}
