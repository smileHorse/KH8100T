#include "fepserverthread.h"
#include "BaseIceStorm.h"
#include "OperationInfo.h"
#include "textelement.h"

#include <string>
#include <sstream>

using namespace std;

template<class T>
string transferToString(const T& value)
{
	stringstream str;
	str << value;
	return str.str();
}

// 获取遥信状态值
string	getDiscreteValue(FepData::DiscreteValue value)
{
	switch(value)
	{
	case FepData::Open:
		return "分闸";
	case FepData::Close:
		return "合闸";
	case  FepData::Unknown:
	default:
		return "未知";
	}
}

// 获取终端状态值
string getState(FepData::State value)
{
	switch(value)
	{
	case FepData::NotInstall:
		return "未安装";
	case FepData::OffLine:
		return "离线";
	case FepData::New:
		return "投运";
	case FepData::Run:
		return "运行";
	case FepData::Stop:
		return "退出";
	case FepData::HighErrorRate:
		return "误码率高";
	default:
		return "未知";
	}
}

// 获取数据类型
string getDataType(FepData::DataType value)
{
	switch(value)
	{
	case FepData::AllDataType:
		return "全数据";
	case FepData::ChangedAnalogType:
		return "变化遥测";
	case FepData::AccumulatorType:
		return "电度";
	case FepData::UnitStateType:
		return "终端状态";
	default:
		return "未知";
	}
}

// 获取保护事项类型
string getProEventType(FepData::ProEventType value)
{
	switch(value)
	{
	case FepData::ProtectAction:
		return "保护动作";
	case FepData::ProtectAlarm:
		return "保护发信号";
	default:
		return "未知";
	}
}

// 获取事项类型
string getEventType(FepData::EventType value)
{
	switch(value)
	{
	case FepData::YxType:
		return "遥信变位";
	case FepData::SoeType:
		return "SOE";
	case FepData::UnitType:
		return "终端状态";
	case FepData::ProType:
		return "保护事项";
	default:
		return "未知";
	}
}

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
			info.setOperationInfo(QStringLiteral("获取前置机数据的发布对象"), QDateTime(), false, QStringLiteral("未知"));
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
		info.setOperationInfo(QStringLiteral("获取前置机数据的发布对象"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return false;
	}
	catch(...)
	{
		OperationInfo info(TYPE_FEP);
		info.setOperationInfo(QStringLiteral("获取前置机数据的发布对象"), QDateTime(), false, QStringLiteral("未知"));
		emit executeOperation(info);
		return false;
	}

	OperationInfo info(TYPE_FEP);
	info.setOperationInfo(QStringLiteral("获取前置机数据的发布对象"));
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
	// 发送5个数据
	for (int i = 1; i < 6; ++i)
	{
		packet.analogs.push_back(i);
		packet.analogs.push_back(i * 2);

		packet.discretes.push_back(i);
		packet.discretes.push_back(i % 2);

		packet.accmulators.push_back(i);
		packet.accmulators.push_back(i+1);
	}
	m_fepDataManagerPrx->processData(packet);

	// 输出发送的数据
	QString text = outputFepData(packet);
	emit publishFepData(text);

	OperationInfo info(TYPE_FEP);
	info.setOperationInfo(QStringLiteral("发布数据"));
	emit executeOperation(info);
}

void FepServerThread::processDLFault()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布短路故障事项
	FepData::FaultPacket packet;
	packet.id = 15;
	packet.fepNode = "fep36";
	FepData::FaultEvent event;
	event.unitNo = 1;
	event.lineNo = 1;
	event.timeStamp = IceUtil::Time::now().toMilliSeconds();
	event.eventType = 0;
	packet.events.push_back(event);

	m_fepDataManagerPrx->processFault(packet);

	OperationInfo info(TYPE_FEP);
	info.setOperationInfo(QStringLiteral("发布短路故障事项"));
	emit executeOperation(info);
}

