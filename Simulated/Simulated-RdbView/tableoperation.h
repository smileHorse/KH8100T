#ifndef TABLEOPERATION_H
#define TABLEOPERATION_H

#include "fastdb.h"
#include "rdbtable.h"

#include <QtCore/QtCore>

class TableOperation
{
public:
	TableOperation(dbDatabase* dbPtr);

	// 根据表名获取操作表的指针
	bool	getTablePtrByTableName(const QString& tableName);

	// 获取表的字段名
	QStringList getTableFieldNames(const QString& tableName);

	// 获取表的数据
	QList<QStringList>	selectDatas(const QString& tableName);

	// 插入新的数据
	bool	insertData(const QString& tableName);

	// 删除指定的数据
	bool	deleteData(const QString& tableName, const QString& mRID);

	// 修改指定的数据
	bool	updateData();

private:

	
private:
	dbDatabase*	m_dbPtr;
	BaseTable*	m_tablePtr;
};
#endif