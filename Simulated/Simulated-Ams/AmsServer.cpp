
#include "AmsServer.h"
#include "AmsServerThread.h"
#include "OperationInfo.h"

using namespace Amssrv;

AmsServer::AmsServer(AmsServerThread* threadPtr)
	: Ice::Application(Ice::NoSignalHandling),
	  m_threadPtr(threadPtr)
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
	((CAmsAppI*)amsApp.get())->setDataSrvInfo(host, port);
}
