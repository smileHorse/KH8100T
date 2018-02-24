
#include "dataStructXmlReader.h"
#include "structInsertThread.h"

bool DataStructXmlReader::loadDataStruct( DataStruct& dataStruct )
{
	QString fileName = "../conf/dataStruct.xml";
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//QMessageBox::warning(0, QStringLiteral("读取XML文件"), QStringLiteral("打开文件失败"));
		return false;
	}

	reader.setDevice(&file);
	reader.readNext();
	while(!reader.atEnd())
	{
		if (reader.isStartElement())
		{
			dataStruct.name = reader.name().toString();
			QXmlStreamAttributes attributes = reader.attributes();
			foreach(QXmlStreamAttribute attribute, attributes)
			{
				dataStruct.attributes.insert(attribute.name().toString(), attribute.value().toString());
			}

			readChildElement(dataStruct);
		}
		else
		{
			reader.readNext();
		}
	}

	file.close();
	if (reader.hasError())
	{
		/*QMessageBox::warning(0, QStringLiteral("解析xml文件"), 
		QStringLiteral("解析文件 %1 失败: %2").arg(fileName).arg(reader.errorString()));*/
		return false;
	}
	else if (file.error() != QFile::NoError)
	{
		/*QMessageBox::warning(0, QStringLiteral("解析xml文件"), 
		QStringLiteral("解析文件 %1 失败: %2").arg(fileName).arg(file.errorString()));*/
		return false;
	}
	return true;
}

void DataStructXmlReader::readChildElement( DataStruct& dataStruct )
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
			DataStruct childDataStruct;
			childDataStruct.name = reader.name().toString();
			QXmlStreamAttributes attributes = reader.attributes();
			foreach(QXmlStreamAttribute attribute, attributes)
			{
				childDataStruct.attributes.insert(attribute.name().toString(), attribute.value().toString());
			}

			readChildElement(childDataStruct);

			dataStruct.appendChildren(childDataStruct);
		}
		else if (reader.isCharacters())
		{
			dataStruct.value = reader.text().toString();
			reader.readNext();
		}
		else
		{
			reader.readNext();
		}
	}
}

