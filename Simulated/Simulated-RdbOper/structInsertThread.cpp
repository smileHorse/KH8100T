
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
#define IEDID				"IEDID"
#define IEDNAME				"IEDName"
#define DEVICETYPE			"deviceType"
#define YCINDEX				"YcIndex"
#define YXINDEX				"YxIndex"
#define COMMANDID			"CommandID"
#define DDINDEX				"ddIndex"
#define POWERTRANSFORMER	"power_tranformer"
#define PSR_TYPE			"psr_type"
#define PSR_RID				"psr_rid"
#define FTUUNITID			"ftuUnitId"
#define FTUPOINTID			"ftuPointId"


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
}

void StructInsertThread::run()
{
	// 加载数据结构
	loadRdbDataStruct();

	// 插入设备厂家
	insertDevManufacturer();

	// 插入区域
	insertGeographicalRegion();
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
	DataStructList devManufacturers = rdbDataStruct.getSpecificChildrens(DevManufacturer);
	foreach(DataStruct devManufacturer, devManufacturers)
	{
		// 获取字段值
		vector<string> fieldValues;
		devManufacturer.getFieldValues(fieldValues);
		devManufacturer.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));

		bool result = insertRdbData(devManufacturer.name, fieldValues);
		if (result)
		{
			// 插入设备类型
			insertDeviceType(devManufacturer);
		}
	}
	return true;
}

bool StructInsertThread::insertDeviceType( const DataStruct& devManufacturer )
{
	// 获取设备类型信息
	DataStructList deviceTypes = devManufacturer.getSpecificChildrens(DevDeviceType);
	foreach(DataStruct deviceType, deviceTypes)
	{
		int count = deviceType.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString name = deviceType.getSepecificAttribute(NAME);
		for (int i = 0; i < count; ++i)
		{
			deviceType.modifyAttribute(NAME, QString("%1-%2").arg(name).arg(i + 1));
			deviceType.modifyAttribute(MANUFACTURER, devManufacturer.getSepecificAttribute(MRID));

			// 获取字段值
			vector<string> fieldValues;
			deviceType.getFieldValues(fieldValues);
			deviceType.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(deviceType.name, fieldValues);
			if (result)
			{
				deviceTypeDataStructs.append(deviceType);
			}
		}
	}

	return true;
}

bool StructInsertThread::insertGeographicalRegion()
{
	// 获取区域信息
	DataStructList regions = rdbDataStruct.getSpecificChildrens(GeographicalRegion);
	foreach(DataStruct region, regions)
	{
		// 插入子区域
		insertSubGeographicalRegion(region);
	}
	return true;
}

bool StructInsertThread::insertSubGeographicalRegion(const DataStruct& parent)
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(SubGeographicalRegion);
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
			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(NAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);
			dataStruct.modifyAttribute(REGION, parent.getSepecificAttribute(MRID));

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertArea(dataStruct);
				insertSubstation(dataStruct);
			}
		}
	}

	return true;
}

bool StructInsertThread::insertArea( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Area);
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

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertSubstation(dataStruct);
			}
		}
	}

	return true;
}

bool StructInsertThread::insertSubstation( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(Substation);
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

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertRemoteUnit(dataStruct);
				insertBusbarSection(dataStruct);
				insertLine(dataStruct);
				insertBreaker(dataStruct);
				insertDisconnector(dataStruct);
				insertPowerTransformer(dataStruct);
			}
		}
	}

	return true;
}

bool StructInsertThread::insertRemoteUnit( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(RemoteUnit);
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
			createNewUnitId();
			
			dataStruct.modifyAttribute(IEDID, getUnitId());

			QString childName = QString("%1-%2").arg(name).arg(i + 1);
			dataStruct.modifyAttribute(IEDNAME, childName);

			QString manufacturer, deviceType;
			getRandomDeviceTypeInfo(manufacturer, deviceType);
			dataStruct.modifyAttribute(MANUFACTURER, manufacturer);
			dataStruct.modifyAttribute(DEVICETYPE, deviceType);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertAnalogUnitPoint(dataStruct);
				insertDiscreteUnitPoint(dataStruct);
				insertControlUnitPoint(dataStruct);
				insertAccumulatorUnitPoint(dataStruct);
			}
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

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertAnalog(dataStruct);
			}
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

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
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

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertAnalog(dataStruct);
				insertDiscrete(dataStruct);
				insertCommand(dataStruct);
				insertAccumulator(dataStruct);
			}
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

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertAnalog(dataStruct);
				insertDiscrete(dataStruct);
				insertCommand(dataStruct);
				insertAccumulator(dataStruct);
			}
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

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertTransformerWinding(dataStruct);
			}
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

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			bool result = insertRdbData(dataStruct.name, fieldValues);
			if (result)
			{
				insertAnalog(dataStruct);
				insertDiscrete(dataStruct);
				insertCommand(dataStruct);
				insertAccumulator(dataStruct);
			}
		}
	}

	return true;
}

bool StructInsertThread::insertAnalogUnitPoint( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(AnalogUnitPoint);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString iedId = parent.getSepecificAttribute(IEDID);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			dataStruct.modifyAttribute(IEDID, iedId);

			QString index = QString("%1").arg(i);
			dataStruct.modifyAttribute(YCINDEX, index);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			insertRdbData(dataStruct.name, fieldValues);
		}
	}

	return true;
}

bool StructInsertThread::insertDiscreteUnitPoint( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(DiscreteUnitPoint);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString iedId = parent.getSepecificAttribute(IEDID);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			dataStruct.modifyAttribute(IEDID, iedId);

			QString index = QString("%1").arg(i);
			dataStruct.modifyAttribute(YXINDEX, index);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			insertRdbData(dataStruct.name, fieldValues);
		}
	}

	return true;
}

