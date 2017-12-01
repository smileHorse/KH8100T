#ifndef TRANSFERFILEITEM_H
#define TRANSFERFILEITEM_H

#include <QtCore/QtCore>

enum OperMode
{
	UploadFile,
	DownloadFile
};

class TransferFileItem : public QObject 
{
	Q_OBJECT

public:
	TransferFileItem(QObject* parent = 0)
		: QObject(parent)
	{}

	// 计算文件大小
	QString calcFileSize(long value);

	// 转换时间
	QString convertTime(long msec);

public:
	OperMode	operMode;
	QString		fileName;
	QString		fileSize;
	QString		transMode;
	QString		transTime;
};

#endif