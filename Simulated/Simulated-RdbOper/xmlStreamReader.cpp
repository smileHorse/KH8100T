
#include <QtWidgets/QtWidgets>

#include "xmlStreamReader.h"

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
			if (reader.name() == "field")
			{
				readFieldElement(tableStruct);
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

	rdbStruct.tables.push_back(tableStruct);
}

void XmlStreamReader::readFieldElement(RdbTableStruct& tableStruct)
{
	RdbFieldStruct fieldStruct;
	fieldStruct.fieldType = reader.attributes().value("type").toString();
	fieldStruct.fieldName = reader.readElementText();
	if (reader.isEndElement())
	{
		reader.readNext();
	}
	tableStruct.fields.push_back(fieldStruct);
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
