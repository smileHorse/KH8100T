#include "RdbRealDataI.h"
#include "textelement.h"
#include "workstationserverthread.h"

RdbRealDataRequestI::RdbRealDataRequestI(WorkStationServerThread* threadPtr)
	: m_threadPtr(threadPtr)
{

}

RdbRealDataRequestI::~RdbRealDataRequestI()
{

}

void RdbRealDataRequestI::RequestDefaultData( const ::RdbRealData::RequestDefaultDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */ )
{

}

void RdbRealDataRequestI::RequestSpecificData( const ::RdbRealData::RequestSpecficDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */ )
{

}

void RdbRealDataRequestI::RequestCompleteData( const ::RdbRealData::RequestCompleteDataSeq& seq, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	if (!m_threadPtr)
	{
		return;
	}

	static int count = 0;
	m_threadPtr->outputOperationData(QString("收到请求 %1").arg(++count));


	/*TextElement parent("收到 全部实时数据请求", "");
	parent.insertChild(new TextElement("id", ConvertTypeToString<long>().convertToString(seq.id)));
	parent.insertChild(new TextElement("id", ConvertTypeToString<long>().convertToString(seq.requestId)));
	parent.insertChild(new TextElement("requestNode", seq.requestNode));
	parent.insertChild(new TextElement("isStop", ConvertTypeToString<bool>().convertToString(seq.isStop)));
	parent.insertChild(new TextElement("refreshFreq", ConvertTypeToString<int>().convertToString(seq.refreshFreq)));
	parent.insertChild(new TextElement("dataCount", ConvertTypeToString<long>().convertToString(seq.dataCount)));
	if (!seq.seq.empty())
	{
		for (size_t i = 0; i < seq.seq.size(); ++i)
		{
			RdbRealData::RequestCompleteData data = seq.seq.at(i);
			parent.insertChild(new TextElement("tableName", data.tableName));
			parent.insertChild(new TextElement("fieldName", data.fieldName));
			parent.insertChild(new TextElement("fieldValue", data.fieldValue));
		}
	}

	QString text = QString().fromStdString(parent.toString());
	m_threadPtr->outputOperationData(text);*/
}

void RdbRealDataRequestI::SendTopoDataRequest( const ::RdbRealData::RequestTopoDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	if (!m_threadPtr)
	{
		return;
	}

	static int topo_count = 0;
	m_threadPtr->outputOperationData(QString("收到拓扑数据请求 %1").arg(++topo_count));
}

RdbRealDataRespondI::RdbRealDataRespondI( WorkStationServerThread* threadPtr )
	: m_threadPtr(threadPtr)
{

}

RdbRealDataRespondI::~RdbRealDataRespondI()
{

}

void RdbRealDataRespondI::RespondDefaultData( const ::RdbRealData::RespondDefaultDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */ )
{

}

void RdbRealDataRespondI::RespondSpecificData( const ::RdbRealData::RespondSpecficDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */ )
{

}

void RdbRealDataRespondI::RespondCompleteData( const ::RdbRealData::RespondCompleteDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	if (!m_threadPtr)
	{
		return;
	}

	static int count = 0;
	m_threadPtr->outputOperationData(QString("收到数据响应 %1").arg(++count));
}

void RdbRealDataRespondI::SendTopoDataRespond( const ::RdbRealData::ReceiveTopoDataSeq&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	if (!m_threadPtr)
	{
		return;
	}

	static int topo_count = 0;
	m_threadPtr->outputOperationData(QString("收到拓扑数据响应 %1").arg(++topo_count));
}

