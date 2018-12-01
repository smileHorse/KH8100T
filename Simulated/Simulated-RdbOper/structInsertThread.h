#ifndef STRUCTINSERTTHREAD_H
#define STRUCTINSERTTHREAD_H

#include <QtCore/QtCore>
#include <string>
#include <vector>

#include "rdbdata.h"

using namespace RdbRealData;
using namespace std;

class DataStruct;
typedef QList<DataStruct>	DataStructList;

#define RDB						"rdb"
#define GlobalParam				"GlobalParam"
#define AnalogSuffix			"AnalogSuffix"
#define DiscreteSuffix			"DiscreteSuffix"
#define CommandSuffix			"CommandSuffix"
#define AccumulatorSuffix		"AccumulatorSuffix"
#define RemoteUnitParam			"RemoteUnitParam"
#define VALUE					"value"
#define StartIedId				"startIedId"
#define DevManufacturer			"DevManufacturer"
#define DevDeviceType			"DevDeviceType"
#define GeographicalRegion		"GeographicalRegion"
#define SubGeographicalRegion	"SubGeographicalRegion"
#define Area					"Area"
#define Substation				"Substation"
#define RemoteUnit				"RemoteUnit"
#define AnalogUnitPoint			"AnalogUnitPoint"
#define DiscreteUnitPoint		"DiscreteUnitPoint"
#define ControlUnitPoint		"ControlUnitPoint"
#define AccumulatorUnitPoint	"AccumulatorUnitPoint"
#define BusbarSection			"BusbarSection"
#define Line					"Line"
#define Breaker					"Breaker"
#define Disconnector			"Disconnector"
#define PowerTransformer		"PowerTransformer"
#define TransformerWinding		"TransformerWinding"
#define Analog					"Analog"
#define Discrete				"Discrete"
#define Command					"Command"
#define Accumulator				"Accumulator"
#define FepPartition			"FepPartition"
#define FepSection				"FepSection"
#define FepProtocol				"FepProtocol"
#define FepChannel				"FepChannel"
#define FepProtocolUnit			"FepProtocolUnit"

class DataStruct
{
public:
	DataStruct();

	// 获取指定的孩子信息
	DataStructList	getSpecificChildrens(QString name) const;

	// 获取指定的属性信息
	QString	getSepecificAttribute(QString key) const;

	// 修改属性信息
	void modifyAttribute(QString key, QString value);

	// 获取字段值
	void getFieldValues(vector<string>& fieldValues) const;

	// 添加孩子
	void appendChildren(const DataStruct& children);

public:
	QString	name;
	QString	value;
	QMap<QString, QString>	attributes;
	DataStructList	childrens;
};

class StructInsertThread : public QThread
{
	Q_OBJECT

public:
	StructInsertThread(const RdbDataOptPrx& rdbDataOptPrx, QObject* parent = 0);

protected:
	virtual void run();

private:
	bool loadRdbDataStruct();

	bool insertDevManufacturer();

	bool insertDeviceType(const DataStruct& devManufacturer);

	bool insertGeographicalRegion();

	bool insertSubGeographicalRegion(const DataStruct& parent);

	bool insertArea(const DataStruct& parent);

	bool insertSubstation(const DataStruct& parent);

	bool insertRemoteUnit(const DataStruct& parent);

	bool insertBusbarSection(const DataStruct& parent);

	bool insertLine(const DataStruct& parent);

	bool insertBreaker(const DataStruct& parent);

	bool insertDisconnector(const DataStruct& parent);

	bool insertPowerTransformer(const DataStruct& parent);

	bool insertTransformerWinding(const DataStruct& parent);

	bool insertAnalogUnitPoint(const DataStruct& parent);

	bool insertDiscreteUnitPoint(const DataStruct& parent);

	bool insertControlUnitPoint(const DataStruct& parent);

	bool insertAccumulatorUnitPoint(const DataStruct& parent);

	bool insertAnalog(const DataStruct& parent);

	bool insertDiscrete(const DataStruct& parent);

	bool insertCommand(const DataStruct& parent);

	bool insertAccumulator(const DataStruct& parent);

	bool insertFepPartition();

	bool insertFepSection(const DataStruct& parent);

	bool insertFepProtocol(const DataStruct& parent);

	bool insertFepChannel(const DataStruct& parent);

	bool insertFepProtocolUnit(const DataStruct& parent);

	bool insertSelf(DataStruct& dataStruct);

	void insertChildren(const DataStruct& parent);

	bool insertRdbData(const QString& tableName, const vector<string>& fieldValues);

	void getRandomDeviceTypeInfo(QString& manufacturer, QString& deviceType);

	QStringList getAnalogSuffix();

	QStringList getDiscreteSuffix();

	QStringList getCommandSuffix();

	QStringList getAccumulatorSuffix();

	int getStartIedId();

	int getDataStructCount(const DataStruct& dataStruct);

	QString createName(const QString& parentName, const QString& childName, int index);
	QString createName(const QString& childName, int index);
	QString	createName(const QString& parentName, const QString& childName);

	QString createPathName(const QString& parentPath, const QString& childName);

	void createNewUnitId();
	QString getUnitId();
	QString getYcPointId();
	QString getYxPointId();
	QString getYkPointId();
	QString getDdPointId();

	void createNewProtocolNo();
	QString getProtocolNo();

	void createNewChannelId();
	QString getChannelId();

private:
	DataStruct	rdbDataStruct;

	DataStructList	deviceTypeDataStructs;

	RdbDataOptPrx	m_rdbDataOptPrx;

	int	unitId;
	int ycPointId;
	int yxPointId;
	int ykPointId;
	int ddPointId;

	int protocolNo;
	int channelId;

	RespondCompleteDataSeq insertDataList;
};
#endif