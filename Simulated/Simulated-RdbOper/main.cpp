
#include <QtWidgets/QApplication>

#include "logger.h"
#include "rdboperframe.h"

//初始化日志操作类
Logger CLogger::my_logger = CLogger::init("./logger/DataCenter.log4");

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RdbOperFrame w;
	w.show();
	return a.exec();
}
