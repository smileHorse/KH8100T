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

void FepServerThread::processYxData()
{
	ProcessDataDialog dialog(AllDataType, Discrete);
	connect(&dialog, SIGNAL(start(SelfDataPacket)), this, SLOT(processDataPacket(SelfDataPacket)));
	dialog.exec();
}

void FepServerThread::processYcData()
{
	ProcessDataDialog dialog(AllDataType, Analog);
	connect(&dialog, SIGNAL(start(SelfDataPacket)), this, SLOT(processDataPacket(SelfDataPacket)));
	dialog.exec();
}

void FepServerThread::processDdData()
{
	ProcessDataDialog dialog(AllDataType, Accumulator);
	connect(&dialog, SIGNAL(start(SelfDataPacket)), this, SLOT(processDataPacket(SelfDataPacket)));
	dialog.exec();
}

void FepServerThread::processDataPacket( SelfDataPacket selfPacket )
{
	// 获取发布者对象
	if (!getFepDataPublisher())
	{
		return;
	}

	// 发布数据
	FepData::DataPacket packet;
	packet.id = selfPacket.id;
	packet.fepNode = selfPacket.fepNode.toStdString();
	packet.type = ::FepData::DataType((int)selfPacket.type);
	packet.unitNo = selfPacket.unitNo;
	packet.analogs.assign(selfPacket.analogs.begin(), selfPacket.analogs.end());
	packet.discretes.assign(selfPacket.discretes.begin(), selfPacket.discretes.end());
	packet.accmulators.assign(selfPacket.accmulators.begin(), selfPacket.accmulators.end());
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

	// 输出发送的数据
	QString text = outputFepFault(packet);
	emit publishFepData(text);

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

	// 输出发送的数据
	QString text = outputFepFault(packet);
	emit publishFepData(text);

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

	// 输出发送的数据
	QString text = outputFepEvent(packet);
	emit publishFepData(text);

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

	// 输出发送的数据
	QString text = outputFepEvent(packet);
	emit publishFepData(text);

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

	// 输出发送的数据
	QString text = outputFepEvent(packet);
	emit publishFepData(text);

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

	// 输出发送的数据
	QString text = outputFepEvent(packet);
	emit publishFepData(text);

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
	parent.insertChild(new TextElement("数据包ID", transferToString<int>(packet.id)));
	parent.insertChild(new TextElement("所属前置机", packet.fepNode));
	parent.insertChild(new TextElement("数据类型", getDataType(packet.type)));
	parent.insertChild(new TextElement("单元号", transferToString<short>(packet.unitNo)));
	if (!packet.units.empty())
	{
		TextElement* units = new TextElement("终端状态", transferToString<size_t>(packet.units.size()), &parent);
		for (size_t i = 0; i < packet.units.size(); ++i)
		{
			units->insertChild(new TextElement("终端编号", transferToString<short>(packet.units.at(i).unitNo)));
			units->insertChild(new TextElement("单元状态", getState(packet.units.at(i).unitState)));
			units->insertChild(new TextElement("主通道状态", getState(packet.units.at(i).channelState1)));
			units->insertChild(new TextElement("副通道状态", getState(packet.units.at(i).channelState2)));
			units->insertChild(new TextElement("误码率", transferToString<unsigned char>(packet.units.at(i).unitNo)));
		}
	}
	if (!packet.analogs.empty())
	{
		TextElement* analogs = new TextElement("遥测数据", transferToString<size_t>(packet.analogs.size()), &parent);

		string value;
		for (size_t i = 0; i < packet.analogs.size(); ++i)
		{
			value += transferToString<int>(packet.analogs.at(i)) + "\t";
		}
		analogs->insertChild(new TextElement("值", value));
	}
	if (!packet.discretes.empty())
	{
		TextElement* discretes = new TextElement("遥信数据", transferToString<size_t>(packet.discretes.size()), &parent);

		string value;
		for (size_t i = 0; i < packet.discretes.size(); ++i)
		{
			value += transferToString<int>(packet.discretes.at(i)) + "\t";
		}
		discretes->insertChild(new TextElement("值", value));
	}
	if (!packet.accmulators.empty())
	{
		TextElement* accmulators = new TextElement("电度数据", transferToString<size_t>(packet.accmulators.size()), &parent);

		string value;
		for (size_t i = 0; i < packet.accmulators.size(); ++i)
		{
			value += transferToString<int>(packet.accmulators.at(i)) + "\t";
		}
		accmulators->insertChild(new TextElement("值", value));
	}
	if (!packet.changedAnalogs.empty())
	{
		TextElement* changedAnalogs = new TextElement("变化遥测数据", transferToString<size_t>(packet.changedAnalogs.size()), &parent);

		for (size_t i = 0; i < packet.changedAnalogs.size(); ++i)
		{
			changedAnalogs->insertChild(new TextElement("时标", transferToString<long>(packet.changedAnalogs.at(i).timeStamp)));
			changedAnalogs->insertChild(new TextElement("终端编号", transferToString<short>(packet.changedAnalogs.at(i).unitNo)));
			changedAnalogs->insertChild(new TextElement("点号", transferToString<short>(packet.changedAnalogs.at(i).index)));
			changedAnalogs->insertChild(new TextElement("值", transferToString<int>(packet.changedAnalogs.at(i).value)));
		}
	}

	return text.fromStdString(parent.toString());
}

