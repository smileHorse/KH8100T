#include "simulatedloganalyse.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SimulatedLogAnalyse w;
	w.show();
	return a.exec();
}
