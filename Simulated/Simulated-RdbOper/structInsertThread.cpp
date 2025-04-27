
#include "dataStructXmlReader.h"
#include "logger.h"
#include "rdbTableDefine.h"
#include "rdbTableFactory.h"
#include "structInsertThread.h"

#include <IceUtil/IceUtil.h>

#define MRID				"mRID"
#define NAME				"name"
#define PATHNAME			"pathName"
#define COUNT				"count"
#define MANUFACTURER		"manufacturer"
#define REGION				"region"
#define EC_TYPE				"ec_type"
#define EC_RID				"ec_rid"
#define LINE_NO				"lineNo"
#define LINE_TYPE			"lineType"
#define IEDID				"IEDID"
#define IEDNAME				"IEDName"
#define DEVICETYPE			"deviceType"
#define YCINDEX				"YcIndex"
#define YCNAME				"YcName"
#define YXINDEX				"YxIndex"
#define YXNAME				"YxName"
#define COMMANDID			"CommandID"
#define COMMANDNAME			"CommandName"
#define DDINDEX				"ddIndex"
#define DDNAME				"ddName"
#define POWERTRANSFORMER	"power_tranformer"
#define PSR_TYPE			"psr_type"
#define PSR_RID				"psr_rid"
#define FTUUNITID			"ftuUnitId"
#define FTUPOINTID			"ftuPointId"
#define REPLYTIMEOUTS		"replyTimeouts"
#define FAULTJUDGECOUNT		"faultJudgeCount"
#define MANUGETRFW			"manuGetRFW"
#define STATNO				"statNo"
#define HOLDFALG			"holdFlag"
#define HOLDFAFLAG			"holdFaFlag"
#define FACTIME				"facTime"
#define RUNTIME				"runTime"
#define UNITTYPE			"unitType"
#define PARTITION			"partition"
#define SECTION				"section"
#define PROTOCOL			"protocol"
#define PROTOCOLNO			"protocolNo"
#define CHANNELID			"channelId"
#define PROTOCOLID			"protocolId"
#define DEVICEID			"deviceId"
#define YX_NUM				"yxNum"
#define YC_NUM				"ycNum"
#define INSERT				"insert"


DataStruct::DataStruct()
{
	attributes.clear();
	childrens.clear();
}

// 获取指定的孩子信息
DataStructList DataStruct::getSpecificChildrens( QString name ) const
{
	DataStructList list;
	foreach(DataStruct dataStruct, childrens)
	{
		if (dataStruct.name == name)
		{
			list.append(dataStruct);
		}
	}
	return list;
}

// 获取指定的属性信息
QString DataStruct::getSepecificAttribute( QString key ) const
{
	if (attributes.contains(key))
	{
		return attributes.value(key);
	}
	else
	{
		return "";
	}
}

// 修改属性信息
void DataStruct::modifyAttribute( QString key, QString value )
{
	if (attributes.contains(key))
	{
		attributes.remove(key);
	}
	attributes.insert(key, value);
}

// 获取字段值
void DataStruct::getFieldValues( vector<string>& fieldValues ) const
{
	// 根据表名获取表的字段信息
	RdbFieldStructList fields;
	RdbTableFactory::getTableFields(name, fields);

	// 为字段赋值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;

		if (attributes.contains(fieldName))
		{
			fieldValues.push_back(attributes.value(fieldName).toStdString());
		}
		else
		{
			if (fieldType == "string")
			{
				if (fieldName == "mRID")
				{
					fieldValues.push_back(IceUtil::generateUUID());
				}
				else
				{
					fieldValues.push_back("");
				}
			}
			else if (fieldType == "int")
			{
				fieldValues.push_back("0");
			}
			else if (fieldType == "double")
			{
				fieldValues.push_back("0");
			}
			else if (fieldType == "bool")
			{
				fieldValues.push_back("0");
			}
			else
			{
				if (fieldName == "psrType")
				{
					fieldValues.push_back(fieldType.toStdString());
				}
				else
				{
					fieldValues.push_back("");
				}
			}
		}		
	}
}

// 添加孩子
void DataStruct::appendChildren( const DataStruct& children )
{
	childrens.append(children);
}

