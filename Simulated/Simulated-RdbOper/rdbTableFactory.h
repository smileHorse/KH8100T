#ifndef RDBTABLEFACTORY_H
#define RDBTABLEFACTORY_H

#include <vector>
#include <QtCore/QtCore>
#include <string>
#include <vector>

#include "rdbTableDefine.h"
#include "xmlStreamReader.h"

using namespace std;
typedef vector<string> StringVector;

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

	// 获取实时库中表的字段信息
	static void getTableFields(const QString& tableName, RdbFieldStructList& fields);

	// 根据表名创建操作表的指针
	static bool createTablePtr(const QString& tableName, BaseTablePtr& tablePtr);

	// 从配置文件中读取已记录的mRID
	static void readTableMRIDs();

	// 将记录的mRID保存至配置文件
	static void saveTableMRIDs();

	// 插入新的mRID
	static void insertMRID(const QString& tableName, const string& mRID);

	// 获取指定表的所有mRID
	static QVector<string>& getTableMRIDs(const QString& tableName);

private:
	static RdbStruct	_rdbStruct;

	static QMap<QString, BaseTablePtr>	_tablePtrs;

	static QMap< QString, QVector<string> > _tableMRIDs;	// 所有表的mRID
};

#endif