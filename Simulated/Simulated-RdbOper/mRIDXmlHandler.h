#ifndef MRIDXMLHANDLER_H
#define MRIDXMLHANDLER_H

#include <QtCore/QtCore>
#include <QtXml/QtXml>
#include <string>

using namespace std;

class MRIDXmlHandler
{
public:
	MRIDXmlHandler();

	bool writeXml(const QMap< QString, QVector<string> >& dataValues);

	bool readXml(QMap< QString, QVector<string> >& dataValues);

private:
	void writeTableXml(QXmlStreamWriter* xmlWriter, const QString& tableName, const QVector<string>& mRIDs);

	void readRdbElement(QXmlStreamReader* xmlReader, QMap< QString, QVector<string> >& dataValues);

	void readTableElement(QXmlStreamReader* xmlReader, QMap< QString, QVector<string> >& dataValues);

	void readMRIDElement(QXmlStreamReader* xmlReader, QVector<string>& mRIDs);

	void skipUnknownElement(QXmlStreamReader* xmlReader);
};
#endif