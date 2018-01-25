
#include <QtWidgets/QtWidgets>

#include "xmlStreamReader.h"


RdbStruct::RdbStruct()
{
	tables.clear();
}

void RdbStruct::addRdbTableStruct( const RdbTableStruct& table )
{
	tables.push_back(table);
}

int RdbStruct::getTableCount() const
{
	return tables.size();
}

QString RdbStruct::getTableName( int index ) const
{
	int tableCount = getTableCount();
	if (index < 0 || index >= tableCount)
	{
		throw "getTableName index out of range";
	}

	return tables[index].name;
}

QStringList RdbStruct::getTableNames() const
{
	QStringList list;
	foreach(const RdbTableStruct& table, tables)
	{
		list << table.name;
	}
	return list;
}

QStringList RdbStruct::getTableFields(const QString& tableName) const
{
	QStringList list;
	foreach(const RdbTableStruct& table, tables)
	{
		if (table.name == tableName)
		{
			foreach(const RdbFieldStruct& field, table.fields)
			{
				list << field.fieldName;
			}
			break;
		}
	}
	return list;
}

void RdbStruct::getTableFields( const QString& tableName, RdbFieldStructList& fields ) const
{
	foreach(const RdbTableStruct& table, tables)
	{
		if (table.name == tableName)
		{
			fields = table.fields.toList();
			break;
		}
	}
}

bool RdbStruct::isEmpty() const
{
	return tables.isEmpty();
}

bool XmlStreamReader::readFile( const QString& fileName, RdbStruct& rdbStruct )
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(0, QStringLiteral("读取XML文件"), QStringLiteral("打开文件失败"));
		return false;
	}

	reader.setDevice(&file);
	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isStartElement())
		{
			if (reader.name() == "rdb")
			{
				readRdbElement(rdbStruct);
			}
			else
			{
				reader.raiseError(QObject::tr("Not a rdb file"));
			}
		}
		else
		{
			reader.readNext();
		}
	}

	file.close();
	if (reader.hasError())
	{
		QMessageBox::warning(0, QStringLiteral("解析xml文件"), 
			QStringLiteral("解析文件 %1 失败: %2").arg(fileName).arg(reader.errorString()));
		return false;
	}
	else if (file.error() != QFile::NoError)
	{
		QMessageBox::warning(0, QStringLiteral("解析xml文件"), 
			QStringLiteral("解析文件 %1 失败: %2").arg(fileName).arg(file.errorString()));
		return false;
	}
	return true;
}

void XmlStreamReader::readRdbElement(RdbStruct& rdbStruct)
{
	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}

		if (reader.isStartElement())
		{
			if (reader.name() == "table")
			{
				readTableElement(rdbStruct);
			}
			else
			{
				skipUnknownElement();
			}
		}
		else
		{
			reader.readNext();
		}
	}
}

void XmlStreamReader::readTableElement(RdbStruct& rdbStruct)
{
	RdbTableStruct tableStruct;
	tableStruct.name = reader.attributes().value("name").toString();

	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}

		if (reader.isStartElement())
		{
			if (reader.name() == "data")
			{
				readDataElement(tableStruct);
			}
			else
			{
				skipUnknownElement();
			}
		}
		else
		{
			reader.readNext();
		}
	}

	rdbStruct.addRdbTableStruct(tableStruct);
}

void XmlStreamReader::readDataElement(RdbTableStruct& tableStruct)
{
	RdbDataStruct data;

	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}

		if (reader.isStartElement())
		{
			RdbFieldStruct fieldStruct;
			fieldStruct.fieldName = reader.name().toString();
			fieldStruct.fieldValue = reader.readElementText();

			data.fields.push_back(fieldStruct);
		}

		reader.readNext();
	}

	tableStruct.datas.push_back(data);
}

void XmlStreamReader::skipUnknownElement()
{
	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}

		if (reader.isStartElement())
		{
			skipUnknownElement();
		}
		else
		{
			reader.readNext();
		}
	}
}
