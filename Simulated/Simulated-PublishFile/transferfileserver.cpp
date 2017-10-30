
#include "transferfileserver.h"
#include "transloggerI.h"
#include "FileHandler.h"
#include "BaseIceStorm.h"

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>

TransferFileServer::TransferFileServer(int argc, char* argv[])
{
	m_argc = argc;
	m_argv = argv;

	m_publishFilePrx = 0;
	m_subscriberFilePrx = 0;
}

TransferFileServer::~TransferFileServer()
{

}

int TransferFileServer::run( int argc, char* argv[] )
{
	try 
	{
		m_communicatorPtr = communicator();
		QString endPoints("default -h 192.168.3.25 -p %1");

		// 发布程序
		endPoints = endPoints.arg(10006);
		m_objectAdapterPtr = communicator()->createObjectAdapterWithEndpoints("pubAdatpter", 
			endPoints.toStdString());
			
		m_objectAdapterPtr->activate();

		communicator()->waitForShutdown();

		communicator()->shutdown();
		communicator()->destroy();
	}
	catch(const Ice::Exception& ex)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void TransferFileServer::start()
{
	main(m_argc, m_argv);
}

void TransferFileServer::publishFile(bool isStart)
{
	if (isStart)
	{
		emit outputOperationInfo(QStringLiteral("开始发布文件..."));

		// 获取发布接口
		if (!getPublisherPtr())
		{
			return;
		}

		// 读取文件内容
		string fileName = "source.log";
		string content = "";
		CFileHandler fileHandler;
		if (!fileHandler.readFileWithQDataStream(fileName, content))
		{
			emit outputOperationInfo(QStringLiteral("读取文件出错"));
		}

		m_publishFilePrx->transferRdbLogger(fileName, content);

		emit outputOperationInfo(QStringLiteral("完成发布文件..."));
	}
	else
	{

		emit outputOperationInfo(QStringLiteral("停止发布文件..."));
	}
}

void TransferFileServer::subscriberFile(bool isStart)
{
	// 创建订阅接口
	if (m_subscriberFilePrx == NULL)
	{
		m_subscriberFilePrx = m_objectAdapterPtr->add(new TransferRdbLoggerInfoI(), 
			m_communicatorPtr->stringToIdentity("file-subscriber"));
	}

	if (isStart)
	{
		emit outputOperationInfo(QStringLiteral("开始订阅文件..."));

		//订阅相关的接口
		std::string strTopic = "";
		std::string strDeliverModel = "oneway";
		std::string strReliability = "";
		std::string strRetryCount = "";

		//订阅实时数据请求数据
		strTopic = TransferRdbLogger::TransferRdbLoggerTopic;
		bool result = BaseIceStorm::Subscriber(communicator(), m_subscriberFilePrx, strTopic, strDeliverModel, 
			strReliability, strRetryCount);
		if (!result)
		{
			emit outputOperationInfo(QStringLiteral("订阅文件失败..."));
			return;
		}

		emit outputOperationInfo(QStringLiteral("完成订阅文件..."));
	}
	else
	{
		emit outputOperationInfo(QStringLiteral("停止订阅文件..."));

		bool result = true;

		//取消订阅 
		std::string strTopic = TransferRdbLogger::TransferRdbLoggerTopic;
		result = BaseIceStorm::UnSubscriber(communicator(), m_subscriberFilePrx, strTopic);
		if (!result)
		{
			emit outputOperationInfo(QStringLiteral("取消订阅文件失败..."));
			return;
		}

		emit outputOperationInfo(QStringLiteral("完成停止订阅文件..."));
	}
}

// 获取发布接口
bool TransferFileServer::getPublisherPtr()
{
	if (m_publishFilePrx != 0)
		return true;

	try
	{
		std::string strTopic;
		std::string strDeliverModel;
		Ice::ObjectPrx objPrx;

		//获得"事项数据"发布接口
		strTopic = TransferRdbLogger::TransferRdbLoggerTopic;
		strDeliverModel = "oneway";
		objPrx = BaseIceStorm::GetPublisher(communicator(), strTopic, strDeliverModel);

		if (objPrx != 0)
		{
			m_publishFilePrx = TransferRdbLogger::TransferRdbLoggerInfoPrx::uncheckedCast(objPrx);
			emit outputOperationInfo(QStringLiteral("获取发布数据接口成功"));
			return true;
		}
		else
		{
			m_publishFilePrx = NULL;
			emit outputOperationInfo(QStringLiteral("获取发布数据接口失败"));
			return false;
		}

	} catch (Ice::Exception& e)
	{
		emit outputOperationInfo(QStringLiteral("获取发布数据接口异常:%1").arg(e.what()));

		m_publishFilePrx = 0;
		return false;
	}
}

// 移除发布接口
bool TransferFileServer::removePublisherPtr()
{
	return true;
}