QString FepServerThread::outputFepFault( const FepData::FaultPacket& packet )
{
	QString text;
	QString currTime = QDateTime::currentDateTime().toString(Qt::SystemLocaleLongDate);

	TextElement parent("前置机发送故障事项", currTime.toStdString());
	parent.insertChild(new TextElement("数据包ID", transferToString<int>(packet.id)));
	parent.insertChild(new TextElement("所属前置机", packet.fepNode));
	if (!packet.events.empty())
	{
		TextElement* events = new TextElement("故障事项", transferToString<size_t>(packet.events.size()), &parent);
		for (size_t i = 0; i < packet.events.size(); ++i)
		{
			events->insertChild(new TextElement("单元编号", transferToString<short>(packet.events.at(i).unitNo)));
			events->insertChild(new TextElement("时标", transferToString<long>(packet.events.at(i).timeStamp)));
			events->insertChild(new TextElement("故障源", transferToString<short>(packet.events.at(i).source)));
			events->insertChild(new TextElement("事项类型", transferToString<short>(packet.events.at(i).eventType)));
			events->insertChild(new TextElement("方向系数标志", transferToString<short>(packet.events.at(i).directionFlag)));
			events->insertChild(new TextElement("故障类型", transferToString<short>(packet.events.at(i).faultType)));
			events->insertChild(new TextElement("故障线路号", transferToString<short>(packet.events.at(i).lineNo)));
			events->insertChild(new TextElement("持续时间", transferToString<short>(packet.events.at(i).duration)));
			events->insertChild(new TextElement("故障方向", transferToString<short>(packet.events.at(i).direction)));
			events->insertChild(new TextElement("故障距离", transferToString<short>(packet.events.at(i).distance)));
			events->insertChild(new TextElement("方向系数", transferToString<short>(packet.events.at(i).directionMultiplier)));
			events->insertChild(new TextElement("可信度", transferToString<short>(packet.events.at(i).credibility)));
			events->insertChild(new TextElement("零序电压幅值", transferToString<short>(packet.events.at(i).v0Amp)));
			events->insertChild(new TextElement("波形类型", transferToString<short>(packet.events.at(i).waveType)));
			if (!packet.events.at(i).values.empty())
			{
				TextElement* values = new TextElement("故障值序列", 
					transferToString<size_t>(packet.events.at(i).values.size()), events);
				string value;
				for (size_t j = 0; j < packet.events.at(i).values.size(); ++j)
				{
					value += transferToString<short>(packet.events.at(i).values.at(j)) + "\t";
				}
				values->insertChild(new TextElement("值", value));
			}
		}
	}

	return text.fromStdString(parent.toString());
}

