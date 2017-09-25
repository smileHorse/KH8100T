#include "amsframe.h"
#include <QtWidgets/QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AmsFrame frame;
	frame.show();

	frame.setParam(argc, argv);
	QTimer::singleShot(1000, &frame, SLOT(startServer()));

	return a.exec();
}
