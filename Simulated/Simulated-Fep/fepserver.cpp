#include "fepserver.h"
#include "fepserverthread.h"
#include "BaseIceStorm.h"
#include "fepdata.h"
#include "AmlConst.h"
#include "OperationInfo.h"

#include <string>
using namespace std;

FepServer::FepServer( int argc, char* argv[] )
	: m_argc(argc), m_argv(argv)
{

}

int FepServer::run( int argc, char* argv[] )
{
	try 
	{
		m_communicatorPtr = communicator();
		OperationInfo info(TYPE_FEP);
		info.setOperationInfo(QStringLiteral("创建连接器"));
		emit executeOperation(info);
		
		string adapterName = "FepAdatpter";
		::Ice::PropertiesPtr props = m_communicatorPtr->getProperties();
		if (props)
		{
			adapterName = props->getPropertyWithDefault("AdapterName", adapterName);
		}

		Ice::ObjectAdapterPtr adapter = m_communicatorPtr->createObjectAdapter(adapterName);
		info.setOperationInfo(QStringLiteral("获取适配器成功"));
		emit executeOperation(info);

		// 建立子线程用于发送事项和数据
		m_threadPtr->setCommunicatorPtr(m_communicatorPtr);

		adapter->activate();
		info.setOperationInfo(QStringLiteral("激活适配器"));
		emit executeOperation(info);

		m_communicatorPtr->waitForShutdown();
		info.setOperationInfo(QStringLiteral("关闭Ice服务"));
		emit executeOperation(info);

		communicator()->shutdown();
		communicator()->destroy();
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_FEP);
		info.setOperationInfo(QStringLiteral("启动Ice服务"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void FepServer::setThreadPtr( FepServerThread* ptr )
{
	m_threadPtr = ptr;
}

void FepServer::startServer()
{
	main(m_argc, m_argv, "config.server");
}
