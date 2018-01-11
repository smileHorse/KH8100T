
#include <QtWidgets/QtWidgets>

#include "mRIDXmlHandler.h"
#include "rdbTableFactory.h"

RdbStruct RdbTableFactory::_rdbStruct = RdbStruct();

QMap<QString, BaseTablePtr> RdbTableFactory::_tablePtrs = QMap<QString, BaseTablePtr>();

QMap< QString, QVector<string> > RdbTableFactory::_tableMRIDs = QMap< QString, QVector<string> >();

bool RdbTableFactory::initRdbStruct()
{
	if (!_rdbStruct.isEmpty())
	{
		return true;
	}

	XmlStreamReader reader;
	if(!reader.readFile("conf/rdbtable.xml", _rdbStruct))
	{
		QMessageBox::warning(0, QStringLiteral("初始实时库结构"), QStringLiteral("初始失败"));
		return false;
	}

	return true;
}

// 获取工厂个数
int RdbTableFactory::getTableCount()
{
	if(!initRdbStruct())
	{
		return 0;
	}
	return _rdbStruct.getTableCount();
}

// 获取工厂名
QString RdbTableFactory::getTableName(int index)
{
	if(!initRdbStruct())
	{
		return "";
	}

	return _rdbStruct.getTableName(index);
}

// 获取实时库中所有的表名
QStringList RdbTableFactory::getTableNames()
{
	if (!initRdbStruct())
	{
		return QStringList();
	}

	return _rdbStruct.getTableNames();
}

// 获取实时库中表的字段名
QStringList RdbTableFactory::getTableFields( const QString& tableName )
{
	if (!initRdbStruct())
	{
		return QStringList();
	}

	return _rdbStruct.getTableFields(tableName);
}

// 获取实时库中表的字段信息
void RdbTableFactory::getTableFields( const QString& tableName, RdbFieldStructList& fields )
{
	if (!initRdbStruct())
	{
		return;
	}

	_rdbStruct.getTableFields(tableName, fields);
}

// 根据表名创建操作表的指针
bool RdbTableFactory::createTablePtr( const QString& tableName, BaseTablePtr& tablePtr )
{
	tablePtr = BaseTablePtr(0);
	if (_tablePtrs.contains(tableName))
	{
		tablePtr = _tablePtrs[tableName];
		return true;
	}

	if (tableName == "GeographicalRegion")
	{
		tablePtr = GeographicalRegionTablePtr::create();
	}
	else if (tableName == "SubGeographicalRegion")
	{
		tablePtr = SubGeographicalRegionTablePtr::create();
	}
	else if (tableName == "Area")
	{
		tablePtr = AreaTablePtr::create();
	}
	else if (tableName == "Substation")
	{
		tablePtr = SubstationTablePtr::create();
	}
	else if (tableName == "BusbarSection")
	{
		tablePtr = BusbarSectionTablePtr::create();
	}
	else if (tableName == "Line")
	{
		tablePtr = LineTablePtr::create();
	}
	else if (tableName == "Breaker")
	{
		tablePtr = BreakerTablePtr::create();
	}
	else if (tableName == "Disconnector")
	{
		tablePtr = DisconnectorTablePtr::create();
	}
	else if (tableName == "PowerTransformer")
	{
		tablePtr = PowerTransformerTablePtr::create();
	}
	else if (tableName == "TransformerWinding")
	{
		tablePtr = TransformerWindingTablePtr::create();
	}
	else if (tableName == "RemoteUnit")
	{
		tablePtr = RemoteUnitTablePtr::create();
	}
	else if (tableName == "AnalogUnitPoint")
	{
		tablePtr = AnalogUnitPointTablePtr::create();
	}
	else if (tableName == "DiscreteUnitPoint")
	{
		tablePtr = DiscreteUnitPointTablePtr::create();
	}
	else if (tableName == "ControlUnitPoint")
	{
		tablePtr = ControlUnitPointTablePtr::create();
	}
	else if (tableName == "DevManufacturer")
	{
		tablePtr = DevManufacturerTablePtr::create();
	}
	else if (tableName == "DevDeviceType")
	{
		tablePtr = DevDeviceTypeTablePtr::create();
	}

	if (tablePtr.isNull())
	{
		return false;
	}

	_tablePtrs.insert(tableName, tablePtr);
	return true;
}

// 从配置文件中读取已记录的mRID
void RdbTableFactory::readTableMRIDs()
{
	MRIDXmlHandler xmlHandler;
	_tableMRIDs.clear();
	xmlHandler.readXml(_tableMRIDs);
}

// 将记录的mRID保存至配置文件
void RdbTableFactory::saveTableMRIDs()
{
	MRIDXmlHandler xmlHandler;
	xmlHandler.writeXml(_tableMRIDs);
}

// 插入新的mRID
void RdbTableFactory::insertMRID( const QString& tableName, const string& mRID )
{
	if (_tableMRIDs.contains(tableName))
	{
		_tableMRIDs[tableName].push_back(mRID);
	}
	else
	{
		QVector<string> mRIDs;
		mRIDs.push_back(mRID);
		_tableMRIDs.insert(tableName, mRIDs);
	}
}

// 获取指定表的所有mRID
QVector<string>& RdbTableFactory::getTableMRIDs( const QString& tableName )
{
	return _tableMRIDs[tableName];
}

