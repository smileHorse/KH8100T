#include "fileTransferFrame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FileTransferFrame frame(argc, argv);
	frame.show();
	return a.exec();
}
