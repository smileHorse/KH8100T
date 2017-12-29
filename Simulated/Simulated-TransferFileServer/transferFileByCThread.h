#ifndef TRANSFERFILEBYC_THREAD_H
#define TRANSFERFILEBYC_THREAD_H

#include <QtCore/QThread>

class TransferFileServer;

class TransferFileByCThread : public QThread
{
	Q_OBJECT

public:
	TransferFileByCThread(TransferFileServer* frame, const QString& filePath, QObject* parent = 0);

protected:
	virtual void run();

signals:
	void emitExecuteInfo(const QString& text);

private:
	TransferFileServer*	_frame;
	QString _filePath;
};
#endif