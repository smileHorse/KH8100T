
#include "MasterServiceThread.h"
#include "RandomOperateDb.h"
#include "common.h"


MasterServiceThread::MasterServiceThread(dbDatabase* dbPtr, QObject* parent)
	: QThread(parent), m_dbPtr(dbPtr), m_isStop(false), m_lastOperTime(QDateTime::currentDateTime())
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
}

void MasterServiceThread::run()
{
	while(!m_isStop)
	{
		// 每隔指定时间操作一次数据库
		if (isNeedDeal())
		{
			RandomOperateDb operDb(m_dbPtr);
			QString threadId = QString("%1").arg((DWORD)(QThread::currentThreadId()));
			operDb.setThreadId(threadId);
			operDb.startOperate();

			emit outputOperationInfo(LoggerInfo::getLoggerInfo(operDb.getOperateInfo(), MasterService));
		}

		QThread::msleep(1);
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
