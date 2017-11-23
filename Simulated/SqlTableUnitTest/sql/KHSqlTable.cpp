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

#include "KHSqlTable.h"

CKHSqlTable::CKHSqlTable()
{
	clear();
}

CKHSqlTable::~CKHSqlTable()
{
	clear();
}

// 设置数据库连接的名称
void CKHSqlTable::setConnectionName( const QString& connectionName /*= ""*/ )
{
	m_connectionName = connectionName;
}

// 获取错误信息
std::string CKHSqlTable::getError() 
{
	return m_error.toStdString();
}

// 获取数据表的列名
QString CKHSqlTable::getTableField( int index )
{
	if (index < 0 || index > m_columns.size())
	{
		m_error = "index索引超出m_columns的范围";
		return "";
	}

	return m_columns.at(index);
}

// 设置数据表的列名
void CKHSqlTable::setTableField( const QVector<QString>& values )
{
	m_columns.clear();
	m_columns.reserve(values.size());
	m_columns = values;
}

// 执行数据库操作
bool CKHSqlTable::exec( const QString& sql, const QVector<QVariant>& values, QList< QMap<QString, QVariant> >& records)
{
	return true;
}

// 获取数据表的guid
QString CKHSqlTable::generateUUID()
{
	return "";
}


// 将string转换为QString
QString CKHSqlTable::convertStdStrToQStr( const string& str )
{
	return QString().fromStdString(str);
}

// 创建插入语句
QString CKHSqlTable::makeInsertSql(int columnCount)
{
	QString makeSql = "insert into " + getTableName() + " values (";
	for (int i = 0; i < columnCount; ++i)
	{
		makeSql += (i == 0 ? "" : ",");
		makeSql += "?";
	}
	makeSql += ")";

	return makeSql;
}

// 创建删除语句
QString CKHSqlTable::makeDeleteSql(const IntSeq& conditions )
{
	QString makeSql = "delete from " + getTableName();
	if (!conditions.isEmpty())
	{
		makeSql += " where ";
		for (int i = 0; i < conditions.size(); ++i)
		{
			makeSql += (i == 0 ? "" : " and ");
			makeSql += getTableField(conditions[i]) + " = ? ";
		}
	}

	return makeSql;
}

// 创建更新语句
QString CKHSqlTable::makeUpdateSql( const IntSeq& sets, const IntSeq& conditions )
{
	QString makeSql = "update " + getTableName() + " set ";

	for (int i = 0; i < sets.size(); ++i)
	{
		makeSql += (i == 0 ? "" : ", ");
		makeSql += getTableField(sets[i]) + " = ? ";
	}

	if (!conditions.isEmpty())
	{
		makeSql += " where ";
		for (int i = 0; i < conditions.size(); ++i)
		{
			makeSql += (i == 0 ? "" : " and ");
			makeSql += getTableField(conditions[i]) + " = ? ";
		}
	}

	return makeSql;
}

// 创建查询语句
QString CKHSqlTable::makeSelectSql( const IntSeq& selects /*= IntSeq()*/, const IntSeq& conditions /*= IntSeq()*/, 
	bool needOrder /*= false*/,  int orderField /*= 0*/, FieldOrder order /*= FieldOrder_Asc*/, 
	bool needLimit /*= false*/, int lowLimit /*= 0*/, int highLimit /*= 1000*/ )
{
	QString makeSql = "select ";

	if (selects.isEmpty())
	{
		makeSql += " * ";
	}
	else
	{
		for (int i = 0; i < selects.size(); ++i)
		{
			makeSql += (i == 0 ? "" : ", ");
			makeSql += getTableField(selects[i]);
		}
	}

	makeSql += " from " + getTableName();

	if (!conditions.isEmpty())
	{
		makeSql += " where ";
		for (int i = 0; i < conditions.size(); ++i)
		{
			makeSql += (i == 0 ? "" : " and ");
			makeSql += getTableField(conditions[i]) + " = ? ";
		}
	}

	if (needOrder)
	{
		makeSql += " order by " + getTableField(orderField);
		makeSql += ((order == FieldOrder_Asc) ? " asc " : " desc "); 
	}

	if (needLimit)
	{
		makeSql += "limit ";
		makeSql += QString("%1").arg(lowLimit) + ", " + QString("%1").arg(highLimit);
	}

	return makeSql;
}

void CKHSqlTable::clear()
{
	m_columns.clear();
	m_error.clear();
}
