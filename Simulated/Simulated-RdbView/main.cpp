#include "RdbViewFrame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RdbViewFrame w;
	w.show();
	return a.exec();
}
