#ifndef CONFIGXMLHANDLER_H
#define CONFIGXMLHANDLER_H

#include <QtCore/QtCore>
#include <QtXml/QtXml>
#include <string>

using namespace std;

class ConfigXmlHandler 
{
public:
	ConfigXmlHandler();

	bool writeXml(const QMap<QString, QVector<QString> >& dataValues);

	bool readXml(QMap<QString, QVector<QString> >& dataValues);

private:
	void writeNodeXml(QXmlStreamWriter* xmlWriter, const QString& nodeName, const QVector<QString>& valus);

	void readConfigElement(QXmlStreamReader* xmlReader, QMap<QString, QVector<QString> >& dataValues);

	void readNodeElement(QXmlStreamReader* xmlReader, QString nodeName, QVector<QString>& values);

	void skipUnknownElement(QXmlStreamReader* reader);
};
#endif