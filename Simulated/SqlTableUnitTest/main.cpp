
#include <QtCore/QCoreApplication>

#include "sql/DayReportTable.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CDayReportTable table;
	table.unitTest();

	return a.exec();
}