StructInsertThread::StructInsertThread(const RdbDataOptPrx& rdbDataOptPrx, QObject* parent /*= 0*/ )
	: QThread(parent), m_rdbDataOptPrx(rdbDataOptPrx)
{
	unitId = 0;
	yxPointId = ycPointId = ykPointId = ddPointId = 0;

	protocolNo = 0;
	channelId = 0;

	insertDataList.id = 1;
	insertDataList.requestId = 1;
	insertDataList.requestNode = "structInsert";
}

void StructInsertThread::run()
{
	// 加载数据结构
	loadRdbDataStruct();

	// 插入设备厂家
	insertDevManufacturer();

	// 插入区域
	insertGeographicalRegion();

	// 插入分区
	insertFepPartition();

	insertRdbData("", vector<std::string>());
}

bool StructInsertThread::loadRdbDataStruct()
{
	DataStructXmlReader xmlReader;
	xmlReader.loadDataStruct(rdbDataStruct);

	return true;
}

bool StructInsertThread::insertDevManufacturer()
{
	// 获取设备厂家信息
	DataStructList dataStructs = rdbDataStruct.getSpecificChildrens(DevManufacturer);
	foreach(DataStruct dataStruct, dataStructs)
	{
		insertSelf(dataStruct);
	}
	return true;
}

bool StructInsertThread::insertDeviceType( const DataStruct& parent )
{
	// 获取设备类型信息
	QString parentMrid = parent.getSepecificAttribute(MRID);
	QString parentName = parent.getSepecificAttribute(NAME);
	DataStructList dataStructs = parent.getSpecificChildrens(DevDeviceType);
	foreach(DataStruct temp, dataStructs)
	{
		int count = getDataStructCount(temp);
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			dataStruct.modifyAttribute(NAME, createName(parentName, name, i + 1));
			dataStruct.modifyAttribute(MANUFACTURER, parentMrid);

			bool result = insertSelf(dataStruct);
			if (result)
			{
				deviceTypeDataStructs.append(dataStruct);
			}
		}
	}

	return true;
}

bool StructInsertThread::insertGeographicalRegion()
{
	// 获取区域信息
	DataStructList dataStructs = rdbDataStruct.getSpecificChildrens(GeographicalRegion);
	foreach(DataStruct dataStruct, dataStructs)
	{
		insertSelf(dataStruct);
	}
	return true;
}

