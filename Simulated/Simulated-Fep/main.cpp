#include "fepframe.h"
#include <QtWidgets/QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FepFrame frame(argc, argv);
	frame.show();

	QTimer::singleShot(1000, &frame, SLOT(startServer()));
	return a.exec();
}
