
#include <QtWidgets/QtWidgets>

#include "rdbTableDefine.h"

RdbStruct RdbTableFactory::_rdbStruct = RdbStruct();

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

