#include "rdbtable.h"
#include "FdbTableDefine.h"

#include <sstream>
#include <QtCore/QUuid>

using namespace std;

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

bool RemoteUnitTable::updateData(const string& mRID, const string& fieldName, const string& value )
{
	dbCursor<RemoteUnit> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID=", mRID;
	try 
	{
		if (cursor.select(query) == 1)
		{
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

bool AnalogUnitPointTable::updateData( const string& mRID, const string& fieldName, const string& value )
{
	dbCursor<AnalogUnitPoint> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID=", mRID;
	try 
	{
		if (cursor.select(query) == 1)
		{
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

bool DiscreteUnitPointTable::updateData( const string& mRID, const string& fieldName, const string& value )
{
	dbCursor<DiscreteUnitPoint> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID=", mRID;
	try 
	{
		if (cursor.select(query) == 1)
		{
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

bool ControlUnitPointTable::updateData( const string& mRID, const string& fieldName, const string& value )
{
	dbCursor<ControlUnitPoint> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID=", mRID;
	try 
	{
		if (cursor.select(query) == 1)
		{
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
//			str << cursor->mRID << "," << cursor->IEDID << "," << cursor->CommandID << "," << cursor->CommandName;

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
	return true;
}

bool SubstationTable::updateData( const string& mRID, const string& fieldName, const string& value )
{
	return true;
}

VoltageLevelTable::VoltageLevelTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> VoltageLevelTable::selectDatas()
{
	return QList<QStringList>();
}

bool VoltageLevelTable::insertData()
{
	VoltageLevel info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool VoltageLevelTable::deleteData( const string& mRID )
{
	return true;
}

bool VoltageLevelTable::updateData( const string& mRID, const string& fieldName, const string& value )
{
	return true;
}

AnalogTable::AnalogTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> AnalogTable::selectDatas()
{
	return QList<QStringList>();
}

bool AnalogTable::insertData()
{
	Analog info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool AnalogTable::deleteData( const string& mRID )
{
	return true;
}

bool AnalogTable::updateData( const string& mRID, const string& fieldName, const string& value )
{
	return true;
}

DiscreteTable::DiscreteTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> DiscreteTable::selectDatas()
{
	return QList<QStringList>();
}

bool DiscreteTable::insertData()
{
	Discrete info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool DiscreteTable::deleteData( const string& mRID )
{
	return true;
}

bool DiscreteTable::updateData( const string& mRID, const string& fieldName, const string& value )
{
	return true;
}

AccumulatorTable::AccumulatorTable( dbDatabase* dbPtr )
	: BaseTable(dbPtr)
{}

QList<QStringList> AccumulatorTable::selectDatas()
{
	return QList<QStringList>();
}

bool AccumulatorTable::insertData()
{
	Accumulator info;
	info.mRID = QUuid::createUuid().toString().toStdString();
	insert(info);
	m_dbPtr->commit();
	return true;
}

bool AccumulatorTable::deleteData( const string& mRID )
{
	return true;
}

bool AccumulatorTable::updateData( const string& mRID, const string& fieldName, const string& value )
{
	return true;
}
