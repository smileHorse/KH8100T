#include "transferfileserver.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TransferFileServer w;
	w.show();
	return a.exec();
}