QString FepServerThread::outputFepEvent( const FepData::EventPacket& packet )
{
	QString text;
	QString currTime = QDateTime::currentDateTime().toString(Qt::SystemLocaleLongDate);

	TextElement parent("前置机事项数据", currTime.toStdString());
	parent.insertChild(new TextElement("数据包ID", transferToString<int>(packet.id)));
	parent.insertChild(new TextElement("所属前置机", packet.fepNode));
	parent.insertChild(new TextElement("告警类型", getEventType(packet.type)));
	if (!packet.digitals.empty())
	{
		TextElement* digitals = new TextElement("遥信变位", transferToString<size_t>(packet.digitals.size()), &parent);
		for (size_t i = 0; i < packet.digitals.size(); ++i)
		{
			digitals->insertChild(new TextElement("终端编号", transferToString<short>(packet.digitals.at(i).unitNo)));
			digitals->insertChild(new TextElement("遥信点号", transferToString<short>(packet.digitals.at(i).index)));
			digitals->insertChild(new TextElement("遥信值", getDiscreteValue(packet.digitals.at(i).value)));
			digitals->insertChild(new TextElement("时标", transferToString<long>(packet.digitals.at(i).timeStamp)));
		}
	}
	if (!packet.soes.empty())
	{
		TextElement* soes = new TextElement("SOE", transferToString<size_t>(packet.soes.size()), &parent);
		for (size_t i = 0; i < packet.soes.size(); ++i)
		{
			soes->insertChild(new TextElement("终端编号", transferToString<short>(packet.soes.at(i).unitNo)));
			soes->insertChild(new TextElement("遥信点号", transferToString<short>(packet.soes.at(i).index)));
			soes->insertChild(new TextElement("遥信值", getDiscreteValue(packet.soes.at(i).value)));
			soes->insertChild(new TextElement("时标", transferToString<long>(packet.soes.at(i).timeStamp)));
		}
	}
	if (!packet.protects.empty())
	{
		TextElement* protects = new TextElement("保护事项", transferToString<size_t>(packet.protects.size()), &parent);
		for (size_t i = 0; i < packet.protects.size(); ++i)
		{
			protects->insertChild(new TextElement("终端编号", transferToString<short>(packet.protects.at(i).unitNo)));
			protects->insertChild(new TextElement("类型", getProEventType(packet.protects.at(i).Type)));
			protects->insertChild(new TextElement("时间", transferToString<long>(packet.protects.at(i).timeStamp)));
			protects->insertChild(new TextElement("模块编号", transferToString<short>(packet.protects.at(i).moduleNo)));
			protects->insertChild(new TextElement("模块类型", transferToString<short>(packet.protects.at(i).moduleType)));
			protects->insertChild(new TextElement("信息序号", transferToString<short>(packet.protects.at(i).infoNo)));
			protects->insertChild(new TextElement("保护动作(1动作 2复归)", transferToString<short>(packet.protects.at(i).state)));
			if (!packet.protects.at(i).values.empty())
			{
				TextElement* values = new TextElement("保护值", transferToString<size_t>(packet.protects.at(i).values.size()), protects);
				for (size_t j = 0; j < packet.protects.at(i).values.size(); ++j)
				{
					values->insertChild(new TextElement("索引", transferToString<short>(packet.protects.at(i).values.at(j).index)));
					values->insertChild(new TextElement("值", transferToString<short>(packet.protects.at(i).values.at(j).value)));
				}
			}
		}
	}
	if (!packet.units.empty())
	{
		TextElement* units = new TextElement("变化的终端序列", transferToString<size_t>(packet.units.size()), &parent);
		for (size_t i = 0; i < packet.units.size(); ++i)
		{
			units->insertChild(new TextElement("终端编号", transferToString<short>(packet.units.at(i).unitNo)));
			units->insertChild(new TextElement("终端状态", getState(packet.units.at(i).unitState)));
			units->insertChild(new TextElement("主通道状态", getState(packet.units.at(i).channelState1)));
			units->insertChild(new TextElement("副通道状态", getState(packet.units.at(i).channelState2)));
			units->insertChild(new TextElement("误码率", transferToString<int>(packet.units.at(i).errorRate)));
			units->insertChild(new TextElement("时标", transferToString<long>(packet.units.at(i).timeStamp)));
		}
	}

	return text.fromStdString(parent.toString());
}

QString FepServerThread::outputFepWave( const FepData::WavePacket& packet )
{
	QString text;
	QString currTime = QDateTime::currentDateTime().toString(Qt::SystemLocaleLongDate);

	TextElement parent("前置机发送录波事项", currTime.toStdString());
	parent.insertChild(new TextElement("数据包ID", transferToString<int>(packet.id)));
	parent.insertChild(new TextElement("所属前置机", packet.fepNode));
	if (!packet.events.empty())
	{
		TextElement* events = new TextElement("录波事项", transferToString<size_t>(packet.events.size()), &parent);
		for (size_t i = 0; i < packet.events.size(); ++i)
		{
			events->insertChild(new TextElement("单元编号", transferToString<short>(packet.events.at(i).unitNo)));
			events->insertChild(new TextElement("时标", transferToString<long>(packet.events.at(i).timeStamp)));
			events->insertChild(new TextElement("故障源", transferToString<short>(packet.events.at(i).source)));
			events->insertChild(new TextElement("事项类型", transferToString<short>(packet.events.at(i).eventType)));
			events->insertChild(new TextElement("方向系数标志", transferToString<short>(packet.events.at(i).directionFlag)));
			events->insertChild(new TextElement("故障类型", transferToString<short>(packet.events.at(i).faultType)));
			events->insertChild(new TextElement("故障线路号", transferToString<short>(packet.events.at(i).lineNo)));
			events->insertChild(new TextElement("持续时间", transferToString<short>(packet.events.at(i).duration)));
			events->insertChild(new TextElement("故障方向", transferToString<short>(packet.events.at(i).direction)));
			events->insertChild(new TextElement("故障距离", transferToString<short>(packet.events.at(i).distance)));
			events->insertChild(new TextElement("方向系数", transferToString<short>(packet.events.at(i).directionMultiplier)));
			events->insertChild(new TextElement("可信度", transferToString<short>(packet.events.at(i).credibility)));
			events->insertChild(new TextElement("零序电压幅值", transferToString<short>(packet.events.at(i).v0Amp)));
			events->insertChild(new TextElement("波形类型", transferToString<short>(packet.events.at(i).waveType)));
			if (!packet.events.at(i).values.empty())
			{
				TextElement* values = new TextElement("录波值序列", 
					transferToString<size_t>(packet.events.at(i).values.size()), events);
				string value;
				for (size_t j = 0; j < packet.events.at(i).values.size(); ++j)
				{
					value += transferToString<short>(packet.events.at(i).values.at(j)) + "\t";
				}
				values->insertChild(new TextElement("值", value));
			}
		}
	}

	return text.fromStdString(parent.toString());
}
