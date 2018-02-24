#ifndef DATASTRUCTXMLREADER_H
#define DATASTRUCTXMLREADER_H

#include <QtCore/QtCore>
#include <QtXml/QtXml>

class DataStruct;

class DataStructXmlReader
{
public:
	bool loadDataStruct(DataStruct& dataStruct);

private:
	void readChildElement(DataStruct& dataStruct);

private:
	QXmlStreamReader	reader;
};
#endif