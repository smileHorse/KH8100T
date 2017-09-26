#include "fepserverthread.h"
#include "BaseIceStorm.h"
#include "OperationInfo.h"

#include <string>

using namespace std;

void FepServerThread::setCommunicatorPtr( Ice::CommunicatorPtr ptr )
{
	m_communicatorPtr = ptr;
}

void FepServerThread::run()
{

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
			OperationInfo info(TYPE_FEP);
			info.setOperation(QStringLiteral("获取前置机数据的发布对象"));
			info.setOperTime();
			info.setResult(false);
			info.setReason(QStringLiteral("未知"));
			emit executeOperation(info);
			return false;
		}
		else
		{
			m_fepDataManagerPrx = FepData::FepDataManagerPrx::uncheckedCast(proxy);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_FEP);
		info.setOperation(QStringLiteral("获取前置机数据的发布对象"));
		info.setOperTime();
		info.setResult(false);
		info.setReason(ex.what());
		emit executeOperation(info);
		return false;
	}
	catch(...)
	{
		OperationInfo info(TYPE_FEP);
		info.setOperation(QStringLiteral("获取前置机数据的发布对象"));
		info.setOperTime();
		info.setResult(false);
		info.setReason(QStringLiteral("未知"));
		emit executeOperation(info);
		return false;
	}

	OperationInfo info(TYPE_FEP);
	info.setOperation(QStringLiteral("获取前置机数据的发布对象"));
	info.setOperTime();
	info.setResult(true);
	emit executeOperation(info);
	return true;
}

void FepServerThread::processData()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布全数据
	FepData::DataPacket packet;
	packet.id = 15;
	packet.fepNode = "fep36";
	packet.type = FepData::AllDataType;
	packet.unitNo = 1;
	for (int i = 0; i < 128; ++i)
	{
		packet.analogs.push_back(i);
		packet.discretes.push_back(i % 2);
		packet.accmulators.push_back(i+1);
	}
	m_fepDataManagerPrx->processData(packet);

	OperationInfo info(TYPE_FEP);
	info.setOperation(QStringLiteral("发布数据"));
	info.setOperTime();
	info.setResult(true);
	emit executeOperation(info);
}

void FepServerThread::processFault()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布故障事项
	FepData::FaultPacket packet;
	packet.id = 15;
	packet.fepNode = "fep36";
	for (int i = 1; i < 5; ++i)
	{
		FepData::FaultEvent event;
		event.unitNo = i;
	}
	m_fepDataManagerPrx->processFault(packet);

	OperationInfo info(TYPE_FEP);
	info.setOperation(QStringLiteral("发布故障事项"));
	info.setOperTime();
	info.setResult(true);
	emit executeOperation(info);
}

void FepServerThread::processEvent()
{

}

void FepServerThread::processWave()
{

}
