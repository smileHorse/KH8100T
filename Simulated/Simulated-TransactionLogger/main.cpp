#include "transactionframe.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TransactionFrame w;
	w.show();
	return a.exec();
}
