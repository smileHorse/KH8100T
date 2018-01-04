#ifndef RDBTABLEDEFINE_H
#define RDBTABLEDEFINE_H

#include <QtCore/QtCore>

#include "xmlStreamReader.h"

// 实时库表工厂类
class RdbTableFactory
{
public:
	// 初始化实时库结构
	static bool initRdbStruct();

	// 获取实时库中表的个数
	static int getTableCount();

	// 获取实时库中表名
	static QString getTableName(int index);

	// 获取实时库中所有的表名
	static QStringList getTableNames();

	// 获取实时库中表的字段名
	static QStringList getTableFields(const QString& tableName);

private:
	static RdbStruct	_rdbStruct;
};

#endif