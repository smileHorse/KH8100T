
#include "YkDataManagerI.h"
#include "workstationserverthread.h"

#include <QtCore/QtCore>

YkFepManagerI::YkFepManagerI( WorkStationServerThread* threadPtr )
	: m_threadPtr(threadPtr)
{

}

void YkFepManagerI::select( const ::Yk::YkCommand& command, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1:%2:%3.%4 收到 %5 请求 %6\t%7").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("select").arg(command.requestId).arg(QString().fromStdString(command.rID));

	m_threadPtr->outputOperationData(text);

	m_threadPtr->selectEcho(command, true, "echo");
	m_threadPtr->selectReturn(command, true, "return");
}

void YkFepManagerI::execute( const ::Yk::YkCommand& command, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1:%2:%3.%4 收到 %5 请求 %6\t%7").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("execute").arg(command.requestId).arg(QString().fromStdString(command.rID));

	m_threadPtr->outputOperationData(text);
}

void YkFepManagerI::cancel( const ::Yk::YkCommand& command, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1:%2:%3.%4 收到 %5 请求 %6\t%7").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("cancel").arg(command.requestId).arg(QString().fromStdString(command.rID));

	m_threadPtr->outputOperationData(text);

}

YkAppManagerI::YkAppManagerI( WorkStationServerThread* threadPtr )
	: m_threadPtr(threadPtr)
{

}

void YkAppManagerI::selectEcho( const ::Yk::YkCommand& command, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QTime requestTime(command.unitNo, command.ykNo, command.yxIndex, 0);
	int secs = requestTime.secsTo(currTime);
	QString text = QString("%1:%2:%3.%4 收到 %5 响应 %6\t%7\t响应时间 %8").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("selectEcho").arg(command.requestId).arg(QString().fromStdString(command.rID)).
		arg(secs);
	m_threadPtr->outputOperationData(text);
}

void YkAppManagerI::selectReturn( const ::Yk::YkCommand& command, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QTime requestTime(command.unitNo, command.ykNo, command.yxIndex, 0);
	int secs = requestTime.secsTo(currTime);
	QString text = QString("%1:%2:%3.%4 收到 %5 响应 %6\t%7\t响应时间 %8").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("selectReturn").arg(command.requestId).arg(QString().fromStdString(command.rID)).
		arg(secs);
	m_threadPtr->outputOperationData(text);
}

void YkAppManagerI::executeEcho( const ::Yk::YkCommand& command, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1:%2:%3.%4 收到 %5 响应 %6\t%7").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("executeEcho").arg(command.requestId).arg(QString().fromStdString(command.rID));

	m_threadPtr->outputOperationData(text);
}

void YkAppManagerI::executeReturn( const ::Yk::YkCommand& command, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1:%2:%3.%4 收到 %5 响应 %6\t%7").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("executeReturn").arg(command.requestId).arg(QString().fromStdString(command.rID));

	m_threadPtr->outputOperationData(text);
}

void YkAppManagerI::cancelEcho( const ::Yk::YkCommand& command, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1:%2:%3.%4 收到 %5 响应 %6\t%7").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("cancelEcho").arg(command.requestId).arg(QString().fromStdString(command.rID));

	m_threadPtr->outputOperationData(text);
}

void YkAppManagerI::cancelReturn( const ::Yk::YkCommand& command, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1:%2:%3.%4 收到 %5 响应 %6\t%7").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("cancelReturn").arg(command.requestId).arg(QString().fromStdString(command.rID));

	m_threadPtr->outputOperationData(text);
}
