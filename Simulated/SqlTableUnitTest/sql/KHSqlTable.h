/********************************************************************
	created:	2017/10/24
	created:	24:10:2017   13:52
	filename: 	D:\KH-8100T\project\src\datasvr\opt\hisdb\KHSqlTable.h
	file path:	D:\KH-8100T\project\src\datasvr\opt\hisdb
	file base:	KHSqlTable
	file ext:	h
	author:		Masf
	
	purpose:	提供数据服务器访问历史库的通用功能
*********************************************************************/
#ifndef KHSQLTABLE_H
#define KHSQLTABLE_H

#include <string>
#include <QtCore/QtCore>
#include <QtSql/QtSql>

typedef QVector<int>			IntSeq;
typedef QVector<QString>		QStringSeq;
typedef QVector<QVariant>		QVariantSeq;
typedef QMap<QString, QVariant>	QStrQVarMap;
typedef QList<QStrQVarMap>		QStrQVarMapList;

using namespace std;

enum FieldOrder
{
	FieldOrder_Asc,
	FieldOrder_Desc
};

class CKHSqlTable
{
public:
	CKHSqlTable();
	virtual ~CKHSqlTable();

public:
	// 设置数据库连接的名称
	void setConnectionName(const QString& connectionName = "");

	// 获取错误信息
	string	getError();

	// 初始化数据表的列名
	virtual void initTableField() = 0;

	// 获取数据表名
	virtual QString	getTableName() = 0;

	// 获取数据表的列名
	QString	getTableField(int index);

	// 设置数据表的列名
	void setTableField(const QStringSeq& values);

		
protected:
	// 执行数据库操作
	bool exec(const QString& sql, const QVariantSeq& values, 
		QStrQVarMapList& records = QStrQVarMapList());

	// 获取数据表的uuid
	QString generateUUID();

	// 将string转换为QString
	QString	convertStdStrToQStr(const string& str);

	// 创建插入语句
	QString	makeInsertSql(int columnCount);

	// 创建删除语句
	QString	makeDeleteSql(const IntSeq& conditions = IntSeq());
	
	// 创建更新语句
	QString	makeUpdateSql(const IntSeq& sets, const IntSeq& conditions = IntSeq());
	
	// 创建查询语句
	QString	makeSelectSql(const IntSeq& selects = IntSeq(), const IntSeq& conditions = IntSeq(), 
		bool needOrder = false, int orderField = 0, FieldOrder order = FieldOrder_Asc, bool needLimit = false, 
		int lowLimit = 0, int highLimit = 1000);

private:

private:
	void clear();


private:
	QString		m_connectionName;

	QVector<QString>	m_columns;
	QString	m_error;
};

#endif