bool StructInsertThread::insertControlUnitPoint( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(ControlUnitPoint);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString iedId = parent.getSepecificAttribute(IEDID);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			dataStruct.modifyAttribute(IEDID, iedId);

			QString index = QString("%1").arg(i);
			dataStruct.modifyAttribute(COMMANDID, index);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			insertRdbData(dataStruct.name, fieldValues);
		}
	}

	return true;
}

bool StructInsertThread::insertAccumulatorUnitPoint( const DataStruct& parent )
{
	// 获取信息
	DataStructList dataStructs = parent.getSpecificChildrens(AccumulatorUnitPoint);
	foreach(DataStruct temp, dataStructs)
	{
		int count = temp.getSepecificAttribute(COUNT).toInt();
		if (count <= 0)
		{
			count = 1;
		}
		QString iedId = parent.getSepecificAttribute(IEDID);
		for (int i = 0; i < count; ++i)
		{
			DataStruct dataStruct = temp;
			dataStruct.modifyAttribute(IEDID, iedId);

			QString index = QString("%1").arg(i);
			dataStruct.modifyAttribute(DDINDEX, index);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			insertRdbData(dataStruct.name, fieldValues);
		}
	}

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
		QString iedId = getUnitId();
		QString point = getYcPointId();
		QString psrType = parent.name;
		QString psrRid = parent.getSepecificAttribute(MRID);
		for (int i = 0; i < values.size(); ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1.%2").arg(name).arg(values.at(i));
			dataStruct.modifyAttribute(NAME, "");

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(PSR_TYPE, psrType);
			dataStruct.modifyAttribute(PSR_RID, psrRid);
			dataStruct.modifyAttribute(FTUUNITID, iedId);
			dataStruct.modifyAttribute(FTUPOINTID, point);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			insertRdbData(dataStruct.name, fieldValues);
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
		QString iedId = getUnitId();
		QString point = getYxPointId();
		QString psrType = parent.name;
		QString psrRid = parent.getSepecificAttribute(MRID);
		for (int i = 0; i < values.size(); ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1.%2").arg(name).arg(values.at(i));
			dataStruct.modifyAttribute(NAME, "");

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(PSR_TYPE, psrType);
			dataStruct.modifyAttribute(PSR_RID, psrRid);
			dataStruct.modifyAttribute(FTUUNITID, iedId);
			dataStruct.modifyAttribute(FTUPOINTID, point);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			insertRdbData(dataStruct.name, fieldValues);
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
		QString iedId = getUnitId();
		QString point = getYkPointId();
		QString psrType = parent.name;
		QString psrRid = parent.getSepecificAttribute(MRID);
		for (int i = 0; i < values.size(); ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1.%2").arg(name).arg(values.at(i));
			dataStruct.modifyAttribute(NAME, "");

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(PSR_TYPE, psrType);
			dataStruct.modifyAttribute(PSR_RID, psrRid);
			dataStruct.modifyAttribute(FTUUNITID, iedId);
			dataStruct.modifyAttribute(FTUPOINTID, point);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			insertRdbData(dataStruct.name, fieldValues);
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
		QString iedId = getUnitId();
		QString point = getDdPointId();
		QString psrType = parent.name;
		QString psrRid = parent.getSepecificAttribute(MRID);
		for (int i = 0; i < values.size(); ++i)
		{
			DataStruct dataStruct = temp;
			QString childName = QString("%1.%2").arg(name).arg(values.at(i));
			dataStruct.modifyAttribute(NAME, "");

			QString pathName = QString("%1/%2").arg(parent.getSepecificAttribute(PATHNAME)).arg(childName);
			dataStruct.modifyAttribute(PATHNAME, pathName);

			dataStruct.modifyAttribute(PSR_TYPE, psrType);
			dataStruct.modifyAttribute(PSR_RID, psrRid);
			dataStruct.modifyAttribute(FTUUNITID, iedId);
			dataStruct.modifyAttribute(FTUPOINTID, point);

			// 获取字段值
			vector<string> fieldValues;
			dataStruct.getFieldValues(fieldValues);
			dataStruct.modifyAttribute(MRID, QString::fromStdString(fieldValues[0]));
			insertRdbData(dataStruct.name, fieldValues);
		}
	}

	return true;
}

bool StructInsertThread::insertRdbData( const QString& tableName, const vector<string>& fieldValues )
{
	RespondCompleteDataSeq repSeq;
	repSeq.id = 1;
	repSeq.requestId = 1;
	repSeq.requestNode = "randomInsert";

	RespondCompleteData data;
	data.tableName = tableName.toStdString();
	data.dataValues.assign(fieldValues.begin(), fieldValues.end());

	repSeq.seq.push_back(data);
	repSeq.dataCount = repSeq.seq.size();
	
	RespondCompleteDataSequence repSequence;
	try
	{
		RdbDataOptPrx rdbDataOptPrx_timeout = RdbDataOptPrx::uncheckedCast(
			m_rdbDataOptPrx->ice_timeout(5000));
		bool result = rdbDataOptPrx_timeout->InsertData(repSeq, repSequence);
		if (result)
		{
			RdbLog(CLogger::Log_INFO, "插入 %s 成功", tableName.toStdString().c_str());
			return true;
		}
		else
		{
			RdbLog(CLogger::Log_INFO, "!!!!!!!!!!!!!! 插入 %s 失败 !!!!!!!!!!!!!!!", tableName.toStdString().c_str());
			return false;
		}
	}
	catch(const Ice::Exception& ex)
	{
		RdbLog(CLogger::Log_ERROR, "插入 %s 数据时出现异常: %s", tableName.toStdString().c_str(), ex.what());
		return false;
	}		
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

void StructInsertThread::createNewUnitId()
{
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
