
#include "workstationframe.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	WorkStationFrame frame(argc, argv);
	frame.show();
	
	return a.exec();
}
