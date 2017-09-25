#include "workstationserverthread.h"
#include "BaseIceStorm.h"
#include "OperationInfo.h"

#include <string>

using namespace std;

void WorkStationServerThread::setCommunicatorPtr( Ice::CommunicatorPtr ptr )
{
	m_communicatorPtr = ptr;
}

void WorkStationServerThread::run()
{

}

bool WorkStationServerThread::getRdbRealDataRequestPublisher()
{
	if (m_rdbRealDataRequestPrx)
	{
		return true;
	}

	// 获取前置机数据的发布者
	string topicName = RdbRealData::strRealRequestTopic;
	string strDeliverModel = "";
	try 
	{
		Ice::ObjectPrx proxy = BaseIceStorm::GetPublisher(m_communicatorPtr, topicName, strDeliverModel);
		if (!proxy)
		{
			OperationInfo info(TYPE_CLIENT);
			info.setOperation("获取数据请求发布对象失败");
			info.setOperTime();
			info.setResult(false);
			info.setReason("失败原因未知");
			emit executeOperation(info);
			return false;
		}
		else
		{
			m_rdbRealDataRequestPrx = RdbRealData::RdbRealDataRequestPrx::uncheckedCast(proxy);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperation("获取数据请求发布对象失败");
		info.setOperTime();
		info.setResult(false);
		info.setReason(ex.what());
		emit executeOperation(info);
		return false;
	}
	catch(...)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperation("获取数据请求发布对象失败");
		info.setOperTime();
		info.setResult(false);
		info.setReason("失败原因未知");
		emit executeOperation(info);
		return false;
	}

	OperationInfo info(TYPE_CLIENT);
	info.setOperation("获取数据请求发布对象成功");
	info.setOperTime();
	info.setResult(true);
	emit executeOperation(info);
	return true;
}

bool WorkStationServerThread::subscribeRdbRealData()
{
	return true;
}
