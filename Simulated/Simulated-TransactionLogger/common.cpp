
#include "common.h"

const char* DatabaseName = "Test-Db";

const char* LoggerPathName = "./logger";

const int OPERATE_SPAN = 2;

QMutex	g_mutex;


QString getServiceType(ServiceType type)
{
	switch(type)
	{
	case MasterService:
		return QStringLiteral("主服务");
	case SlaveService:
		return QStringLiteral("备服务");
	case FrameService:
		return QStringLiteral("主框架");
	default:
		return QStringLiteral("未知服务");
	}
}

QString LoggerInfo::getLoggerInfo( QString text, ServiceType type )
{
	QTime currTime = QDateTime::currentDateTime().time();
	return QString("%1:%2:%3\t%4\t%5\n").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(getServiceType(type)).arg(text);
}
