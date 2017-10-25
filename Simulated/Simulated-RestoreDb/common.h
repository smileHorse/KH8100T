#ifndef COMMON_H
#define COMMON_H

#include <QtCore/QtCore>

extern const char* DatabaseName;
extern const char* LoggerPathName;
extern const int OPERATE_SPAN;

extern QMutex	g_mutex;

enum ServiceType
{
	MasterService,
	SlaveService,
	FrameService,
	UnknownService
};


class LoggerInfo
{
public:
	static QString getLoggerInfo(QString text, ServiceType type);

};

#endif