
#include "SelectThread3.h"
#include "FastdbTableDefine.h"
#include "../common.h"

CSelectThread3::CSelectThread3(dbDatabase* db, QObject* parent /*= 0*/)
	: QThread(parent)
{
	m_db = db;
}

void CSelectThread3::run()
{
	QDateTime time1 = QDateTime::currentDateTime();

	m_db->attach();

	dbCursor<AccumulatorUnitPoint> accumulatorPointCursor;
	dbCursor<Accumulator> accumulatorCursor;
	
	int count = accumulatorPointCursor.select();
	if (count > 0)
	{
		QDateTime startTime = QDateTime::currentDateTime();
		do 
		{
		} while (accumulatorPointCursor.next());
		QDateTime endTime = QDateTime::currentDateTime();

		QString str = QStringLiteral("查询 %1 完成，条数: %2, 耗时: %3ms").arg("AccumulatorUnitPoint")
					.arg(count).arg(startTime.msecsTo(endTime));
		emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
	}

	count = accumulatorCursor.select();
	if (count > 0)
	{
		QDateTime startTime = QDateTime::currentDateTime();
		do 
		{
		} while (accumulatorCursor.next());
		QDateTime endTime = QDateTime::currentDateTime();

		QString str = QStringLiteral("查询 %1 完成，条数: %2, 耗时: %3ms").arg("Accumulator")
			.arg(count).arg(startTime.msecsTo(endTime));
		emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
	}
	
	m_db->detach();

	QDateTime time2 = QDateTime::currentDateTime();
	QString str = QStringLiteral("线程3查询完成耗时: %1ms").arg(time1.msecsTo(time2));
	emit outputThreadInfo(LoggerInfo::getLoggerInfo(str, FrameService));
}