void FepServerThread::processJDFault()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布接地故障事项--选线
	FepData::FaultPacket packet;
	packet.id = 15;
	packet.fepNode = "fep36";
	FepData::FaultEvent event;
	event.unitNo = 2;
	event.lineNo = 2;
	event.timeStamp = IceUtil::Time::now().toMilliSeconds();
	event.eventType = 2;
	event.credibility = 30;
	packet.events.push_back(event);
	
	// 发布接地故障事项--接地
	event.unitNo = 2;
	event.lineNo = 2;
	event.timeStamp = IceUtil::Time::now().toMilliSeconds();
	event.eventType = 1;
	event.directionFlag = 1;
	event.faultType = 0;
	event.directionMultiplier = 50;
	event.values.push_back(10);
	event.values.push_back(30);
	event.values.push_back(50);
	event.values.push_back(80);
	event.values.push_back(100);
	packet.events.push_back(event);
	
	// 发布接地故障事项--选线结束
	event.unitNo = 2;
	event.lineNo = 2;
	event.timeStamp = IceUtil::Time::now().toMilliSeconds();
	event.eventType = 3;
	packet.events.push_back(event);

	m_fepDataManagerPrx->processFault(packet);

	OperationInfo info(TYPE_FEP);
	info.setOperationInfo(QStringLiteral("发布接地故障事项"));
	emit executeOperation(info);
}

void FepServerThread::processYxTypeEvent()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布遥信变位事项
	FepData::EventPacket packet;
	packet.id = 15;
	packet.fepNode = "fep36";
	packet.type = FepData::YxType;
	FepData::ChangedDigital changedDigital;
	changedDigital.unitNo = 1;
	changedDigital.index = 1;
	changedDigital.value = FepData::Close;
	changedDigital.timeStamp = IceUtil::Time::now().toMilliSeconds();
	packet.digitals.push_back(changedDigital);

	m_fepDataManagerPrx->processEvent(packet);

	OperationInfo info(TYPE_FEP);
	info.setOperationInfo(QStringLiteral("发布遥信变位事项"));
	emit executeOperation(info);
}

void FepServerThread::processSoeTypeEvent()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布SOE事项
	FepData::EventPacket packet;
	packet.id = 15;
	packet.fepNode = "fep36";
	packet.type = FepData::SoeType;
	FepData::Soe soe;
	soe.unitNo = 1;
	soe.index = 1;
	soe.value = FepData::Open;
	soe.timeStamp = IceUtil::Time::now().toMilliSeconds();
	packet.soes.push_back(soe);

	m_fepDataManagerPrx->processEvent(packet);

	OperationInfo info(TYPE_FEP);
	info.setOperationInfo(QStringLiteral("发布SOE事项"));
	emit executeOperation(info);
}

void FepServerThread::processUnitTypeEvent()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布单元事项
	FepData::EventPacket packet;
	packet.id = 15;
	packet.fepNode = "fep36";
	packet.type = FepData::UnitType;
	FepData::ChangedUnit changedUnit;
	changedUnit.unitNo = 1;
	changedUnit.unitState = FepData::Run;
	changedUnit.channelState1 = FepData::Run;
	changedUnit.channelState2 = FepData::NotInstall;
	changedUnit.errorRate = 6;
	changedUnit.timeStamp = IceUtil::Time::now().toMilliSeconds();
	packet.units.push_back(changedUnit);

	m_fepDataManagerPrx->processEvent(packet);

	OperationInfo info(TYPE_FEP);
	info.setOperationInfo(QStringLiteral("发布单元事项"));
	emit executeOperation(info);
}

void FepServerThread::processProTypeEvent()
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布保护事项
	FepData::EventPacket packet;
	packet.id = 15;
	packet.fepNode = "fep36";
	packet.type = FepData::ProType;
	FepData::ProtectEvent protectEvent;
	protectEvent.unitNo = 1;
	protectEvent.Type = FepData::ProtectAlarm;
	protectEvent.timeStamp = IceUtil::Time::now().toMilliSeconds();
	protectEvent.moduleNo = 0;
	protectEvent.moduleType = 0;
	protectEvent.infoNo = 0;
	protectEvent.state = 0;
	packet.protects.push_back(protectEvent);

	m_fepDataManagerPrx->processEvent(packet);

	OperationInfo info(TYPE_FEP);
	info.setOperationInfo(QStringLiteral("发布保护事项"));
	emit executeOperation(info);
}

