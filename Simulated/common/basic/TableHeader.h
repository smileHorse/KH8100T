#ifndef TABLEHEADER_H
#define TABLEHEADER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#pragma execution_character_set("utf-8")

enum Headers 
{ 
	Header_Operation = 0,
	Header_OperTime,
	Header_Result,
	Header_Reason,
	Header_Count
};

class TableHeader
{
public:
	static QMap<Headers, QString> createTableHeaderLabels();
	//static QMap<Headers, QString> getTableHeaderLabels();
	static QStringList	getTableHeaderLabels();

private:
	static QMap<Headers, QString> headerLabels;
};

#endif