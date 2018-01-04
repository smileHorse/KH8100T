#ifndef XMLSTREAMREADER_H
#define XMLSTREAMREADER_H

#include <QtCore/QtCore>
#include <QtXml/QtXml>


// 实时库表字段结构
struct RdbFieldStruct
{
	QString fieldName;
	QString	fieldType;
};

// 实时库表结构
struct RdbTableStruct
{
	QString	name;
	QVector<RdbFieldStruct>	fields;
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
	bool isEmpty() const;

private:
	QVector<RdbTableStruct>	tables;
};

class XmlStreamReader
{
public:
	bool readFile(const QString& fileName, RdbStruct& rdbStruct);

private:
	void readRdbElement(RdbStruct& rdbStruct);
	void readTableElement(RdbStruct& rdbStruct);
	void readFieldElement(RdbTableStruct& tableStruct);
	void skipUnknownElement();

private:
	QXmlStreamReader	reader;
};
#endif