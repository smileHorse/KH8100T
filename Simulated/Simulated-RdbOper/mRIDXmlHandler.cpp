
#include <QtWidgets/QtWidgets>

#include "mRIDXmlHandler.h"

MRIDXmlHandler::MRIDXmlHandler()
{

}

bool MRIDXmlHandler::writeXml( const QMap< QString, QVector<string> >& dataValues )
{
	QString fileName = "./conf/tableMRID.xml";
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(0, QStringLiteral("存储实时库表的mRID"), QStringLiteral("打开文件失败"));
		return false;
	}

	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("rdb");
	QMapIterator<QString, QVector<string> > i(dataValues);
	while(i.hasNext())
	{
		i.next();
		writeTableXml(&xmlWriter, i.key(), i.value());
	}
	xmlWriter.writeEndDocument();
	file.close();
	if (file.error())
	{
		QMessageBox::warning(0, QStringLiteral("存储实时库表的mRID"), QStringLiteral("保存数据失败"));
		return false;
	}
	return true;
}

bool MRIDXmlHandler::readXml( QMap< QString, QVector<string> >& dataValues )
{
	QString fileName = "./conf/tableMRID.xml";
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(0, QStringLiteral("读取XML文件"), QStringLiteral("打开文件失败"));
		return false;
	}

	QXmlStreamReader xmlReader(&file);
	xmlReader.readNext();
	while(!xmlReader.atEnd())
	{
		if (xmlReader.isStartElement())
		{
			if (xmlReader.name() == "rdb")
			{
				readRdbElement(&xmlReader, dataValues);
			}
			else
			{
				xmlReader.raiseError(QObject::tr("Not a rdb file"));
			}
		}
		else
		{
			xmlReader.readNext();
		}
	}

	file.close();
	if (xmlReader.hasError())
	{
		QMessageBox::warning(0, QStringLiteral("解析xml文件"), 
			QStringLiteral("解析文件 %1 失败: %2").arg(fileName).arg(xmlReader.errorString()));
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

void MRIDXmlHandler::writeTableXml( QXmlStreamWriter* xmlWriter, const QString& tableName, const QVector<string>& mRIDs )
{
	xmlWriter->writeStartElement("table");
	xmlWriter->writeAttribute("tableName", tableName);
	if (!mRIDs.isEmpty())
	{
		foreach(const string& str, mRIDs)
		{
			xmlWriter->writeTextElement("mRID", QString().fromStdString(str));
		}
	}
	xmlWriter->writeEndElement();
}

void MRIDXmlHandler::readRdbElement( QXmlStreamReader* xmlReader, QMap< QString, QVector<string> >& dataValues )
{
	xmlReader->readNext();
	while(!xmlReader->atEnd())
	{
		if (xmlReader->isEndElement())
		{
			xmlReader->readNext();
			break;
		}

		if (xmlReader->isStartElement())
		{
			if (xmlReader->name() == "table")
			{
				readTableElement(xmlReader, dataValues);
			}
			else
			{
				skipUnknownElement(xmlReader);
			}
		}
		else
		{
			xmlReader->readNext();
		}
	}
}

void MRIDXmlHandler::readTableElement( QXmlStreamReader* xmlReader, QMap< QString, QVector<string> >& dataValues )
{
	QString tableName = xmlReader->attributes().value("tableName").toString();
	QVector<string> mRIDs;

	xmlReader->readNext();
	while(!xmlReader->atEnd())
	{
		if (xmlReader->isEndElement())
		{
			xmlReader->readNext();
			break;
		}

		if (xmlReader->isStartElement())
		{
			if (xmlReader->name() == "mRID")
			{
				readMRIDElement(xmlReader, mRIDs);
			}
			else
			{
				skipUnknownElement(xmlReader);
			}
		}
		else
		{
			xmlReader->readNext();
		}
	}

	dataValues.insert(tableName, mRIDs);
}

void MRIDXmlHandler::readMRIDElement(QXmlStreamReader* xmlReader, QVector<string>& mRIDs )
{
	string mRID = xmlReader->readElementText().toStdString();
	if (xmlReader->isEndElement())
	{
		xmlReader->readNext();
	}
	mRIDs.push_back(mRID);
}

void MRIDXmlHandler::skipUnknownElement(QXmlStreamReader* xmlReader)
{
	xmlReader->readNext();
	while(!xmlReader->atEnd())
	{
		if (xmlReader->isEndElement())
		{
			xmlReader->readNext();
			break;
		}

		if (xmlReader->isStartElement())
		{
			skipUnknownElement(xmlReader);
		}
		else
		{
			xmlReader->readNext();
		}
	}
}

