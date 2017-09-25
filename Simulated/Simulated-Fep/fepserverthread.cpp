#include "fepserverthread.h"
#include "BaseIceStorm.h"

#include <string>

using namespace std;

void FepServerThread::setCommunicatorPtr( Ice::CommunicatorPtr ptr )
{
	m_communicatorPtr = ptr;
}

void FepServerThread::run()
{

}

void FepServerThread::processData()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	FepData::DataPacket packet;
	packet.id = 1000;
	packet.fepNode = "主前置机";
	packet.type = FepData::AllDataType;
	packet.unitNo = 1;

	m_fepDataManagerPrx->processData(packet);

	emit executeOperation("发布数据成功");
}

bool FepServerThread::getFepDataPublisher()
{
	if (m_fepDataManagerPrx)
	{
		return true;
	}

	// 获取前置机数据的发布者
	string topicName = FepData::strDataTopic;
	string strDeliverModel = "";
	try 
	{
		Ice::ObjectPrx proxy = BaseIceStorm::GetPublisher(m_communicatorPtr, topicName, strDeliverModel);
		if (!proxy)
		{
			emit executeOperation("获取前置机数据的发布对象失败");
			return false;
		}
		else
		{
			m_fepDataManagerPrx = FepData::FepDataManagerPrx::uncheckedCast(proxy);
		}
	}
	catch(const Ice::Exception& ex)
	{
		QString error = "获取前置机数据的发布对象失败: ";
		emit executeOperation(error + ex.what());
		return false;
	}
	catch(...)
	{
		QString error = "获取前置机数据的发布对象失败: ";
		emit executeOperation("未知异常");
		return false;
	}

	emit executeOperation("获取前置机数据发布对象成功");
	return true;
}
