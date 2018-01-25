#ifndef XMLSTREAMREADER_H
#define XMLSTREAMREADER_H

#include <QtCore/QtCore>
#include <QtXml/QtXml>


// 实时库表字段结构
struct RdbFieldStruct
{
	QString fieldName;
	QString	fieldValue;
};
typedef QList<RdbFieldStruct>	RdbFieldStructList;

// 实时库表数据
struct RdbDataStruct
{
	RdbFieldStructList fields;
};
typedef QVector<RdbDataStruct>	RdbDataStructVct;

// 实时库表结构
struct RdbTableStruct
{
	QString	name;
	RdbDataStructVct	datas;
	QVector<RdbFieldStruct>  fields;
};

// 实时库结构
class RdbStruct 
{
public:	
	RdbStruct();

	void addRdbTableStruct(const RdbTableStruct& table);

	int getTableCount() const;
	QString getTableName(int index) const;
	QStringList getTableNames() const;
	QStringList getTableFields(const QString& tableName) const;
	void getTableFields(const QString& tableName, RdbFieldStructList& fields) const;
	bool isEmpty() const;

public:
	QVector<RdbTableStruct>	tables;
};

class XmlStreamReader
{
public:
	bool readFile(const QString& fileName, RdbStruct& rdbStruct);

private:
	void readRdbElement(RdbStruct& rdbStruct);
	void readTableElement(RdbStruct& rdbStruct);
	void readDataElement(RdbTableStruct& tableStruct);
	void skipUnknownElement();

private:
	QXmlStreamReader	reader;
};
#endif