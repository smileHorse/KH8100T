#include "workstationserverthread.h"
#include "BaseIceStorm.h"
#include "OperationInfo.h"
#include "RdbAlarmDataI.h"

#include <string>

using namespace std;

template<class T> string transferToString(const T& value);

void WorkStationServerThread::setCommunicatorPtr( Ice::CommunicatorPtr ptr )
{
	m_communicatorPtr = ptr;
}

void WorkStationServerThread::setObjectAdapterPtr( Ice::ObjectAdapterPtr ptr )
{
	m_objectAdapterPtr = ptr;
}

void WorkStationServerThread::outputWarningData( const QString& text )
{
	emit outputReceiveData(text);
}

void WorkStationServerThread::run()
{
	// 创建订阅接口
	m_alarmDataPrx = m_objectAdapterPtr->add(new RdbAlarmDataI(this), 
		m_communicatorPtr->stringToIdentity("alarm-subscriber"));

	// 订阅数据
	subscribeData();
	
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

bool WorkStationServerThread::subscribeData()
{
	string strTopic = "";
	string strDeliverModel = "oneway";
	string strReliability = "";
	string strRetryCount = "";

	//订阅实时数据请求数据
	strTopic = RdbWarningData::strAlarmDataTopic;
	bool one_result = BaseIceStorm::Subscriber(m_communicatorPtr, m_alarmDataPrx, strTopic, strDeliverModel, strReliability,
		strRetryCount);
	if (!one_result)
	{
		return false;
	}

	return true;
}
