#include "restoredb.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RestoreDb w;
	w.show();
	return a.exec();
}
