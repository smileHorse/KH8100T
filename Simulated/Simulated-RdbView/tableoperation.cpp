#include "tableoperation.h"
#include "FdbTableDefine.h"


TableOperation::TableOperation( dbDatabase* dbPtr )
	: m_dbPtr(dbPtr)
{

}

QStringList TableOperation::getTableFieldNames( const QString& tableName )
{
	if (!getTablePtrByTableName(tableName))
	{
		return QStringList();
	}

	return m_tablePtr->getTableFieldNames(tableName.toStdString());
}

QList<QStringList> TableOperation::selectDatas( const QString& tableName )
{
	if (!getTablePtrByTableName(tableName))
	{
		return QList<QStringList>();
	}

	return m_tablePtr->selectDatas();
}

bool TableOperation::getTablePtrByTableName( const QString& tableName )
{
	static QMap<QString, BaseTable*> tablePtrs;
	if (tablePtrs.isEmpty())
	{
		tablePtrs.insert("RemoteUnit",			new RemoteUnitTable(m_dbPtr));
		tablePtrs.insert("AnalogUnitPoint",			new AnalogUnitPointTable(m_dbPtr));
		tablePtrs.insert("DiscreteUnitPoint",			new DiscreteUnitPointTable(m_dbPtr));
		tablePtrs.insert("ControlUnitPoint",			new ControlUnitPointTable(m_dbPtr));
		tablePtrs.insert("Substation",			new SubstationTable(m_dbPtr));
		tablePtrs.insert("VoltageLevel",			new VoltageLevelTable(m_dbPtr));
		tablePtrs.insert("Analog",			new AnalogTable(m_dbPtr));
		tablePtrs.insert("Discrete",			new DiscreteTable(m_dbPtr));
		tablePtrs.insert("Accumulator",			new AccumulatorTable(m_dbPtr));
	}

	if (tablePtrs.contains(tableName))
	{
		m_tablePtr = tablePtrs.value(tableName);
		return true;
	}
	else
	{
		m_tablePtr = NULL;
		return false;
	}
}

bool TableOperation::insertData( const QString& tableName )
{
	if (!getTablePtrByTableName(tableName))
	{
		return false;
	}

	return m_tablePtr->insertData();
}

bool TableOperation::deleteData( const QString& tableName, const QString& mRID )
{
	if (!getTablePtrByTableName(tableName))
	{
		return false;
	}

	return m_tablePtr->deleteData(mRID.toStdString());
}
