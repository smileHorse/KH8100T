#include "TableHeader.h"

QMap<Headers, QString> TableHeader::headerLabels = TableHeader::createTableHeaderLabels();

QMap<Headers, QString> TableHeader::createTableHeaderLabels()
{
	QMap<Headers, QString> labels;
	labels.insert(Header_Operation, QStringLiteral("操作"));
	labels.insert(Header_OperTime, QStringLiteral("操作时间"));
	labels.insert(Header_Result, QStringLiteral("操作结果"));
	labels.insert(Header_Reason, QStringLiteral("失败原因"));

	return labels;
}

QStringList TableHeader::getTableHeaderLabels()
{
	QStringList list;
	list << "操作" << "操作时间" << "操作结果" << "失败原因";

	return list;
}

//QMap<Headers, QString> TableHeader::getTableHeaderLabels()
//{
//	return TableHeader::headerLabels;
//}
