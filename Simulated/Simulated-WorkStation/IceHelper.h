#ifndef ICEHELPER_H
#define ICEHELPER_H

#include "rdbdata.h"

#include <Ice/Ice.h>
#include <string>
#include <vector>
using namespace std;

#define SUBSTATION 		"Substation"
#define BREAKER			"Breaker"
#define DISCONNECTOR	"Disconnector"
#define ANALOG 			"Analog"
#define DISCRETE 		"Discrete"
#define REMOTEUNIT 		"RemoteUnit"

#define MRID	"mRID"
#define NAME	"name"
#define ECTYPE	"ec_type"
#define ECRID	"ec_rid"
#define FTUUNITID	"ftuUnitId"

typedef vector<string> StringVct;
typedef vector< StringVct > StringsVct;

struct RdbDataItem
{
	string mrid;
	string name;
};
typedef vector<RdbDataItem> RdbDataItemVct;

struct RemoteUnitItem
{
	string mrid;
	string iedId;
	string iedName;
	string ecRid;
};
typedef vector<RemoteUnitItem> RemoteUnitItemVct;

class CIceHelper
{
private:
	CIceHelper();

public:
	static CIceHelper* instance()
	{
		static CIceHelper iceHelper;
		return &iceHelper;
	}

public:
	void setCommunicatorPtr(Ice::CommunicatorPtr ptr);
	void setProxyPoints(const string& points);

	bool selectCompleteData(const string& tableName, 
		const string& fieldName, const string& fieldValue, StringsVct& dataValues);

	bool selectCompleteData(const string& tableName, 
		const string& fieldName, const string& fieldValue, RdbDataItemVct& items);

	// 查询所有的单元
	void selectRemoteUnits(const string& fieldName, const string& fieldValue, RemoteUnitItemVct& items);

	// 查询所有的配电所/车站
	void selectSubstations(const string& fieldName, const string& fieldValue, RdbDataItemVct& items);

	// 查询开关数据
	void selectBreakers(const string& fieldName, const string& fieldValue, RdbDataItemVct& items);

	// 查询刀闸数据
	void selectDisconnectors(const string& fieldName, const string& fieldValue, RdbDataItemVct& items);

	// 查询遥测数据
	void selectAnalogs(const string& fieldName, const string& fieldValue, RdbDataItemVct& items);

	// 查询遥信数据
	void selectDiscretes(const string& fieldName, const string& fieldValue, RdbDataItemVct& items);

private:
	RdbRealData::RdbDataOptPrx getRdbDataOptPrx();

private:
	Ice::CommunicatorPtr m_communicatorPtr;
	RdbRealData::RdbDataOptPrx m_rdbOptPrx;

	string m_proxyPoints;
};
#endif