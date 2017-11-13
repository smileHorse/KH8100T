#include "RdbAlarmDataI.h"
#include "textelement.h"
#include "workstationserverthread.h"

#include <QtCore/QtCore>

string	getRdbWarningLevel(RdbWarningData::RdbWarningLevel value)
{
	switch(value)
	{
	case RdbWarningData::Critical:
		return "严重";
	case RdbWarningData::Major:
		return "重要";
	case RdbWarningData::Minor:
		return "中等";
	case RdbWarningData::Info:
		return "一般";
	default:
		return "未知";
	}
}

string	getRdbWarningType(RdbWarningData::RdbWarningType value)
{
	switch(value)
	{
	case RdbWarningData::LimitEvent:
		return "遥测越限";
	case RdbWarningData::SoeEvent:
		return "SOE事项";
	case RdbWarningData::YxEvent:
		return "遥信变位";
	case RdbWarningData::UnitEvent:
		return "终端状态";
	case RdbWarningData::SysEvent:
		return "系统事项";
	case RdbWarningData::OperEvent:
		return "操作事项";
	case RdbWarningData::RelayEvent:
		return "保护事项";
	case RdbWarningData::FaultEvent:
		return "故障事项";
	case RdbWarningData::FaEvent:
		return "FA告警";
	case RdbWarningData::WaveEvent:
		return "录波事项";
	case RdbWarningData::GraphEvent:
		return "画面事项";
	default:
		return "未知";
	}
}

RdbAlarmDataI::RdbAlarmDataI( WorkStationServerThread* threadPtr )
	: m_threadPtr(threadPtr)
{

}

void RdbAlarmDataI::SendAlarmData( const ::RdbWarningData::WarningInfoSeq& seq, const ::Ice::Current& /* = ::Ice::Current */ )
{
	if (!m_threadPtr)
	{
		return;
	}
	if (!seq.empty())
	{
		for (size_t i = 0; i < seq.size(); ++i)
		{
			RdbWarningData::WarningInfo info = seq.at(i);
			TextElement parent("收到 告警数据", ConvertTypeToString<size_t>().convertToString(seq.size()));
			parent.insertChild(new TextElement("标识号", info.id));
			parent.insertChild(new TextElement("告警类型", getRdbWarningType(info.warnType)));
			IceUtil::Time time = IceUtil::Time::milliSeconds(info.timeStamp);
			parent.insertChild(new TextElement("告警时间", time.toString("%Y-%m-%d %H:%M:%S")));
			parent.insertChild(new TextElement("告警等级", getRdbWarningLevel(info.warningLevel)));
			parent.insertChild(new TextElement("告警等级", getRdbWarningLevel(info.warningLevel)));
			parent.insertChild(new TextElement("告警源", info.warnSource));
			parent.insertChild(new TextElement("告警内容数量", ConvertTypeToString<int>().convertToString(info.columnNum)));

			string content;
			for (size_t j = 0; j < info.contents.size(); ++j)
			{
				content += info.contents.at(j) + "\t";
			}
			parent.insertChild(new TextElement("告警内容", content));

			QString text = QString().fromStdString(parent.toString());
			m_threadPtr->outputOperationData(text);
		}
	}
}
