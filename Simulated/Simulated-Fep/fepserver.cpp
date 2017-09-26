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
		info.setOperation(QStringLiteral("创建连接器"));
		info.setOperTime();
		info.setResult(true);
		emit executeOperation(info);

		QString endPoints("default -h localhost -p %1");
		endPoints = endPoints.arg(FEP_ADAPTER_PORT);
		Ice::ObjectAdapterPtr adapter = m_communicatorPtr->createObjectAdapterWithEndpoints("fepAdatpter", 
			endPoints.toStdString());
		info.setOperation(QStringLiteral("获取适配器成功"));
		info.setOperTime();
		info.setResult(true);
		emit executeOperation(info);

		// 建立子线程用于发送事项和数据
		m_threadPtr->setCommunicatorPtr(m_communicatorPtr);

		adapter->activate();
		info.setOperation(QStringLiteral("激活适配器"));
		info.setOperTime();
		info.setResult(true);
		emit executeOperation(info);

		m_communicatorPtr->waitForShutdown();
		info.setOperation(QStringLiteral("关闭Ice服务"));
		info.setOperTime();
		info.setResult(true);
		emit executeOperation(info);

		communicator()->shutdown();
		communicator()->destroy();
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_FEP);
		info.setOperation(QStringLiteral("启动Ice服务"));
		info.setOperTime();
		info.setResult(false);
		info.setReason(ex.what());
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
	main(m_argc, m_argv);
}
