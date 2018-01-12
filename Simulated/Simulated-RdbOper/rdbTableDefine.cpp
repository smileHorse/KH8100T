
#include <IceUtil/IceUtil.h>

#include "rdbTableFactory.h"

#define AreaCountPerSubGeographicalRegion		1		// 每个子区域包含的区间个数
#define SubstationCountPerSubGeographicalRegion	2		// 每个子区域包含的变电站个数
#define SubstationCountPerArea					8		// 每个区间包含的变电站个数
#define BusbarCountPerSubstation				2		// 每个变电站包含的母线个数
#define BreakerCountPerSubstation				4		// 每个变电站包含的断路器个数
#define DisconnectorCountPerSubstation			4		// 每个变电站包含的刀闸个数
#define PowerTransCountPerSubstation			2		// 每个变电站包含的变压器个数
#define TransformCountPerSubstation				4		// 每个变电站包含的绕组个数
#define TransformCountPerPowerTrans				2		// 每个变压器包含的绕组个数
#define LineCountPerSubstation					2		// 每个变电站包含的线路个数
#define DeviceTypeCountPerManufacture			5		// 每个设备厂家包含的设备类型个数
#define PointCountPerRemoteUnit					1000	// 每个单元的点号个数

/************************************************************************/
/* 基类表                                                               */
/************************************************************************/
void BaseTable::getRandomValues( StringVector& values )
{
	QString tableName = getTableName();
	// 根据表名获取表的字段信息
	RdbFieldStructList fields;
	RdbTableFactory::getTableFields(tableName, fields);

	getRandomValuesImpl(fields, values);
}

void BaseTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;

		generateCommonRandomValue(field, values);
	}
}

void BaseTable::generateCommonRandomValue( const RdbFieldStruct& field, StringVector& values )
{
	QString tableName = getTableName();
	QString fieldType = field.fieldType;
	QString fieldName = field.fieldName;
	if (fieldType == "string")
	{
		if (fieldName == "mRID")
		{
			values.push_back(generateMRID());
		}
		else if (fieldName == "name")
		{
			values.push_back(generateName());
		}
		else if (fieldName == "pathName")
		{
			values.push_back(generateMRID());
		}
		else
		{
			QString text = QString("%1-%2").arg(tableName).arg(qrand());
			values.push_back(text.toStdString());
		}
	}
	else if (fieldType == "int")
	{
		int value = qrand();
		values.push_back(QString::number(value).toStdString());
	}
	else if (fieldType == "double")
	{
		double value = ((double)qrand() * 5.0) / 3.0;
		values.push_back(QString::number(value).toStdString());
	}
	else if (fieldType == "bool")
	{
		bool value = (qrand() % 2 == 0);
		values.push_back(QString::number(value).toStdString());
	}
	else
	{
		if (fieldName == "ec_type" || fieldName == "psrType")
		{
			values.push_back(fieldType.toStdString());
		}
		else
		{
			QVector<string> tableMRIDs = RdbTableFactory::getTableMRIDs(fieldType);
			if (!tableMRIDs.isEmpty())
			{
				int index = qrand() % tableMRIDs.size();
				values.push_back(tableMRIDs.at(index));
			}
			else
			{
				values.push_back("");
			}
		}		
	}
}

// 产生mRID
std::string BaseTable::generateMRID()
{
	return IceUtil::generateUUID();
}

// 产生name
std::string BaseTable::generateName()
{
	QDateTime current = QDateTime::currentDateTime();
	QDate currentDate = current.date();
	QTime currentTime = current.time();
	QString displayName = getTableDisplayName();
	QString name = QStringLiteral("%1-%2%3%4%5%6%7").arg(displayName).arg(currentDate.month()).arg(currentDate.day())
					.arg(currentTime.hour()).arg(currentTime.minute()).arg(currentTime.second())
					.arg(currentTime.msec());
	return name.toLocal8Bit().toStdString();
}

// 获取引用的对象的mRID
std::string BaseTable::getMRIDOfReferenceObject( const QString& tableName, int index, int perValue )
{
	QVector<string> tableMRIDs = RdbTableFactory::getTableMRIDs(tableName);
	if (!tableMRIDs.isEmpty())
	{
		int i = ((index -1) / perValue) + 1;
		if (i < tableMRIDs.size())
		{
			return tableMRIDs[i];
		}
		else
		{
			return tableMRIDs[0];
		}
	}
	
	return "";
}

/************************************************************************/
/* 区域表                                                               */
/************************************************************************/
QString GeographicalRegionTable::getTableName() const
{
	return "GeographicalRegion";
}

QString GeographicalRegionTable::getTableDisplayName() const
{
	return QStringLiteral("区域");
}

void GeographicalRegionTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	BaseTable::getRandomValuesImpl(fields, values);
}

/************************************************************************/
/* 子区域表                                                             */
/************************************************************************/
QString SubGeographicalRegionTable::getTableName() const
{
	return "SubGeographicalRegion";
}

QString SubGeographicalRegionTable::getTableDisplayName() const
{
	return QStringLiteral("子区域");
}

void SubGeographicalRegionTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	BaseTable::getRandomValuesImpl(fields, values);
}

/************************************************************************/
/* 区间表                                                             */
/************************************************************************/
QString AreaTable::getTableName() const
{
	return "Area";
}

QString AreaTable::getTableDisplayName() const
{
	return QStringLiteral("区间");
}

void AreaTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int areaCount = 0;
	areaCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "ec_rid")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, areaCount, AreaCountPerSubGeographicalRegion));
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}
/************************************************************************/
/* 变电站表                                                             */
/************************************************************************/
QString SubstationTable::getTableName() const
{
	return "Substation";
}

QString SubstationTable::getTableDisplayName() const
{
	return QStringLiteral("变电站");
}

void SubstationTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int substationCount = 0;
	substationCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "ec_type")
		{
			if (substationCount <= 200)
			{
				values.push_back("SubGeographicalRegion");
			}
			else
			{
				values.push_back("Area");
			}
		}
		else if (fieldName == "ec_rid")
		{
			if (substationCount <= 200)
			{
				values.push_back(getMRIDOfReferenceObject("SubGeographicalRegion", substationCount, 
					SubstationCountPerSubGeographicalRegion));
			}
			else
			{
				values.push_back(getMRIDOfReferenceObject("Area", substationCount - 200, 
					SubstationCountPerArea));
			}
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 母线表                                                             */
/************************************************************************/
QString BusbarSectionTable::getTableName() const
{
	return "BusbarSection";
}

QString BusbarSectionTable::getTableDisplayName() const
{
	return QStringLiteral("母线");
}

void BusbarSectionTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int busbarCount = 0;
	busbarCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "ec_rid")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, busbarCount, 
				BusbarCountPerSubstation));
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 线路表                                                             */
/************************************************************************/
QString LineTable::getTableName() const
{
	return "Line";
}

QString LineTable::getTableDisplayName() const
{
	return QStringLiteral("线路");
}

void LineTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int lineCount = 0;
	lineCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "ec_rid")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, lineCount, 
				LineCountPerSubstation));
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 断路器表                                                             */
/************************************************************************/
QString BreakerTable::getTableName() const
{
	return "Breaker";
}

QString BreakerTable::getTableDisplayName() const
{
	return QStringLiteral("断路器");
}

void BreakerTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int breakerCount = 0;
	breakerCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "ec_rid")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, breakerCount, 
				BreakerCountPerSubstation));
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 刀闸表                                                             */
/************************************************************************/
QString DisconnectorTable::getTableName() const
{
	return "Disconnector";
}

QString DisconnectorTable::getTableDisplayName() const
{
	return QStringLiteral("刀闸");
}

void DisconnectorTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int disconnectorCount = 0;
	disconnectorCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "ec_rid")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, disconnectorCount, 
				DisconnectorCountPerSubstation));
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 变压器表                                                             */
/************************************************************************/
QString PowerTransformerTable::getTableName() const
{
	return "PowerTransformer";
}

QString PowerTransformerTable::getTableDisplayName() const
{
	return QStringLiteral("变压器");
}

void PowerTransformerTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int powerTransCount = 0;
	powerTransCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "ec_rid")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, powerTransCount, 
				PowerTransCountPerSubstation));
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 绕组表                                                             */
/************************************************************************/
QString TransformerWindingTable::getTableName() const
{
	return "TransformerWinding";
}

QString TransformerWindingTable::getTableDisplayName() const
{
	return QStringLiteral("绕组");
}

void TransformerWindingTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int transFormCount = 0;
	transFormCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "ec_rid")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, transFormCount, 
				TransformCountPerSubstation));
		}
		else if (fieldName == "power_tranformer")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, transFormCount, 
				TransformCountPerPowerTrans));
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 单元表                                                             */
/************************************************************************/
QString RemoteUnitTable::getTableName() const
{
	return "RemoteUnit";
}

QString RemoteUnitTable::getTableDisplayName() const
{
	return QStringLiteral("单元");
}

void RemoteUnitTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int remoteCount = 0;
	remoteCount++;
	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "IEDID")
		{
			values.push_back(QString().number(remoteCount).toStdString());
		}
		else if (fieldName == "IEDName")
		{
			values.push_back(QStringLiteral("单元%1").arg(remoteCount).toLocal8Bit().toStdString());
		}
		else if (fieldName == "ec_rid")
		{
			QVector<string> mRIDs = RdbTableFactory::getTableMRIDs("Substation");
			if (mRIDs.size() >= remoteCount)
			{
				values.push_back(mRIDs.at(remoteCount - 1));
			}
			else
			{
				values.push_back("");
			}
		}
		else if (fieldName == "yxNum")
		{
			values.push_back("10000");
		}
		else if (fieldName == "ycNum")
		{
			values.push_back("10000");
		}
		else if (fieldName == "ddNum")
		{
			values.push_back("10000");
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 遥测点表                                                             */
/************************************************************************/
QString AnalogUnitPointTable::getTableName() const
{
	return "AnalogUnitPoint";
}

QString AnalogUnitPointTable::getTableDisplayName() const
{
	return QStringLiteral("遥测点");
}

void AnalogUnitPointTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int analogUnitPointCount = 0;
	analogUnitPointCount++;

	int value1 = analogUnitPointCount / PointCountPerRemoteUnit;
	int value2 = analogUnitPointCount % PointCountPerRemoteUnit;
	int iedIndex = value1 + 1;
	int ycIndex = value2 + 1;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "IEDID")
		{
			values.push_back(QString().number(iedIndex).toStdString());
		}
		else if (fieldName == "YcIndex")
		{
			values.push_back(QString().number(ycIndex).toStdString());
		}
		else if (fieldName == "YcName")
		{
			values.push_back(QStringLiteral("遥测-%1-%2").arg(iedIndex).arg(ycIndex).toLocal8Bit().toStdString());
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 遥信点表                                                             */
/************************************************************************/
QString DiscreteUnitPointTable::getTableName() const
{
	return "DiscreteUnitPoint";
}

QString DiscreteUnitPointTable::getTableDisplayName() const
{
	return QStringLiteral("遥信点");
}

void DiscreteUnitPointTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int discreteUnitPointCount = 0;
	discreteUnitPointCount++;

	int value1 = discreteUnitPointCount / PointCountPerRemoteUnit;
	int value2 = discreteUnitPointCount % PointCountPerRemoteUnit;
	int iedIndex = value1 + 1;
	int yxIndex = value2 + 1;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "IEDID")
		{
			values.push_back(QString().number(iedIndex).toStdString());
		}
		else if (fieldName == "YcIndex")
		{
			values.push_back(QString().number(yxIndex).toStdString());
		}
		else if (fieldName == "YcName")
		{
			values.push_back(QStringLiteral("遥测-%1-%2").arg(iedIndex).arg(yxIndex).toLocal8Bit().toStdString());
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 遥控点表                                                             */
/************************************************************************/
QString ControlUnitPointTable::getTableName() const
{
	return "ControlUnitPoint";
}

QString ControlUnitPointTable::getTableDisplayName() const
{
	return QStringLiteral("遥控点");
}

void ControlUnitPointTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int controlUnitPointCount = 0;
	controlUnitPointCount++;

	int value1 = controlUnitPointCount / PointCountPerRemoteUnit;
	int value2 = controlUnitPointCount % PointCountPerRemoteUnit;
	int iedIndex = value1 + 1;
	int ykIndex = value2 + 1;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "IEDID")
		{
			values.push_back(QString().number(iedIndex).toStdString());
		}
		else if (fieldName == "CommandID")
		{
			values.push_back(QString().number(ykIndex).toStdString());
		}
		else if (fieldName == "CommandName")
		{
			values.push_back(QStringLiteral("遥控-%1-%2").arg(iedIndex).arg(ykIndex).toLocal8Bit().toStdString());
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}

/************************************************************************/
/* 设备厂家表                                                             */
/************************************************************************/
QString DevManufacturerTable::getTableName() const
{
	return "DevManufacturer";
}

QString DevManufacturerTable::getTableDisplayName() const
{
	return QStringLiteral("设备厂家");
}

/************************************************************************/
/* 设备类型表                                                             */
/************************************************************************/
QString DevDeviceTypeTable::getTableName() const
{
	return "DevDeviceType";
}

QString DevDeviceTypeTable::getTableDisplayName() const
{
	return QStringLiteral("设备类型");
}

void DevDeviceTypeTable::getRandomValuesImpl( const RdbFieldStructList& fields, StringVector& values )
{
	static int deviceTypeCount = 0;
	deviceTypeCount++;

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldName == "manufacturer")
		{
			values.push_back(getMRIDOfReferenceObject(fieldType, deviceTypeCount, DeviceTypeCountPerManufacture));
		}
		else
		{
			generateCommonRandomValue(field, values);
		}
	}
}
