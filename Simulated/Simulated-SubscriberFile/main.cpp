#include "subscriberfileframe.h"
#include "transferfileserver.h"

#include <QtCore/QtCore>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SubscriberFileFrame w;
	w.show();

	QSharedPointer<QThread> thread(new QThread);
	QSharedPointer<TransferFileServer> server(new TransferFileServer(argc, argv));
	server->moveToThread(thread.data());
	QObject::connect(thread.data(), SIGNAL(started()), server.data(), SLOT(start()));
	QObject::connect(server.data(), SIGNAL(outputOperationInfo(QString)), &w, SLOT(updateTextEdit(QString)));
	thread->start();
	w.setTransFileServer(server);

	return a.exec();
}