void FepServerThread::processWave()
{

}

QString FepServerThread::outputFepData( const FepData::DataPacket& packet )
{
	QString text;
	QString currTime = QDateTime::currentDateTime().toString(Qt::SystemLocaleLongDate);

	TextElement parent("前置机发送数据", currTime.toStdString());
	parent.insertChild( TextElement("数据包ID", transferToString<unsigned char>(packet.id)));
	parent.insertChild( TextElement("所属前置机", packet.fepNode));
	parent.insertChild( TextElement("数据类型", getDataType(packet.type)));
	parent.insertChild( TextElement("单元号", transferToString<short>(packet.unitNo)));
	if (!packet.units.empty())
	{
		TextElement units("终端状态", transferToString<size_t>(packet.units.size()), &parent);
		for (size_t i = 0; i < packet.units.size(); ++i)
		{
			units.insertChild(TextElement("终端编号", transferToString<short>(packet.units.at(i).unitNo)));
			units.insertChild(TextElement("单元状态", getState(packet.units.at(i).unitState)));
			units.insertChild(TextElement("主通道状态", getState(packet.units.at(i).channelState1)));
			units.insertChild(TextElement("副通道状态", getState(packet.units.at(i).channelState2)));
			units.insertChild(TextElement("误码率", transferToString<unsigned char>(packet.units.at(i).unitNo)));
		}
	}
	if (!packet.analogs.empty())
	{
		TextElement analogs("遥测数据", transferToString<size_t>(packet.analogs.size()), &parent);

		string value;
		for (size_t i = 0; i < packet.analogs.size(); ++i)
		{
			value += transferToString<int>(packet.analogs.at(i)) + "\t";
		}
		analogs.insertChild(TextElement("值", value));
	}
	if (!packet.discretes.empty())
	{
		TextElement discretes("遥信数据", transferToString<size_t>(packet.discretes.size()), &parent);

		string value;
		for (size_t i = 0; i < packet.discretes.size(); ++i)
		{
			value += transferToString<int>(packet.discretes.at(i)) + "\t";
		}
		discretes.insertChild(TextElement("值", value));
	}
	if (!packet.accmulators.empty())
	{
		TextElement accmulators("电度数据", transferToString<size_t>(packet.accmulators.size()), &parent);

		string value;
		for (size_t i = 0; i < packet.accmulators.size(); ++i)
		{
			value += transferToString<int>(packet.accmulators.at(i)) + "\t";
		}
		accmulators.insertChild(TextElement("值", value));
	}
	if (!packet.changedAnalogs.empty())
	{
		TextElement changedAnalogs("变化遥测数据", transferToString<size_t>(packet.changedAnalogs.size()), &parent);

		for (size_t i = 0; i < packet.changedAnalogs.size(); ++i)
		{
			changedAnalogs.insertChild(TextElement("时标", transferToString<long>(packet.changedAnalogs.at(i).timeStamp)));
			changedAnalogs.insertChild(TextElement("终端编号", transferToString<short>(packet.changedAnalogs.at(i).unitNo)));
			changedAnalogs.insertChild(TextElement("点号", transferToString<short>(packet.changedAnalogs.at(i).index)));
			changedAnalogs.insertChild(TextElement("值", transferToString<int>(packet.changedAnalogs.at(i).value)));
		}
	}

	return text.fromStdString(parent.toString());
}

QString FepServerThread::outputFepFault( const FepData::FaultPacket& packet )
{
	QString text;
	return text;
}

QString FepServerThread::outputFepEvent( const FepData::EventPacket& packet )
{
	QString text;
	return text;
}

QString FepServerThread::outputFepWave( const FepData::WavePacket& packet )
{
	QString text;
	return text;
}