bool StructInsertThread::insertSubGeographicalRegion(const DataStruct& parent)
{
	// 获取信息
	QString parentName = parent.getSepecificAttribute(NAME);
	QString parentMrid = parent.getSepecificAttribute(MRID);
	DataStructList dataStructs = parent.getSpecificChildrens(SubGeographicalRegion);
	foreach(DataStruct temp, dataStructs)
	{
		int count = getDataStructCount(temp);
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = createName(name, i + 1);
			dataStruct.modifyAttribute(NAME, childName);
			QString pathName = createPathName(parentName, childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);
			dataStruct.modifyAttribute(REGION, parentMrid);

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertArea( const DataStruct& parent )
{
	// 获取信息
	QString parentMrid = parent.getSepecificAttribute(MRID);
	QString parentName = parent.getSepecificAttribute(NAME);
	QString parentPath = parent.getSepecificAttribute(PATHNAME);
	DataStructList dataStructs = parent.getSpecificChildrens(Area);
	foreach(DataStruct temp, dataStructs)
	{
		int count = getDataStructCount(temp);
		QString name = temp.getSepecificAttribute(NAME);
		QString ecRid = temp.getSepecificAttribute(EC_RID);
		if (ecRid.isEmpty()) {
			ecRid = parentMrid;
		}
		QString no = temp.getSepecificAttribute("no");
		int suffix = no.isEmpty() ? 0 : no.toInt();
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = createName(parentName, name, suffix + i + 1);
			dataStruct.modifyAttribute(NAME, childName);
			QString pathName = createPathName(parentPath, childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);
			dataStruct.modifyAttribute(EC_TYPE, parent.name);
			dataStruct.modifyAttribute(EC_RID, ecRid);

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertSubstation( const DataStruct& parent )
{
	// 获取信息
	QString parentMrid = parent.getSepecificAttribute(MRID);
	QString parentName = parent.getSepecificAttribute(NAME);
	QString parentPath = parent.getSepecificAttribute(PATHNAME);
	DataStructList dataStructs = parent.getSpecificChildrens(Substation);
	foreach(DataStruct temp, dataStructs)
	{
		int count = getDataStructCount(temp);
		QString name = temp.getSepecificAttribute(NAME);
		QString ecRid = temp.getSepecificAttribute(EC_RID);
		if (ecRid.isEmpty()) {
			ecRid = parentMrid;
		}
		QString no = temp.getSepecificAttribute("no");
		int suffix = no.isEmpty() ? 0 : no.toInt();
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = createName(parentName, name, suffix + i + 1);
			dataStruct.modifyAttribute(NAME, childName);
			QString pathName = createPathName(parentPath, childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);
			dataStruct.modifyAttribute(EC_TYPE, parent.name);
			dataStruct.modifyAttribute(EC_RID, ecRid);

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertRemoteUnit( const DataStruct& parent )
{
	// 获取信息
	QString parentMrid = parent.getSepecificAttribute(MRID);
	QString parentName = parent.getSepecificAttribute(NAME);
	DataStructList dataStructs = parent.getSpecificChildrens(RemoteUnit);
	foreach(DataStruct temp, dataStructs)
	{
		int count = getDataStructCount(temp);
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			createNewUnitId();
			
			QString iedId = getUnitId();
			dataStruct.modifyAttribute(IEDID, iedId);

			QString childName = createName(parentName, name);
			dataStruct.modifyAttribute(IEDNAME, childName);

			QString manufacturer, deviceType;
			manufacturer = dataStruct.getSepecificAttribute(MANUFACTURER);
			deviceType = dataStruct.getSepecificAttribute(DEVICETYPE);
			if (manufacturer.isEmpty() || deviceType.isEmpty()) {
				getRandomDeviceTypeInfo(manufacturer, deviceType);
			}
			dataStruct.modifyAttribute(MANUFACTURER, manufacturer);
			dataStruct.modifyAttribute(DEVICETYPE, deviceType);
			dataStruct.modifyAttribute(EC_TYPE, parent.name);
			dataStruct.modifyAttribute(EC_RID, parentMrid);
			dataStruct.modifyAttribute(REPLYTIMEOUTS, "3000");
			dataStruct.modifyAttribute(FAULTJUDGECOUNT, "5");
			dataStruct.modifyAttribute(STATNO, "1");
			dataStruct.modifyAttribute(MANUGETRFW, "1");
			dataStruct.modifyAttribute(HOLDFALG, "1");
			dataStruct.modifyAttribute(HOLDFAFLAG, "1");
			dataStruct.modifyAttribute(FACTIME, "2024-01-01");
			dataStruct.modifyAttribute(RUNTIME, "2024-01-01");

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertBusbarSection( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(BusbarSection);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1-%2").arg(name).arg(i + 1);
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(EC_TYPE, parent.name);
			dataStruct.modifyAttribute(EC_RID, parent.getSepecificAttribute(MRID));

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertLine( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Line);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1-%2").arg(name).arg(i + 1);
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(EC_TYPE, parent.name);
			dataStruct.modifyAttribute(EC_RID, parent.getSepecificAttribute(MRID));
			dataStruct.modifyAttribute(LINE_TYPE, QString("%1").arg(30));
			dataStruct.modifyAttribute(LINE_NO, QString("%1").arg(i + 1));

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertBreaker( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Breaker);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1-%2").arg(name).arg(i + 1);
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(EC_TYPE, parent.name);
			dataStruct.modifyAttribute(EC_RID, parent.getSepecificAttribute(MRID));
			QString iedId = getUnitId();
			dataStruct.modifyAttribute("FTUNo", iedId);
			dataStruct.modifyAttribute("EventID", QString("%1").arg((i % 8) + 1));
			dataStruct.modifyAttribute("switchTranMode", "1");
			dataStruct.modifyAttribute("switchCommandType", "1");
			dataStruct.modifyAttribute("base_voltage", "7F24B357-41D0-4069-ADA1-24AA26525A29");

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertDisconnector( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Disconnector);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1-%2").arg(name).arg(i + 1);
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(EC_TYPE, parent.name);
			dataStruct.modifyAttribute(EC_RID, parent.getSepecificAttribute(MRID));
			dataStruct.modifyAttribute("switchTranMode", "1");
			dataStruct.modifyAttribute("switchCommandType", "1");
			dataStruct.modifyAttribute("base_voltage", "7F24B357-41D0-4069-ADA1-24AA26525A29");

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertPowerTransformer( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(PowerTransformer);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1-%2").arg(name).arg(i + 1);
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(EC_TYPE, parent.name);
			dataStruct.modifyAttribute(EC_RID, parent.getSepecificAttribute(MRID));

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertTransformerWinding( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(TransformerWinding);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString name = temp.getSepecificAttribute(NAME);
		QString ecType = parent.getSepecificAttribute(EC_TYPE);
		QString ecRid = parent.getSepecificAttribute(EC_RID);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1-%2").arg(name).arg(i + 1);
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);
			
			dataStruct.modifyAttribute(EC_TYPE, ecType);
			dataStruct.modifyAttribute(EC_RID, ecRid);

			dataStruct.modifyAttribute(POWERTRANSFORMER, parent.getSepecificAttribute(MRID));

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertAnalogUnitPoint( const DataStruct& parent )
{
	// 获取信息
	DataStruct dataStruct;
	dataStruct.name = AnalogUnitPoint;

	QString iedId = parent.getSepecificAttribute(FTUUNITID);
	QString index = parent.getSepecificAttribute(FTUPOINTID);
	QString desc = parent.getSepecificAttribute(NAME);

	dataStruct.modifyAttribute(IEDID, iedId);
	dataStruct.modifyAttribute(YCINDEX, index);
	dataStruct.modifyAttribute(YCNAME, desc);

	insertSelf(dataStruct);

	return true;
}

bool StructInsertThread::insertDiscreteUnitPoint( const DataStruct& parent )
{
	// 获取信息
	DataStruct dataStruct;
	dataStruct.name = DiscreteUnitPoint;

	QString iedId = parent.getSepecificAttribute(FTUUNITID);
	QString index = parent.getSepecificAttribute(FTUPOINTID);
	QString desc = parent.getSepecificAttribute(NAME);

	dataStruct.modifyAttribute(IEDID, iedId);
	dataStruct.modifyAttribute(YXINDEX, index);
	dataStruct.modifyAttribute(YXNAME, desc);

	insertSelf(dataStruct);

	return true;
}

bool StructInsertThread::insertControlUnitPoint( const DataStruct& parent )
{
	// 获取信息
	DataStruct dataStruct;
	dataStruct.name = ControlUnitPoint;

	QString iedId = parent.getSepecificAttribute(FTUUNITID);
	QString index = parent.getSepecificAttribute(FTUPOINTID);
	QString desc = parent.getSepecificAttribute(NAME);

	dataStruct.modifyAttribute(IEDID, iedId);
	dataStruct.modifyAttribute(COMMANDID, index);
	dataStruct.modifyAttribute(COMMANDNAME, desc);

	insertSelf(dataStruct);

	return true;
}

bool StructInsertThread::insertAccumulatorUnitPoint( const DataStruct& parent )
{
	// 获取信息
	DataStruct dataStruct;
	dataStruct.name = AccumulatorUnitPoint;

	QString iedId = parent.getSepecificAttribute(FTUUNITID);
	QString index = parent.getSepecificAttribute(FTUPOINTID);
	QString desc = parent.getSepecificAttribute(NAME);

	dataStruct.modifyAttribute(IEDID, iedId);
	dataStruct.modifyAttribute(DDINDEX, index);
	dataStruct.modifyAttribute(DDNAME, desc);

	insertSelf(dataStruct);

	return true;
}

bool StructInsertThread::insertAnalog( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Analog);
	foreach(DataStruct temp, dataStructs)
	{
		QStringList values = getAnalogSuffix();

		QString name = parent.getSepecificAttribute(NAME);
		QString psrType = parent.name;
		QString psrRid = parent.getSepecificAttribute(MRID);
		for (int i = 0; i < values.size(); ++i)
		{
			QString iedId = getUnitId();
			QString point = getYcPointId();

			DataStruct dataStruct = temp;
			QString childName = QString("%1%2").arg(name).arg(values.at(i));
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(PSR_TYPE, psrType);
			dataStruct.modifyAttribute(PSR_RID, psrRid);
			dataStruct.modifyAttribute(FTUUNITID, iedId);
			dataStruct.modifyAttribute(FTUPOINTID, point);

			insertSelf(dataStruct);

			insertAnalogUnitPoint(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertDiscrete( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Discrete);
	foreach(DataStruct temp, dataStructs)
	{
		QStringList values = getDiscreteSuffix();

		QString name = parent.getSepecificAttribute(NAME);
		QString psrType = parent.name;
		QString psrRid = parent.getSepecificAttribute(MRID);
		for (int i = 0; i < values.size(); ++i)
		{
			QString iedId = getUnitId();
			QString point = getYxPointId();

			DataStruct dataStruct = temp;
			QString childName = QString("%1%2").arg(name).arg(values.at(i));
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(PSR_TYPE, psrType);
			dataStruct.modifyAttribute(PSR_RID, psrRid);
			dataStruct.modifyAttribute(FTUUNITID, iedId);
			dataStruct.modifyAttribute(FTUPOINTID, point);

			insertSelf(dataStruct);
			insertDiscreteUnitPoint(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertCommand( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Command);
	foreach(DataStruct temp, dataStructs)
	{
		QStringList values = getCommandSuffix();

		QString name = parent.getSepecificAttribute(NAME);
		QString psrType = parent.name;
		QString psrRid = parent.getSepecificAttribute(MRID);
		for (int i = 0; i < values.size(); ++i)
		{
			QString iedId = getUnitId();
			QString point = getYkPointId();

			DataStruct dataStruct = temp;
			QString childName = QString("%1%2").arg(name).arg(values.at(i));
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(PSR_TYPE, psrType);
			dataStruct.modifyAttribute(PSR_RID, psrRid);
			dataStruct.modifyAttribute(FTUUNITID, iedId);
			dataStruct.modifyAttribute(FTUPOINTID, point);

			insertSelf(dataStruct);
			insertControlUnitPoint(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertAccumulator( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Accumulator);
	foreach(DataStruct temp, dataStructs)
	{
		QStringList values = getAccumulatorSuffix();

		QString name = parent.getSepecificAttribute(NAME);
		QString psrType = parent.name;
		QString psrRid = parent.getSepecificAttribute(MRID);
		for (int i = 0; i < values.size(); ++i)
		{
			QString iedId = getUnitId();
			QString point = getDdPointId();

			DataStruct dataStruct = temp;
			QString childName = QString("%1%2").arg(name).arg(values.at(i));
			dataStruct.modifyAttribute(NAME, childName);

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(PSR_TYPE, psrType);
			dataStruct.modifyAttribute(PSR_RID, psrRid);
			dataStruct.modifyAttribute(FTUUNITID, iedId);
			dataStruct.modifyAttribute(FTUPOINTID, point);

			insertSelf(dataStruct);
			insertAccumulatorUnitPoint(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertFepPartition()
{
	getChannelIp1();
	getChannelIp2();
	getChannelPort1();
	getChannelPort2();
	DataStructList dataStructs = rdbDataStruct.getSpecificChildrens(FepPartition);
	foreach(DataStruct dataStruct, dataStructs)
	{
		insertSelf(dataStruct);
	}
	return true;
}

bool StructInsertThread::insertFepSection( const DataStruct& parent )
{
	// 获取信息
	QString parentMrid = parent.getSepecificAttribute(MRID);
	DataStructList dataStructs = parent.getSpecificChildrens(FepSection);
	foreach(DataStruct temp, dataStructs)
	{
		int count = getDataStructCount(temp);
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = createName(name, i + 1);
			dataStruct.modifyAttribute(NAME, childName);
			dataStruct.modifyAttribute(PARTITION, parentMrid);

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertFepProtocol( const DataStruct& parent )
{
	// 获取信息
	QString partitionId = parent.getSepecificAttribute(PARTITION);
	QString parentMrid = parent.getSepecificAttribute(MRID);
	QString parentName = parent.getSepecificAttribute(NAME);
	DataStructList dataStructs = parent.getSpecificChildrens(FepProtocol);

	foreach(DataStruct temp, dataStructs)
	{
		QString partition = temp.getSepecificAttribute(PARTITION);
		partition = partition.isEmpty() ? partitionId : partition;
		QString section = temp.getSepecificAttribute(SECTION);
		section = section.isEmpty() ? parentMrid : section; 

		int startProNo = temp.getSepecificAttribute("startNo").toInt();
		int count = getDataStructCount(temp);
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			createNewProtocolNo();

			QString proNo = getProtocolNo();
			proNo = QString("%1").arg(startProNo++);
			dataStruct.modifyAttribute(PROTOCOLNO, proNo);

			QString childName = createName(parentName, name, startProNo - 1);
			dataStruct.modifyAttribute(NAME, childName);
			dataStruct.modifyAttribute(PARTITION, partition);
			dataStruct.modifyAttribute(SECTION, section);

			insertSelf(dataStruct);
			insertFepProtocolUnit(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertFepChannel( const DataStruct& parent )
{
	// 获取信息
	QString parentMrid = parent.getSepecificAttribute(MRID);
	DataStructList dataStructs = parent.getSpecificChildrens(FepChannel);
	QString strProtocolNo = parent.getSepecificAttribute(PROTOCOLNO);
	int protocolNo = strProtocolNo.toInt();
	foreach(DataStruct temp, dataStructs)
	{
		int count = getDataStructCount(temp);
		QString name = temp.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			createNewChannelId();

			QString chanId = getChannelId();
			dataStruct.modifyAttribute(CHANNELID, QString("%1").arg(i));

			QString childName = createName(name, i + 1);
			dataStruct.modifyAttribute(NAME, childName);
			dataStruct.modifyAttribute(PROTOCOL, parentMrid);

			QString param = dataStruct.getSepecificAttribute("param");
			if (protocolNo < 100) {
				param = QString("CommType=TCP;LocalPort=;LocalIP=;RemotePort=%1;RemoteIP=%2;Server=0").arg(channelPort1++).arg(channelIp1);
			} else {
				param = QString("CommType=TCP;LocalPort=;LocalIP=;RemotePort=%1;RemoteIP=%2;Server=0").arg(channelPort2++).arg(channelIp2);
			}
			dataStruct.modifyAttribute("param", param);

			insertSelf(dataStruct);
		}
	}

	return true;
}

bool StructInsertThread::insertFepProtocolUnit( const DataStruct& parent )
{
	// 获取信息
	DataStruct dataStruct;
	dataStruct.name = FepProtocolUnit;

	QString protocolId = parent.getSepecificAttribute(MRID);
	QString deviceId = parent.getSepecificAttribute(PROTOCOLNO);

	dataStruct.modifyAttribute(PROTOCOLID, protocolId);
	dataStruct.modifyAttribute(DEVICEID, deviceId);

	insertSelf(dataStruct);

	return true;
}

bool StructInsertThread::insertSelf( DataStruct& dataStruct )
{
	// 获取字段值
	vector<string> fieldValues;
	dataStruct.getFieldValues(fieldValues);
	dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));

	bool result = true;
	QString insert = dataStruct.getSepecificAttribute(INSERT);
	if (insert != "false") {
		result = insertRdbData(dataStruct.name, fieldValues);
	}
 
	if (result)
	{
		insertChildren(dataStruct);
	}
	return result;
}

void StructInsertThread::insertChildren( const DataStruct& parent )
{
	foreach(DataStruct children, parent.childrens)
	{
		QString childrenName = children.name;
		if (childrenName == SubGeographicalRegion)
		{
			insertSubGeographicalRegion(parent);
		}
		else if (childrenName == Area)
		{
			insertArea(parent);
		}
		else if (childrenName == Substation)
		{
			insertSubstation(parent);
		}
		else if (childrenName == RemoteUnit)
		{
			insertRemoteUnit(parent);
		}
		else if (childrenName == BusbarSection)
		{
			insertBusbarSection(parent);
		}
		else if (childrenName == Line)
		{
			insertLine(parent);
		}
		else if (childrenName == Breaker)
		{
			insertBreaker(parent);
		}
		else if (childrenName == Disconnector)
		{
			insertDisconnector(parent);
		}
		else if (childrenName == PowerTransformer)
		{
			insertPowerTransformer(parent);
		}
		else if (childrenName == TransformerWinding)
		{
			insertTransformerWinding(parent);
		}
		else if (childrenName == Analog)
		{
			insertAnalog(parent);
		}
		else if (childrenName == Discrete)
		{
			insertDiscrete(parent);
		}
		else if (childrenName == Command)
		{
			insertCommand(parent);
		}
		else if (childrenName == Accumulator)
		{
			insertAccumulator(parent);
		}
		else if (childrenName == DevDeviceType)
		{
			insertDeviceType(parent);
		}
		else if (childrenName == FepSection)
		{
			insertFepSection(parent);
		}
		else if (childrenName == FepProtocol)
		{
			insertFepProtocol(parent);
		}
		else if (childrenName == FepChannel)
		{
			insertFepChannel(parent);
		}
	}
}

//bool StructInsertThread::insertRdbData( const QString& tableName, const vector<string>& fieldValues )
//{
//	RespondCompleteDataSeq repSeq;
//	repSeq.id = 1;
//	repSeq.requestId = 1;
//	repSeq.requestNode = "randomInsert";
//
//	RespondCompleteData data;
//	data.tableName = tableName.toStdString();
//	data.dataValues.assign(fieldValues.begin(), fieldValues.end());
//
//	repSeq.seq.push_back(data);
//	repSeq.dataCount = repSeq.seq.size();
//	
//	RespondCompleteDataSequence repSequence;
//	try
//	{
//		RdbDataOptPrx rdbDataOptPrx_timeout = RdbDataOptPrx::uncheckedCast(
//			m_rdbDataOptPrx->ice_timeout(5000));
//		bool result = m_rdbDataOptPrx->InsertData(repSeq, repSequence);
//		if (result)
//		{
//			RdbLog(CLogger::Log_INFO, "插入 %s 成功", tableName.toStdString().c_str());
//			return true;
//		}
//		else
//		{
//			RdbLog(CLogger::Log_INFO, "!!!!!!!!!!!!!! 插入 %s 失败 !!!!!!!!!!!!!!!", tableName.toStdString().c_str());
//			return false;
//		}
//	}
//	catch(const Ice::Exception& ex)
//	{
//		RdbLog(CLogger::Log_ERROR, "插入 %s 数据时出现异常: %s", tableName.toStdString().c_str(), ex.what());
//		return false;
//	}		
//}
bool StructInsertThread::insertRdbData( const QString& tableName, const vector<string>& fieldValues )
{
	if (!tableName.isEmpty()) {
		RespondCompleteData data;
		data.tableName = tableName.toStdString();
		data.dataValues.assign(fieldValues.begin(), fieldValues.end());

		insertDataList.seq.push_back(data);
		insertDataList.dataCount = insertDataList.seq.size();
	}

	if (tableName.isEmpty() || insertDataList.seq.size() > 300)
	{
		RespondCompleteDataSequence repSequence;
		try
		{
			RdbDataOptPrx rdbDataOptPrx_timeout = RdbDataOptPrx::uncheckedCast(
				m_rdbDataOptPrx->ice_timeout(5000));
			bool result = m_rdbDataOptPrx->InsertData(insertDataList, repSequence);
			if (result)
			{
				insertDataList.seq.clear();
				RdbLog(CLogger::Log_INFO, "插入 %s 成功", tableName.toStdString().c_str());
				return true;
			}
			else
			{
				insertDataList.seq.clear();
				RdbLog(CLogger::Log_INFO, "!!!!!!!!!!!!!! 插入 %s 失败 !!!!!!!!!!!!!!!", tableName.toStdString().c_str());
				return false;
			}
		}
		catch(const Ice::Exception& ex)
		{
			insertDataList.seq.clear();
			RdbLog(CLogger::Log_ERROR, "插入 %s 数据时出现异常: %s", tableName.toStdString().c_str(), ex.what());
			return false;
		}		
	}

	return true;
}

void StructInsertThread::getRandomDeviceTypeInfo( QString& manufacturer, QString& deviceType )
{
	if (deviceTypeDataStructs.isEmpty())
	{
		manufacturer = "";
		deviceType = "";
	}
	else
	{
		int index = qrand() % deviceTypeDataStructs.size();
		manufacturer = deviceTypeDataStructs.at(index).getSepecificAttribute(MANUFACTURER);
		deviceType = deviceTypeDataStructs.at(index).getSepecificAttribute(MRID);
	}

}

QStringList StructInsertThread::getAnalogSuffix()
{
	QStringList strs;
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return strs;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(AnalogSuffix);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(VALUE);
		foreach (DataStruct value, values)
		{
			strs.append(value.value);
		}
	}
	return strs;
}

QStringList StructInsertThread::getDiscreteSuffix()
{
	QStringList strs;
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return strs;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(DiscreteSuffix);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(VALUE);
		foreach (DataStruct value, values)
		{
			strs.append(value.value);
		}
	}
	return strs;
}

QStringList StructInsertThread::getCommandSuffix()
{
	QStringList strs;
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return strs;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(CommandSuffix);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(VALUE);
		foreach (DataStruct value, values)
		{
			strs.append(value.value);
		}
	}
	return strs;
}

QStringList StructInsertThread::getAccumulatorSuffix()
{
	QStringList strs;
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return strs;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(AccumulatorSuffix);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(VALUE);
		foreach (DataStruct value, values)
		{
			strs.append(value.value);
		}
	}
	return strs;
}

int StructInsertThread::getStartIedId()
{
	int iedId = 0;
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return iedId;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(RemoteUnitParam);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(StartIedId);
		if (!values.isEmpty())
		{
			iedId = values.at(0).value.toInt();
		}
	}
	return iedId;
}

QString StructInsertThread::getChannelIp1()
{
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return channelIp1;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(FepChannelParam);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(IP1);
		if (!values.isEmpty())
		{
			channelIp1 = values.at(0).value;
		}
	}
	return channelIp1;
}

QString StructInsertThread::getChannelIp2()
{
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return channelIp2;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(FepChannelParam);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(IP2);
		if (!values.isEmpty())
		{
			channelIp2 = values.at(0).value;
		}
	}
	return channelIp2;
}

int StructInsertThread::getChannelPort1()
{
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return channelPort1;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(FepChannelParam);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(PORT1);
		if (!values.isEmpty())
		{
			channelPort1 = values.at(0).value.toInt();
		}
	}
	return channelPort1;
}

int StructInsertThread::getChannelPort2()
{
	DataStructList globalStructs = rdbDataStruct.getSpecificChildrens(GlobalParam);
	if (globalStructs.isEmpty())
	{
		return channelPort2;
	}

	DataStruct globalStruct = globalStructs.at(0);
	DataStructList dataStructs = globalStruct.getSpecificChildrens(FepChannelParam);
	foreach(DataStruct dataStruct, dataStructs)
	{
		DataStructList values = dataStruct.getSpecificChildrens(PORT2);
		if (!values.isEmpty())
		{
			channelPort2 = values.at(0).value.toInt();
		}
	}
	return channelPort2;
}

int StructInsertThread::getDataStructCount(const DataStruct& dataStruct)
{
	int count = dataStruct.getSepecificAttribute(COUNT).toInt();
	return count <=0 ? 0 : count;
}

QString StructInsertThread::createName( const QString& parentName, const QString& childName, int index )
{
	QString name = QString("%1-%2-%3").arg(parentName).arg(childName).arg(index);
	return name;
}

QString StructInsertThread::createName( const QString& childName, int index )
{
	QString name = QString("%1-%2").arg(childName).arg(index);
	return name;
}

QString StructInsertThread::createName( const QString& parentName, const QString& childName )
{
	QString name = QString("%1-%2").arg(parentName).arg(childName);
	return name;
}

QString StructInsertThread::createPathName( const QString& parentPath, const QString& childName )
{
	QString pathName = QString("%1/%2").arg(parentPath).arg(childName);
	return pathName;
}

void StructInsertThread::createNewUnitId()
{
	if (unitId == 0)
	{
		unitId = getStartIedId();
	}
	unitId++;
	yxPointId = ycPointId = ykPointId = ddPointId = 0;
}

QString StructInsertThread::getUnitId()
{
	return QString("%1").arg(unitId);
}

QString StructInsertThread::getYcPointId()
{
	return QString("%1").arg(ycPointId++);
}

QString StructInsertThread::getYxPointId()
{
	return QString("%1").arg(yxPointId++);
}

QString StructInsertThread::getYkPointId()
{
	return QString("%1").arg(ykPointId++);
}

QString StructInsertThread::getDdPointId()
{
	return QString("%1").arg(ddPointId++);
}

void StructInsertThread::createNewProtocolNo()
{
	protocolNo++;
}

QString StructInsertThread::getProtocolNo()
{
	return QString("%1").arg(protocolNo);
}

void StructInsertThread::createNewChannelId()
{
	channelId++;
}

QString StructInsertThread::getChannelId()
{
	return QString("%1").arg(channelId);
}
