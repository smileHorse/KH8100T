
#include "IceHelper.h"

CIceHelper::CIceHelper()
{
	
}

void CIceHelper::setCommunicatorPtr( Ice::CommunicatorPtr ptr )
{
	m_communicatorPtr = ptr;
}

void CIceHelper::setProxyPoints(const string& points)
{
	m_proxyPoints = points;
}

bool CIceHelper::selectCompleteData(const string& tableName, 
	const string& fieldName, const string& fieldValue, StringsVct& dataValues)
{
	dataValues.clear();

	RdbRealData::RdbDataOptPrx rdbOptPrx = getRdbDataOptPrx();
	if (!rdbOptPrx) {
		return false;
	}

	RdbRealData::RequestCompleteDataSeq requestSeq;
	requestSeq.id = 0;
	requestSeq.requestId = 0;
	requestSeq.requestNode = "workstation";
	requestSeq.isStop = false;
	requestSeq.refreshFreq = 0;
	RdbRealData::RequestCompleteData request;
	request.tableName = tableName;
	request.fieldName = fieldName;
	request.fieldValue = fieldValue;
	requestSeq.seq.push_back(request);
	requestSeq.dataCount = requestSeq.seq.size();

	RdbRealData::RespondCompleteDataSeq respondSeq;
	if (!rdbOptPrx->SelectCompleteData(requestSeq, respondSeq))
	{
		return false;
	}

	size_t count = respondSeq.seq.size();
	dataValues.reserve(count);
	for (size_t i = 0; i < count; ++i)
	{
		dataValues.push_back(respondSeq.seq[i].dataValues);
	}
	return true;
}

bool CIceHelper::selectCompleteData(const string& tableName, const string& fieldName, 
	const string& fieldValue, RdbDataItemVct& items)
{
	items.clear();

	StringsVct dataValues;
	bool result = selectCompleteData(tableName, fieldName, fieldValue, dataValues);
	if (!result)
	{
		return false;
	}

	items.reserve(dataValues.size());
	for (StringsVct::iterator iter = dataValues.begin(); iter != dataValues.end(); ++iter)
	{
		if (iter->size() >= 2)
		{
			RdbDataItem item;
			item.mrid = (*iter)[0];
			item.name = (*iter)[1];
			items.push_back(item);
		}
	}
	return true;
}

// 查询所有的单元
void CIceHelper::selectRemoteUnits(const string& fieldName, const string& fieldValue, 
	RemoteUnitItemVct& items)
{
	items.clear();

	StringsVct dataValues;
	bool result = selectCompleteData(REMOTEUNIT, fieldName, fieldValue, dataValues);
	if (!result)
	{
		return;
	}

	items.reserve(dataValues.size());
	for (StringsVct::iterator iter = dataValues.begin(); iter != dataValues.end(); ++iter)
	{
		if (iter->size() >= 13)
		{
			RemoteUnitItem item;
			item.mrid = (*iter)[0];
			item.iedId = (*iter)[1];
			item.iedName = (*iter)[2];
			item.ecRid = (*iter)[13];
			items.push_back(item);
		}
	}
}

// 查询所有的配电所/车站
void CIceHelper::selectSubstations(const string& fieldName, const string& fieldValue, 
	RdbDataItemVct& items)
{
	selectCompleteData(SUBSTATION, fieldName, fieldValue, items);
}

// 查询开关数据
void CIceHelper::selectBreakers(const string& fieldName, const string& fieldValue, 
	RdbDataItemVct& items)
{
	selectCompleteData(BREAKER, fieldName, fieldValue, items);
}

// 查询刀闸数据
void CIceHelper::selectDisconnectors(const string& fieldName, const string& fieldValue, 
	RdbDataItemVct& items)
{
	selectCompleteData(DISCONNECTOR, fieldName, fieldValue, items);
}

// 查询遥测数据
void CIceHelper::selectAnalogs(const string& fieldName, const string& fieldValue, 
	RdbDataItemVct& items)
{
	selectCompleteData(ANALOG, fieldName, fieldValue, items);
}

// 查询遥信数据
void CIceHelper::selectDiscretes(const string& fieldName, const string& fieldValue, 
	RdbDataItemVct& items)
{
	selectCompleteData(DISCRETE, fieldName, fieldValue, items);
}

RdbRealData::RdbDataOptPrx CIceHelper::getRdbDataOptPrx()
{
	if (!m_rdbOptPrx)
	{
		m_rdbOptPrx = RdbRealData::RdbDataOptPrx::checkedCast(
			m_communicatorPtr->stringToProxy("rdb-opt:default " + m_proxyPoints));
	}
	return m_rdbOptPrx;
}
