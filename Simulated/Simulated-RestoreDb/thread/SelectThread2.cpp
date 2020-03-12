
#include "SelectThread2.h"
#include "FastdbTableDefine.h"
#include "../common.h"

CSelectThread2::CSelectThread2(dbDatabase* db, QObject* parent /*= 0*/)
	: QThread(parent)
{
	m_db = db;
}

void CSelectThread2::run()
{
	QDateTime time1 = QDateTime::currentDateTime();

	m_db->attach();

	dbCursor<DiscreteUnitPoint> discretePointCursor;
	dbCursor<Discrete> discreteCursor;
	
	int count = discretePointCursor.select();
	if (count > 0)
	{
		QDateTime startTime = QDateTime::currentDateTime();
		do 
		{
		} while (discretePointCursor.next());
		QDateTime endTime = QDateTime::currentDateTime();

		QString str = QStringLiteral("查询 %1 完成，条数: %2, 耗时: %3ms").arg("DiscreteUnitPoint")
					.arg(count).arg(startTime.msecsTo(endTime));
		emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
	}

	count = discreteCursor.select();
	if (count > 0)
	{
		QDateTime startTime = QDateTime::currentDateTime();
		do 
		{
		} while (discreteCursor.next());
		QDateTime endTime = QDateTime::currentDateTime();

		QString str = QStringLiteral("查询 %1 完成，条数: %2, 耗时: %3ms").arg("Discrete")
			.arg(count).arg(startTime.msecsTo(endTime));
		emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
	}
	
	m_db->detach();

	QDateTime time2 = QDateTime::currentDateTime();
	QString str = QStringLiteral("线程2查询完成耗时: %1ms").arg(time1.msecsTo(time2));
	emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
}
