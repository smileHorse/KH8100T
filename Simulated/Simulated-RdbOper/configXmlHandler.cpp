
#include <QtWidgets/QtWidgets>
#include "configXmlHandler.h"

#define CONFIG_FILEPATH "../conf/config.xml"

ConfigXmlHandler::ConfigXmlHandler()
{

}

bool ConfigXmlHandler::writeXml( const QMap<QString, QVector<QString> >& dataValues )
{
	QString fileName = CONFIG_FILEPATH;
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(0, QStringLiteral("配置文件"), QStringLiteral("打开文件失败"));
		return false;
	}

	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("config");
	QMapIterator<QString, QVector<QString> > i(dataValues);
	while(i.hasNext())
	{
		i.next();
		writeNodeXml(&xmlWriter, i.key(), i.value());
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

bool ConfigXmlHandler::readXml( QMap<QString, QVector<QString> >& dataValues )
{
	QString fileName = CONFIG_FILEPATH;
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
			if (xmlReader.name() == "config")
			{
				readConfigElement(&xmlReader, dataValues);
			}
			else
			{
				xmlReader.raiseError(QObject::tr("Not a config file"));
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

void ConfigXmlHandler::writeNodeXml( QXmlStreamWriter* xmlWriter, const QString& nodeName, const QVector<QString>& values )
{
	xmlWriter->writeStartElement(nodeName);
	QString ip = (values.size() >= 1) ? values[0] : "";
	QString port = (values.size() >= 2) ? values[1] : "";
	xmlWriter->writeTextElement("ip", ip);
	xmlWriter->writeTextElement("port", port);
	xmlWriter->writeEndElement();
}

void ConfigXmlHandler::readConfigElement( QXmlStreamReader* xmlReader, QMap<QString, QVector<QString> >& dataValues )
{
	xmlReader->readNext();
	while(!xmlReader->atEnd()) 
	{
		if (xmlReader->isEndElement())
		{
			xmlReader->readNext();
			break;
		}

		QVector<QString> values;
		if (xmlReader->isStartElement())
		{
			if(xmlReader->name() == "self-ice" 
				|| xmlReader->name() == "off-ice" 
				|| xmlReader->name() == "ice-storm")
			{
				readNodeElement(xmlReader, xmlReader->name().toString(), values);
				dataValues.insert(xmlReader->name().toString(), values);
				xmlReader->readNext();
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

void ConfigXmlHandler::readNodeElement( QXmlStreamReader* xmlReader, QString nodeName, QVector<QString>& values )
{
	xmlReader->readNext();
	while(!xmlReader->atEnd()) 
	{
		QString name = xmlReader->name().toString();
		QString token = xmlReader->tokenString();
		QXmlStreamReader::TokenType type = xmlReader->tokenType();
		if (xmlReader->isStartElement())
		{
			QString text = xmlReader->readElementText();
			if (name == "ip" || name == "port")
			{
				values.push_back(text);
				xmlReader->readNext();
			}
		}
		else if (xmlReader->isEndElement())
		{
			break;
		} 
		else
		{
			xmlReader->readNext();
		}
	}
}

void ConfigXmlHandler::skipUnknownElement( QXmlStreamReader* reader )
{

}
