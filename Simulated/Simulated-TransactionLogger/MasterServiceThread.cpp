
#include "MasterServiceThread.h"
#include "RandomOperateDb.h"
#include "FastdbManager.h"
#include "common.h"

#include <string>

using namespace std;


MasterServiceThread::MasterServiceThread(QObject* parent)
	: QThread(parent), m_isStop(false), m_lastOperTime(QDateTime::currentDateTime()),
	m_lastChangeLoggerTime(QDateTime::currentDateTime())
{
	m_fastdbManager = FastdbManagerInstance::getFastdbManagerInstance();
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
		QMutexLocker locker(&m_mutex);

		// 每隔指定时间操作一次数据库
		if (isNeedDeal())
		{
			RandomOperateDb operDb;
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
	// 超过指定时间则需要处理
	QDateTime currTime = QDateTime::currentDateTime();
	if ((currTime.toTime_t() - m_lastOperTime.toTime_t()) > OPERATE_SPAN)
	{
		m_lastOperTime = currTime;
		return true;
	}
	return false;
}

