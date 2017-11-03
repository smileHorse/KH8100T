#include "rdbtable.h"
#include "FdbTableDefine.h"

#include <sstream>
#include <QtCore/QUuid>

using namespace std;

const string SPLITER_TOKEN = ",";

template <class output_type,class input_type>
output_type transferType(const input_type &input)
{
	stringstream ss;
	ss<<input;
	output_type result;
	ss>>result;
	return result;
}

BaseTable::BaseTable( dbDatabase* dbPtr )
	: m_dbPtr(dbPtr)
{}

QStringList BaseTable::getTableFieldNames( const string& tableName )
{
	if (m_dbPtr->isOpen())
	{
		dbTableDescriptor* tableDesc = m_dbPtr->findTableByName(tableName.c_str());
		if (tableDesc)
		{
			QStringList names;
			dbFieldDescriptor* fieldDesc = tableDesc->getFirstField();
			do 
			{
				if (fieldDesc)
				{
					names.push_back(fieldDesc->name);
				}
				else
				{
					break;
				}
			} while (fieldDesc = fieldDesc->nextField);

			return names;
		}
		else
		{
			return QStringList();
		}
	}
	else
	{
		return QStringList();
	}
}

bool BaseTable::deleteDataByCursor( dbAnyCursor& cursor, const string& mRID )
{
	dbQuery query;
	query = "mRID=", mRID;
	try 
	{
		if (cursor.select(query) == 1)
		{
			cursor.remove();
			m_dbPtr->commit();
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(const dbException& ex)
	{
		string error = ex.what();
	}
	return true;
}

bool BaseTable::updateDataByCursor( dbAnyCursor& cursor, const string& mRID, const QMap<QString,QString>& values )
{
	dbQuery query;
	query = "mRID=", mRID;
	try 
	{
		if (cursor.select(query) == 1)
		{
			QMap<QString,QString>::const_iterator iter = values.begin();
			for (; iter != values.end(); ++iter)
			{
				string fieldName = iter.key().toStdString();
				string value = iter.value().toStdString();
				setFieldValue(cursor, fieldName, value);
			}			
			cursor.update();
			m_dbPtr->commit();
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(const dbException& ex)
	{
		string error = ex.what();
	}
	return true;
}

bool BaseTable::GetDbReference( dbAnyCursor& table, const string rid )
{
	dbQuery q1;
	q1 = "mRID = ", rid;

	if (table.select(q1) == 1)
		return true;

	return false;
}

GeographicalRegionTable::GeographicalRegionTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{
}

QList<QStringList> GeographicalRegionTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<GeographicalRegion> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName << "," 
				<< cursor->aliasName << "," << cursor->description << ",";
			if (cursor->sub_regions.length() > 0)
			{
				for (int i = 0; i < cursor->sub_regions.length(); ++i)
				{
					if (i != 0)
					{
						str << "/";
					}
					str << cursor->sub_regions.getat(i).getOid();
				}
				str << cursor->sub_regions.getat(0).getOid();
			}
			else
			{
				str << "";
			}

			str << "," << cursor.currentId().getOid();	// 特殊追加，否则区域表少一列

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool GeographicalRegionTable::insertData()
{
	GeographicalRegion info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.name = "";
	info.localName = "";
	info.pathName = "";
	info.aliasName = "";
	info.description = "";
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool GeographicalRegionTable::deleteData( const string& mRID )
{
	dbCursor<GeographicalRegion> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool GeographicalRegionTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<GeographicalRegion> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void GeographicalRegionTable::getHidedColumns( QList<int>& hideColumns )
{

}

void GeographicalRegionTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<GeographicalRegion>& cursor = static_cast< dbCursor<GeographicalRegion>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "localName")
	{
		cursor->localName = value;
	}
	else if (fieldName == "pathName")
	{
		cursor->pathName = value;
	}
	else if (fieldName == "aliasName")
	{
		cursor->aliasName = value;
	}
	else if (fieldName == "description")
	{
		cursor->description = value;
	}
}

SubGeographicalRegionTable::SubGeographicalRegionTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{
}

QList<QStringList> SubGeographicalRegionTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<SubGeographicalRegion> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName << "," 
				<< cursor->aliasName << "," << cursor->description << "," << cursor->region.getOid();
			
			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool SubGeographicalRegionTable::insertData()
{
	SubGeographicalRegion info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool SubGeographicalRegionTable::deleteData( const string& mRID )
{
	dbCursor<SubGeographicalRegion> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool SubGeographicalRegionTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<SubGeographicalRegion> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void SubGeographicalRegionTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5;
}

void SubGeographicalRegionTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<SubGeographicalRegion>& cursor = static_cast< dbCursor<SubGeographicalRegion>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "localName")
	{
		cursor->localName = value;
	}
	else if (fieldName == "pathName")
	{
		cursor->pathName = value;
	}
	else if (fieldName == "aliasName")
	{
		cursor->aliasName = value;
	}
	else if (fieldName == "description")
	{
		cursor->description = value;
	}
	else if (fieldName == "region")
	{
		string region_rid = value;
		dbCursor<GeographicalRegion> regions;
		bool ret = GetDbReference(regions, region_rid);
		if (ret)
		{
			cursor->region = regions.currentId();
		}
	}
}

AreaTable::AreaTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{
}

QList<QStringList> AreaTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Area> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName << "," 
				<< cursor->aliasName << "," << cursor->description 
				<< "," << cursor->psrType << "," << cursor->ec_type << "," << cursor->ec_rid;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool AreaTable::insertData()
{
	Area info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool AreaTable::deleteData( const string& mRID )
{
	dbCursor<Area> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool AreaTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Area> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void AreaTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5;
}

void AreaTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<Area>& cursor = static_cast< dbCursor<Area>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psrType")
	{
		cursor->psrType = value;
	}
	else if (fieldName == "ec_type")
	{
		cursor->ec_type = value;
	}
	else if (fieldName == "ec_rid")
	{
		cursor->ec_rid = value;
	}
}

RemoteUnitTable::RemoteUnitTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> RemoteUnitTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<RemoteUnit> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->IEDID << "," << cursor->IEDName << "," << cursor->IEDType << "," 
				<< cursor->timeStamp << "," << cursor->status << "," << cursor->channelState1 << "," 
				<< cursor->channelState2 << "," << cursor->errorRate << "," << cursor->safeDays << "," 
				<< cursor->dayRate << "," << cursor->monthRate;

			QString value = QString().fromStdString(str.str());
			
			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool RemoteUnitTable::insertData()
{
	RemoteUnit info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.IEDID = 0;
	info.IEDName = "";
	info.IEDType = 0;
	info.timeStamp = "";
	info.status = 0;
	info.channelState1 = 0;
	info.channelState2 = 0;
	info.errorRate = 0;
	info.safeDays = 0;
	info.dayRate = 0;
	info.monthRate = 0;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool RemoteUnitTable::deleteData( const string& mRID )
{
	dbCursor<RemoteUnit> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool RemoteUnitTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<RemoteUnit> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void RemoteUnitTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<RemoteUnit>& cursor = static_cast< dbCursor<RemoteUnit>& >(anyCursor);
	if (fieldName == "IEDID")
	{
		cursor->IEDID = transferType<int4, string>(value);
	}
	else if (fieldName == "IEDName")
	{
		cursor->IEDName = value;
	}
	else if (fieldName == "IEDType")
	{
		cursor->IEDType = transferType<int4, string>(value);
	}
}

void RemoteUnitTable::getHidedColumns( QList<int>& hideColumns )
{
	
}

AnalogUnitPointTable::AnalogUnitPointTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> AnalogUnitPointTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<AnalogUnitPoint> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->IEDID << "," << cursor->YcIndex << "," << cursor->YcName;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool AnalogUnitPointTable::insertData()
{
	AnalogUnitPoint info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.IEDID = 0;
	info.YcIndex = 0;
	info.YcName = "";
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool AnalogUnitPointTable::deleteData( const string& mRID )
{
	dbCursor<AnalogUnitPoint> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool AnalogUnitPointTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<AnalogUnitPoint> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void AnalogUnitPointTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<AnalogUnitPoint>& cursor = static_cast< dbCursor<AnalogUnitPoint>& >(anyCursor);
	if (fieldName == "IEDID")
	{
		cursor->IEDID = transferType<int, string>(value);
	}
	else if (fieldName == "YcIndex")
	{
		cursor->YcIndex = transferType<int, string>(value);
	}
	else if (fieldName == "YcName")
	{
		cursor->YcName = value;
	}
}

void AnalogUnitPointTable::getHidedColumns( QList<int>& hideColumns )
{

}

DiscreteUnitPointTable::DiscreteUnitPointTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> DiscreteUnitPointTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<DiscreteUnitPoint> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->IEDID << "," << cursor->YxIndex << "," << cursor->YxName;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool DiscreteUnitPointTable::insertData()
{
	DiscreteUnitPoint info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.IEDID = 0;
	info.YxIndex = 0;
	info.YxName = "";
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool DiscreteUnitPointTable::deleteData( const string& mRID )
{
	dbCursor<DiscreteUnitPoint> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool DiscreteUnitPointTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<DiscreteUnitPoint> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void DiscreteUnitPointTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<DiscreteUnitPoint>& cursor = static_cast< dbCursor<DiscreteUnitPoint>& >(anyCursor);
	if (fieldName == "IEDID")
	{
		cursor->IEDID = transferType<int, string>(value);
	}
	else if (fieldName == "YxIndex")
	{
		cursor->YxIndex = transferType<int, string>(value);
	}
	else if (fieldName == "YxName")
	{
		cursor->YxName = value;
	}
}

void DiscreteUnitPointTable::getHidedColumns( QList<int>& hideColumns )
{

}

ControlUnitPointTable::ControlUnitPointTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> ControlUnitPointTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<ControlUnitPoint> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->IEDID << "," << cursor->CommandID << "," << cursor->CommandName;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool ControlUnitPointTable::insertData()
{
	ControlUnitPoint info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.CommandID = 0;
	info.CommandName = "";
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool ControlUnitPointTable::deleteData( const string& mRID )
{
	dbCursor<ControlUnitPoint> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool ControlUnitPointTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<ControlUnitPoint> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void ControlUnitPointTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<ControlUnitPoint>& cursor = static_cast< dbCursor<ControlUnitPoint>& >(anyCursor);
	if (fieldName == "IEDID")
	{
		cursor->IEDID = transferType<int, string>(value);
	}
	else if (fieldName == "CommandID")
	{
		cursor->CommandID = transferType<int, string>(value);
	}
	else if (fieldName == "CommandName")
	{
		cursor->CommandName = value;
	}
}

void ControlUnitPointTable::getHidedColumns( QList<int>& hideColumns )
{
	
}

SubstationTable::SubstationTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> SubstationTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Substation> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->psrType 
				<< "," << cursor->ec_type << "," << cursor->ec_rid;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool SubstationTable::insertData()
{
	Substation info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool SubstationTable::deleteData( const string& mRID )
{
	dbCursor<Substation> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool SubstationTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Substation> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void SubstationTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5;
}

void SubstationTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<Substation>& cursor = static_cast< dbCursor<Substation>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psrType")
	{
		cursor->psrType = value;
	}
	else if (fieldName == "ec_type")
	{
		cursor->ec_type = value;
	}
	else if (fieldName == "ec_rid")
	{
		cursor->ec_rid = value;
	}
}

VoltageLevelTable::VoltageLevelTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> VoltageLevelTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<VoltageLevel> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->psrType 
				<< "," << cursor->highVoltageLimit << "," << cursor->lowVoltageLimit << "," << cursor->ec_type 
				<< "," << cursor->ec_rid << "," << cursor->base_voltage.getOid();

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool VoltageLevelTable::insertData()
{
	VoltageLevel info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.highVoltageLimit = 0;
	info.lowVoltageLimit = 0;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool VoltageLevelTable::deleteData( const string& mRID )
{
	dbCursor<VoltageLevel> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool VoltageLevelTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<VoltageLevel> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void VoltageLevelTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5;
}

void VoltageLevelTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<VoltageLevel>& cursor = static_cast< dbCursor<VoltageLevel>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psrType")
	{
		cursor->psrType = value;
	}
	else if (fieldName == "highVoltageLimit")
	{
		cursor->highVoltageLimit = transferType<real8, string>(value);
	}
	else if (fieldName == "lowVoltageLimit")
	{
		cursor->lowVoltageLimit = transferType<real8, string>(value);
	}
	else if (fieldName == "ec_type")
	{
		cursor->ec_type = value;
	}
	else if (fieldName == "ec_rid")
	{
		cursor->ec_rid = value;
	}
}

BusbarSectionTable::BusbarSectionTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> BusbarSectionTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<BusbarSection> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->psrType 
				<< "," << cursor->ec_type << "," << cursor->ec_rid 
				<< "," << cursor->phase << "," << cursor->powerPoint << "," << cursor->base_voltage;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool BusbarSectionTable::insertData()
{
	BusbarSection info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.phase = 0;
	info.powerPoint = false;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool BusbarSectionTable::deleteData( const string& mRID )
{
	dbCursor<BusbarSection> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool BusbarSectionTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<BusbarSection> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void BusbarSectionTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5;
}

void BusbarSectionTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<BusbarSection>& cursor = static_cast< dbCursor<BusbarSection>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psrType")
	{
		cursor->psrType = value;
	}
	else if (fieldName == "ec_type")
	{
		cursor->ec_type = value;
	}
	else if (fieldName == "ec_rid")
	{
		cursor->ec_rid = value;
	}
	else if (fieldName == "phase")
	{
		cursor->phase = transferType<int4, string>(value);
	}
	else if (fieldName == "powerPoint")
	{
		cursor->powerPoint = transferType<bool, string>(value);
	}
	else if (fieldName == "base_voltage")
	{
		cursor->base_voltage = value;
	}
}

LineTable::LineTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> LineTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Line> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << SPLITER_TOKEN << cursor->name << SPLITER_TOKEN << cursor->localName 
				<< SPLITER_TOKEN << cursor->pathName 
				<< SPLITER_TOKEN << cursor->aliasName << SPLITER_TOKEN << cursor->description 
				<< SPLITER_TOKEN << cursor->psrType 
				<< SPLITER_TOKEN << cursor->voltageLevel << SPLITER_TOKEN << cursor->ec_type 
				<< SPLITER_TOKEN << cursor->ec_rid 
				<< SPLITER_TOKEN << cursor->ctRatio << SPLITER_TOKEN << cursor->ptRatio 
				<< SPLITER_TOKEN << cursor->lineType << SPLITER_TOKEN << cursor->lineNo;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(QString().fromStdString(SPLITER_TOKEN));
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool LineTable::insertData()
{
	Line info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.ctRatio = 0.0;
	info.ptRatio = 0.0;
	info.lineNo = 0;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool LineTable::deleteData( const string& mRID )
{
	dbCursor<Line> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool LineTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Line> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void LineTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5;
}

void LineTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<Line>& cursor = static_cast< dbCursor<Line>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psrType")
	{
		cursor->psrType = value;
	}
	else if (fieldName == "ec_type")
	{
		cursor->ec_type = value;
	}
	else if (fieldName == "ec_rid")
	{
		cursor->ec_rid = value;
	}
	else if (fieldName == "voltageLevel")
	{
		cursor->voltageLevel = value;
	}
	else if (fieldName == "ctRatio")
	{
		cursor->ctRatio = transferType<double, string>(value);
	}
	else if (fieldName == "ptRatio")
	{
		cursor->ptRatio = transferType<double, string>(value);
	}
	else if (fieldName == "lineType")
	{
		cursor->lineType = value;
	}
	else if (fieldName == "lineNo")
	{
		cursor->lineNo = transferType<int4, string>(value);
	}
}

BreakerTable::BreakerTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> BreakerTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Breaker> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->psrType 
				<< "," << cursor->ec_type << "," << cursor->ec_rid 
				<< "," << cursor->phase << "," << cursor->powerPoint << "," << cursor->base_voltage 
				<< "," << cursor->normalOpen << "," << cursor->switchOnCount << "," << cursor->switchOnDate 
				<< "," << cursor->switchTranMode << "," << cursor->switchCommandType << "," << cursor->brandFlag 
				<< "," << cursor->ratedCurrent << "," << cursor->inTransitTime << "," << cursor->faFlag 
				<< "," << cursor->PowerPointFlag << "," << cursor->PPThreshold << "," << cursor->FTUNo 
				<< "," << cursor->EventID << "," << cursor->WaveDirFlag << "," << cursor->factorDL_I 
				<< "," << cursor->factorJD_I << "," << cursor->breakerImportant;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool BreakerTable::insertData()
{
	Breaker info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.phase = 0;
	info.powerPoint = false;
	info.normalOpen = false;
	info.switchOnCount = 0;
	info.ratedCurrent = 0.0;
	info.inTransitTime = 0.0;
	info.faFlag = false;
	info.PowerPointFlag = false;
	info.PPThreshold = 0.0;
	info.FTUNo = 0;
	info.EventID = 0;
	info.WaveDirFlag = false;
	info.factorDL_I = 0.0;
	info.factorJD_I = 0.0;
	info.breakerImportant = false;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool BreakerTable::deleteData( const string& mRID )
{
	dbCursor<Breaker> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool BreakerTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Breaker> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void BreakerTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 22;
}

void BreakerTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<Breaker>& cursor = static_cast< dbCursor<Breaker>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psrType")
	{
		cursor->psrType = value;
	}
	else if (fieldName == "ec_type")
	{
		cursor->ec_type = value;
	}
	else if (fieldName == "ec_rid")
	{
		cursor->ec_rid = value;
	}
	else if (fieldName == "phase")
	{
		cursor->phase = transferType<int4, string>(value);
	}
	else if (fieldName == "powerPoint")
	{
		cursor->powerPoint = transferType<bool, string>(value);
	}
	else if (fieldName == "faFlag")
	{
		cursor->faFlag = transferType<bool, string>(value);
	}
	else if (fieldName == "PowerPointFlag")
	{
		cursor->PowerPointFlag = transferType<bool, string>(value);
	}
	else if (fieldName == "PPThreshold")
	{
		cursor->PPThreshold = transferType<double, string>(value);
	}
	else if (fieldName == "FTUNo")
	{
		cursor->FTUNo = transferType<int4, string>(value);
	}
	else if (fieldName == "EventID")
	{
		cursor->EventID = transferType<int4, string>(value);
	}
	else if (fieldName == "WaveDirFlag")
	{
		cursor->WaveDirFlag = transferType<bool, string>(value);
	}
	else if (fieldName == "factorDL_I")
	{
		cursor->factorDL_I = transferType<double, string>(value);
	}
	else if (fieldName == "factorJD_I")
	{
		cursor->factorJD_I = transferType<double, string>(value);
	}
	else if (fieldName == "breakerImportant")
	{
		cursor->breakerImportant = transferType<bool, string>(value);
	}
}

AnalogTable::AnalogTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> AnalogTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Analog> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->measurementType << "," << cursor->unitMultiplier << "," << cursor->unitSymbol 
				<< "," << cursor->badReference << "," << cursor->estimatorReplaced << "," << cursor->failure 
				<< "," << cursor->oldData << "," << cursor->operatorBlocked << "," << cursor->oscillatory 
				<< "," << cursor->outOfRange << "," << cursor->overFlow << "," << cursor->source 
				<< "," << cursor->suspect << "," << cursor->test << "," << cursor->validity 
				<< "," << cursor->valueSource << "," << cursor->timeStamp << "," << cursor->sensorAccuracy 
				<< "," << cursor->saveSection << "," << cursor->lockFlag << "," << cursor->holdFlag 
				<< "," << cursor->psr_type << "," << cursor->psr_rid 
				<< "," << cursor->saveReport << "," << cursor->maxValue << "," << cursor->minValue 
				<< "," << cursor->normalValue << "," << cursor->positiveFlowIn << "," << cursor->ratioFactor 
				<< "," << cursor->value << "," << cursor->checkLimit << "," << cursor->hhLimitValue 
				<< "," << cursor->hLimitValue << "," << cursor->lLimitValue << "," << cursor->llLimitValue 
				<< "," << cursor->hlimitDead << "," << cursor->llimitDead << "," << cursor->isPercentageLimits 
				<< "," << cursor->limitStatus << "," << cursor->ftuUnitId << "," << cursor->ftuPointId 
				<< "," << cursor->ftuVlDesc << "," << cursor->analog_curve.getOid() << "," << cursor->analog_formula.getOid();

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool AnalogTable::insertData()
{
	Analog info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.measurementType = 0;
	info.unitMultiplier = 0;
	info.unitSymbol = 0;
	info.badReference = false;
	info.estimatorReplaced = false;
	info.failure = false;
	info.oldData = false;
	info.operatorBlocked = false;
	info.oscillatory = false;
	info.outOfRange = false;
	info.overFlow = false;
	info.source = false;
	info.suspect = false;
	info.test = false;
	info.validity = false;
	info.sensorAccuracy = 0;
	info.saveSection = false;
	info.lockFlag = false;
	info.holdFlag = false;
	info.saveReport = false;
	info.maxValue = 0;
	info.minValue = 0;
	info.normalValue = 0;
	info.positiveFlowIn = false;
	info.ratioFactor = 1;
	info.value = 50;
	info.checkLimit = true;
	info.hhLimitValue = 100;
	info.hLimitValue = 98;
	info.lLimitValue = 8;
	info.llLimitValue = 10;
	info.hlimitDead = 1;
	info.llimitDead = 1;
	info.isPercentageLimits = false;
	info.limitStatus = 0;
	info.ftuUnitId = 0;
	info.ftuPointId = 0;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool AnalogTable::deleteData( const string& mRID )
{
	dbCursor<Analog> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool AnalogTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Analog> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void AnalogTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 
		<< 19 << 20 << 21 << 22 << 33 << 43 << 47;
	//hideColumns << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 
	//	<< 19 << 20 << 21 << 23 << 30 << 31 << 32 << 33 << 34 << 35 << 36 << 37 << 38 << 39 << 40 << 41 
	//	<< 42 << 43 << 44 << 47;
}

void AnalogTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<Analog>& cursor = static_cast< dbCursor<Analog>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psr_type")
	{
		cursor->psr_type = value;
	}
	else if (fieldName == "psr_rid")
	{
		cursor->psr_rid = value;
	}
	else if (fieldName == "maxValue")
	{
		cursor->maxValue = transferType<real8, string>(value);
	}
	else if (fieldName == "minValue")
	{
		cursor->minValue = transferType<real8, string>(value);
	}
	else if (fieldName == "normalValue")
	{
		cursor->normalValue = transferType<real8, string>(value);
	}
	else if (fieldName == "value")
	{
		cursor->value = transferType<real8, string>(value);
	}
	else if (fieldName == "checkLimit")
	{
		cursor->checkLimit = transferType<bool, string>(value);
	}
	else if (fieldName == "hhLimitValue")
	{
		cursor->hhLimitValue = transferType<real8, string>(value);
	}
	else if (fieldName == "hLimitValue")
	{
		cursor->hLimitValue = transferType<real8, string>(value);
	}
	else if (fieldName == "lLimitValue")
	{
		cursor->lLimitValue = transferType<real8, string>(value);
	}
	else if (fieldName == "llLimitValue")
	{
		cursor->llLimitValue = transferType<real8, string>(value);
	}
	else if (fieldName == "hlimitDead")
	{
		cursor->hlimitDead = transferType<real8, string>(value);
	}
	else if (fieldName == "llimitDead")
	{
		cursor->llimitDead = transferType<real8, string>(value);
	}
	else if (fieldName == "ftuUnitId")
	{
		cursor->ftuUnitId = transferType<int4, string>(value);
	}
	else if (fieldName == "ftuPointId")
	{
		cursor->ftuPointId = transferType<int4, string>(value);
	}
	else if (fieldName == "saveReport")
	{
		cursor->saveReport = transferType<bool, string>(value);
	}
	else if (fieldName == "analog_curve")
	{
		oid_t analog_curve_id = transferType<oid_t, string>(value);

		dbCursor<AnalogCurveData> curveCursor;
		curveCursor.select();
		if (!curveCursor.isEmpty())
		{
			bool bFind = false;
			do 
			{
				if (curveCursor.currentId().getOid() == analog_curve_id)
				{
					cursor->analog_curve = curveCursor.currentId();
					bFind = true;
					break;
				}
			} while (curveCursor.next());
			if (!bFind)
			{
				cursor->analog_curve = 0;
			}
		}
		else
		{
			cursor->analog_curve = 0;
		}
	}
	else if (fieldName == "analog_formula")
	{
		string analog_formula_id = value;
		dbCursor<FormulaDefinition> formulaCursor;
		bool bRet = GetDbReference(formulaCursor, analog_formula_id);
		if (bRet)
		{
			cursor->analog_formula = formulaCursor.currentId();
		}
		else
		{
			cursor->analog_formula = 0;
		}
	}
}

DiscreteTable::DiscreteTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> DiscreteTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Discrete> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->measurementType << "," << cursor->unitMultiplier << "," << cursor->unitSymbol 
				<< "," << cursor->badReference << "," << cursor->estimatorReplaced << "," << cursor->failure 
				<< "," << cursor->oldData << "," << cursor->operatorBlocked << "," << cursor->oscillatory 
				<< "," << cursor->outOfRange << "," << cursor->overFlow << "," << cursor->source 
				<< "," << cursor->suspect << "," << cursor->test << "," << cursor->validity 
				<< "," << cursor->valueSource << "," << cursor->timeStamp << "," << cursor->sensorAccuracy 
				<< "," << cursor->saveSection << "," << cursor->lockFlag << "," << cursor->holdFlag 
				<< "," << cursor->psr_type << "," << cursor->psr_rid 
				<< "," << cursor->maxValue << "," << cursor->minValue 
				<< "," << cursor->reverse << "," << cursor->normalValue << "," << cursor->value 
				<< "," << cursor->ftuUnitId << "," << cursor->ftuPointId << "," << cursor->ftuVlDesc;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool DiscreteTable::insertData()
{
	Discrete info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.measurementType = 0;
	info.unitMultiplier = 0;
	info.unitSymbol = 0;
	info.badReference = false;
	info.estimatorReplaced = false;
	info.failure = false;
	info.oldData = false;
	info.operatorBlocked = false;
	info.oscillatory = false;
	info.outOfRange = false;
	info.overFlow = false;
	info.source = false;
	info.suspect = false;
	info.test = false;
	info.validity = false;
	info.sensorAccuracy = 0;
	info.saveSection = false;
	info.lockFlag = false;
	info.holdFlag = false;
	info.maxValue = 0;
	info.minValue = 0;
	info.reverse = false;
	info.normalValue = 0;
	info.value = 0;
	info.ftuUnitId = 0;
	info.ftuPointId = 0;

	insert(info);
	m_dbPtr->commit();
	return true;
}

bool DiscreteTable::deleteData( const string& mRID )
{
	dbCursor<Discrete> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool DiscreteTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Discrete> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void DiscreteTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 
		<< 19 << 20 << 21 << 22 << 36;
	//hideColumns << 2 << 3 << 4 << 5 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 
	//	<< 19 << 20 << 21 << 23 << 29 << 30 << 31 << 32 << 33 << 36;
}

void DiscreteTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<Discrete>& cursor = static_cast< dbCursor<Discrete>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "measurementType")
	{
		cursor->measurementType = transferType<int4, string>(value);
	}
	else if (fieldName == "maxValue")
	{
		cursor->maxValue = transferType<int4, string>(value);
	}
	else if (fieldName == "minValue")
	{
		cursor->minValue = transferType<int4, string>(value);
	}
	else if (fieldName == "normalValue")
	{
		cursor->normalValue = transferType<int4, string>(value);
	}
	else if (fieldName == "value")
	{
		cursor->value = transferType<int4, string>(value);
	}
	else if (fieldName == "ftuUnitId")
	{
		cursor->ftuUnitId = transferType<int4, string>(value);
	}
	else if (fieldName == "ftuPointId")
	{
		cursor->ftuPointId = transferType<int4, string>(value);
	}
	else if (fieldName == "psr_type")
	{
		cursor->psr_type = value;
	}
	else if (fieldName == "psr_rid")
	{
		cursor->psr_rid = value;
	}
}

AccumulatorTable::AccumulatorTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> AccumulatorTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Accumulator> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->measurementType << "," << cursor->unitMultiplier << "," << cursor->unitSymbol 
				<< "," << cursor->badReference << "," << cursor->estimatorReplaced << "," << cursor->failure 
				<< "," << cursor->oldData << "," << cursor->operatorBlocked << "," << cursor->oscillatory 
				<< "," << cursor->outOfRange << "," << cursor->overFlow << "," << cursor->source 
				<< "," << cursor->suspect << "," << cursor->test << "," << cursor->validity 
				<< "," << cursor->valueSource << "," << cursor->sensorAccuracy << "," << cursor->timeStamp 
				<< "," << cursor->saveSection << "," << cursor->lockFlag << "," << cursor->holdFlag 
				<< "," << cursor->psr_type << "," << cursor->psr_rid << "," << cursor->maxValue 
				<< "," << cursor->value << "," << cursor->ftuUnitId << "," << cursor->ftuPointId 
				<< "," << cursor->ftuVlDesc;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool AccumulatorTable::insertData()
{
	Accumulator info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.measurementType = 0;
	info.unitMultiplier = 0;
	info.unitSymbol = 0;
	info.badReference = false;
	info.estimatorReplaced = false;
	info.failure = false;
	info.oldData = false;
	info.operatorBlocked = false;
	info.oscillatory = false;
	info.outOfRange = false;
	info.overFlow = false;
	info.source = false;
	info.suspect = false;
	info.test = false;
	info.validity = false;
	info.sensorAccuracy = 0;
	info.saveSection = false;
	info.lockFlag = false;
	info.holdFlag = false;
	info.maxValue = 0;
	info.value = 0;
	info.ftuUnitId = 0;
	info.ftuPointId = 0;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool AccumulatorTable::deleteData( const string& mRID )
{
	dbCursor<Accumulator> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool AccumulatorTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Accumulator> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void AccumulatorTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 
		<< 19 << 20 << 21 << 22 << 33;
}

void AccumulatorTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<Accumulator>& cursor = static_cast< dbCursor<Accumulator>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "maxValue")
	{
		cursor->maxValue = transferType<int4, string>(value);
	}
	else if (fieldName == "value")
	{
		cursor->value = transferType<int4, string>(value);
	}
	else if (fieldName == "ftuUnitId")
	{
		cursor->ftuUnitId = transferType<int4, string>(value);
	}
	else if (fieldName == "ftuPointId")
	{
		cursor->ftuPointId = transferType<int4, string>(value);
	}
}

CommandTable::CommandTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{

}

QList<QStringList> CommandTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Command> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->timeStamp << "," << cursor->operationInProgress << "," << cursor->bilaFlag 
				<< "," << cursor->psr_rid << "," << cursor->psr_type << "," << cursor->ftuUnitId 
				<< "," << cursor->ftuPointId << "," << cursor->ftuVlDesc << "," << cursor->directControl;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool CommandTable::insertData()
{
	Command info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.operationInProgress = false;
	info.bilaFlag = 1;
	info.ftuUnitId = 0;
	info.ftuPointId = 0;
	info.directControl = false;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool CommandTable::deleteData( const string& mRID )
{
	dbCursor<Command> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool CommandTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Command> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void CommandTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5 << 6 << 7;
}

void CommandTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<Command>& cursor = static_cast< dbCursor<Command>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "operationInProgress")
	{
		cursor->operationInProgress = transferType<bool, string>(value);
	}
	else if (fieldName == "bilaFlag")
	{
		cursor->bilaFlag = transferType<int, string>(value);
	}
	else if (fieldName == "psr_type")
	{
		cursor->psr_type = (value);
	}
	else if (fieldName == "psr_rid")
	{
		cursor->psr_rid = (value);
	}
	else if (fieldName == "ftuUnitId")
	{
		cursor->ftuUnitId = transferType<int4, string>(value);
	}
	else if (fieldName == "ftuPointId")
	{
		cursor->ftuPointId = transferType<int4, string>(value);
	}
}

AnalogCurveDataTable::AnalogCurveDataTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> AnalogCurveDataTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<AnalogCurveData> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->date.year() << "-" << cursor->date.month() << "-" << cursor->date.day();
			str << SPLITER_TOKEN << cursor->date.asTime_t();
			str << SPLITER_TOKEN << cursor->analog.getOid();
			str << SPLITER_TOKEN;
			if (cursor->pointValues.length() > 0)
			{
				for (int i = 0; i < cursor->pointValues.length(); ++i)
				{
					if (i != 0)
					{
						str << "/";
					}
					str << "(" << cursor->pointValues.getat(i).isValid << ";" << cursor->pointValues.getat(i).value << ")";
				}
			}
			else
			{
				str << " ";
			}
			str << SPLITER_TOKEN << cursor.currentId().getOid();
			str << SPLITER_TOKEN << " " << SPLITER_TOKEN << " ";
			str << SPLITER_TOKEN << cursor->updateLimit;
			str << SPLITER_TOKEN << " " << SPLITER_TOKEN << cursor->maxValue.tm << SPLITER_TOKEN << cursor->maxValue.value;
			str << SPLITER_TOKEN << " " << SPLITER_TOKEN << cursor->minValue.tm << SPLITER_TOKEN << cursor->minValue.value;

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(QString().fromStdString(SPLITER_TOKEN));
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool AnalogCurveDataTable::insertData()
{
	AnalogCurveData info;
	dbDateTime db_tm(time(NULL));
	info.date = dbDateTime(db_tm.year(), db_tm.month(), db_tm.day());
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool AnalogCurveDataTable::deleteData( const string& mRID )
{
	dbCursor<AnalogCurveData> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool AnalogCurveDataTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<AnalogCurveData> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void AnalogCurveDataTable::getHidedColumns( QList<int>& hideColumns )
{

}

void AnalogCurveDataTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<AnalogCurveData>& cursor = static_cast< dbCursor<AnalogCurveData>& >(anyCursor);
	if (fieldName == "analog")
	{
		string analogId = value;
		dbCursor<Analog> analogCursor;
		bool bRet = GetDbReference(analogCursor, analogId);
		if (bRet)
		{
			cursor->analog = analogCursor.currentId();
		}
	}
}

PowerTransformerTable::PowerTransformerTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> PowerTransformerTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<PowerTransformer> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->psrType
				<< "," << cursor->ec_type << "," << cursor->ec_rid 
				<< "," << cursor->bmagSat << "," << cursor->magBaseU << "," << cursor->magSatFlux 
				<< "," << cursor->phases << "," << cursor->transfCoolingType << "," << cursor->transformerType 
				<< ",";
			int length = cursor->transformer_windings.length();
			if (length > 0)
			{
				for (int i = 0; i < length; ++i)
				{
					str << ((i == 0) ? "" : "/");
					str << cursor->transformer_windings[i].getOid();
				}
			}
			else
			{
				str << "";
			}
			str << "," << cursor.currentId().getOid();

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool PowerTransformerTable::insertData()
{
	PowerTransformer info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.bmagSat = 0.0;
	info.magBaseU = 0.0;
	info.magSatFlux = 0.0;
	info.phases = 0;
	info.transfCoolingType = 0;
	info.transformerType = 0;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool PowerTransformerTable::deleteData( const string& mRID )
{
	dbCursor<PowerTransformer> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool PowerTransformerTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<PowerTransformer> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void PowerTransformerTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5;
}

void PowerTransformerTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<PowerTransformer>& cursor = static_cast< dbCursor<PowerTransformer>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psrType")
	{
		cursor->psrType = value;
	}
	else if (fieldName == "ec_type")
	{
		cursor->ec_type = value;
	}
	else if (fieldName == "ec_rid")
	{
		cursor->ec_rid = value;
	}
	else if (fieldName == "bmagSat")
	{
		cursor->bmagSat = transferType<real8, string>(value);
	}
	else if (fieldName == "magBaseU")
	{
		cursor->magBaseU = transferType<real8, string>(value);
	}
	else if (fieldName == "magSatFlux")
	{
		cursor->magSatFlux = transferType<real8, string>(value);
	}
	else if (fieldName == "phases")
	{
		cursor->phases = transferType<int, string>(value);
	}
	else if (fieldName == "transfCoolingType")
	{
		cursor->transfCoolingType = transferType<int, string>(value);
	}
	else if (fieldName == "transformerType")
	{
		cursor->transformerType = transferType<int, string>(value);
	}
}

TransformerWindingTable::TransformerWindingTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> TransformerWindingTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<TransformerWinding> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->psrType 
				<< "," << cursor->ec_type << "," << cursor->ec_rid 
				<< "," << cursor->phase << "," << cursor->powerPoint << "," << cursor->base_voltage 
				<< "," << cursor->b << "," << cursor->b0 << "," << cursor->connectionType 
				<< "," << cursor->emergencyS << "," << cursor->g << "," << cursor->g0 
				<< "," << cursor->grounded << "," << cursor->insulationU << "," << cursor->r 
				<< "," << cursor->r0 << "," << cursor->ratedU << "," << cursor->ratedS << "," << cursor->rground 
				<< "," << cursor->shortTermS << "," << cursor->windingType << "," << cursor->x 
				<< "," << cursor->x0 << "," << cursor->xground << "," << cursor->power_tranformer.getOid();

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool TransformerWindingTable::insertData()
{
	TransformerWinding info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	info.phase = 0;
	info.powerPoint = false;
	info.b = 0.0;
	info.b0 = 0.0;
	info.connectionType = 0;
	info.emergencyS = 0.0;
	info.g = 0.0;
	info.g0 = 0.0;
	info.grounded = false;
	info.insulationU = 0.0;
	info.r = 0.0;
	info.r0 = 0.0;
	info.ratedU = 0.0;
	info.ratedS = 0.0;
	info.rground = 0.0;
	info.shortTermS = 0.0;
	info.windingType = 0;
	info.x = 0.0;
	info.x0 = 0.0;
	info.xground = 0.0;
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool TransformerWindingTable::deleteData( const string& mRID )
{
	dbCursor<TransformerWinding> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool TransformerWindingTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<TransformerWinding> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void TransformerWindingTable::getHidedColumns( QList<int>& hideColumns )
{
	hideColumns << 2 << 3 << 4 << 5 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 20 << 21 << 22 
		<< 23 << 24 << 25 << 27 << 28 << 29;
}

void TransformerWindingTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	dbCursor<TransformerWinding>& cursor = static_cast< dbCursor<TransformerWinding>& >(anyCursor);
	if (fieldName == "name")
	{
		cursor->name = value;
	}
	else if (fieldName == "psrType")
	{
		cursor->psrType = value;
	}
	else if (fieldName == "ec_type")
	{
		cursor->ec_type = value;
	}
	else if (fieldName == "ec_rid")
	{
		cursor->ec_rid = value;
	}
	else if (fieldName == "phase")
	{
		cursor->phase = transferType<int, string>(value);
	}
	else if (fieldName == "powerPoint")
	{
		cursor->powerPoint = transferType<bool, string>(value);
	}
	else if (fieldName == "windingType")
	{
		cursor->windingType = transferType<int, string>(value);
	}
	else if (fieldName == "power_tranformer")
	{
		string powerTransformerId = value;
		dbCursor<PowerTransformer> powerTransCursor;
		bool bRet = GetDbReference(powerTransCursor, powerTransformerId);
		if (bRet)
		{
			cursor->power_tranformer = powerTransCursor.currentId();
		}
	}
}

ConnectivityNodeTable::ConnectivityNodeTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{

}

QList<QStringList> ConnectivityNodeTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<ConnectivityNode> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor->localName << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->fixed_terminal 
				<< "," << cursor->ec_type << "," << cursor->ec_rid 
				<< ",";

			int length = cursor->terminals.length();
			if (length > 0)
			{
				for (int i = 0; i < length; ++i)
				{
					str << (i == 0 ? "" : "/");
					str << cursor->terminals[i].getOid();
				}
			}
			else
			{
				str << "";
			}
			str << "," << cursor.currentId().getOid() << "," << cursor->power_status;
			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool ConnectivityNodeTable::insertData()
{
	return true;
}

bool ConnectivityNodeTable::deleteData( const string& mRID )
{
	dbCursor<ConnectivityNode> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool ConnectivityNodeTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<ConnectivityNode> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void ConnectivityNodeTable::getHidedColumns( QList<int>& hideColumns )
{
	return;
}

void ConnectivityNodeTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	return;
}

TerminalTable::TerminalTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{

}

QList<QStringList> TerminalTable::selectDatas()
{
	QList<QStringList> result;

	dbCursor<Terminal> cursor;
	if (cursor.select() > 0)
	{
		do 
		{
			std::ostringstream str;
			str << cursor->mRID << "," << cursor->name << "," << cursor.currentId().getOid() << "," << cursor->pathName 
				<< "," << cursor->aliasName << "," << cursor->description 
				<< "," << cursor->ce_type << "," << cursor->ce_rid 
				<< "," << cursor->connectivity_node.getOid();

			QString value = QString().fromStdString(str.str());

			QStringList values = value.split(",");
			result.push_back(values);
		} while (cursor.next());
	}

	return result;
}

bool TerminalTable::insertData()
{
	return true;
}

bool TerminalTable::deleteData( const string& mRID )
{
	dbCursor<Terminal> cursor(dbCursorForUpdate);
	return deleteDataByCursor(cursor, mRID);
}

bool TerminalTable::updateDatas( const string& mRID, const QMap<QString,QString>& values )
{
	dbCursor<Terminal> cursor(dbCursorForUpdate);
	return updateDataByCursor(cursor, mRID, values);
}

void TerminalTable::getHidedColumns( QList<int>& hideColumns )
{
	return;
}

void TerminalTable::setFieldValue( dbAnyCursor& anyCursor, const string& fieldName, const string& value )
{
	return;
}
