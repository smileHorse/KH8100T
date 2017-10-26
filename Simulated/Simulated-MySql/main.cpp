#include "sqlframe.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SqlFrame w;
	w.show();
	return a.exec();
}
