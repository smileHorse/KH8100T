
#include "MasterServiceThread.h"
#include "RandomOperateDb.h"
#include "common.h"


MasterServiceThread::MasterServiceThread(QObject* parent)
	: QThread(parent), m_isStop(false), m_lastOperTime(QDateTime::currentDateTime())
{
}

MasterServiceThread::~MasterServiceThread()
{
}

void MasterServiceThread::setStop( bool stop )
{
	m_isStop = stop;
}

void MasterServiceThread::close()
{
	m_db.close();
	tl.close();

	emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("关闭实时库和事务日志文件成功"), MasterService));
}

void MasterServiceThread::run()
{
	m_db.open(DatabaseName);
	tl.open(_T("testtl.log"), dbFile::truncate|dbFile::no_sync);
	m_db.setTransactionLogger(&tl);

	emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("打开实时库和事务日志文件成功"), MasterService));

	while(!m_isStop)
	{
		// 每隔指定时间操作一次数据库
		if (isNeedDeal())
		{
			RandomOperateDb operDb(&m_db);
			operDb.startOperate();

			emit outputOperationInfo(LoggerInfo::getLoggerInfo(operDb.getOperateInfo(), MasterService));
		}
	}

	m_isStop = false;
	close();
}

bool MasterServiceThread::isNeedDeal()
{
	QDateTime currTime = QDateTime::currentDateTime();
	if ((currTime.toTime_t() - m_lastOperTime.toTime_t()) > OPERATE_SPAN)
	{
		m_lastOperTime = currTime;
		return true;
	}
	return false;
}
