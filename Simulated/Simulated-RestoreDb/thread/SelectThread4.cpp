
#include "SelectThread4.h"
#include "FastdbTableDefine.h"
#include "../common.h"

CSelectThread4::CSelectThread4(dbDatabase* db, QObject* parent /*= 0*/)
	: QThread(parent)
{
	m_db = db;
}

void CSelectThread4::run()
{
	QDateTime time1 = QDateTime::currentDateTime();

	m_db->attach();

	dbCursor<Substation> substationCursor;
	dbCursor<Line> lineCursor;
	dbCursor<Breaker> breakerCursor;
	
	int count = substationCursor.select();
	if (count > 0)
	{
		QDateTime startTime = QDateTime::currentDateTime();
		do 
		{
		} while (substationCursor.next());
		QDateTime endTime = QDateTime::currentDateTime();

		QString str = QStringLiteral("查询 %1 完成，条数: %2, 耗时: %3ms").arg("Substation")
					.arg(count).arg(startTime.msecsTo(endTime));
		emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
	}

	count = lineCursor.select();
	if (count > 0)
	{
		QDateTime startTime = QDateTime::currentDateTime();
		do 
		{
		} while (lineCursor.next());
		QDateTime endTime = QDateTime::currentDateTime();

		QString str = QStringLiteral("查询 %1 完成，条数: %2, 耗时: %3ms").arg("Line")
			.arg(count).arg(startTime.msecsTo(endTime));
		emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
	}

	count = breakerCursor.select();
	if (count > 0)
	{
		QDateTime startTime = QDateTime::currentDateTime();
		do 
		{
		} while (breakerCursor.next());
		QDateTime endTime = QDateTime::currentDateTime();

		QString str = QStringLiteral("查询 %1 完成，条数: %2, 耗时: %3ms").arg("Breaker")
			.arg(count).arg(startTime.msecsTo(endTime));
		emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
	}
	
	m_db->detach();

	QDateTime time2 = QDateTime::currentDateTime();
	QString str = QStringLiteral("线程4查询完成耗时: %1ms").arg(time1.msecsTo(time2));
	emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
}
