
#include "DbTransactionLoggerThread.h"
#include "FastdbManager.h"
#include "common.h"

DbTransactionLoggerThread::DbTransactionLoggerThread( QObject* parent /*= 0*/ )
	: QThread(parent), m_isStop(false),
	m_lastChangeLoggerTime(QDateTime::currentDateTime())
{
	m_fastdbManager = FastdbManagerInstance::getFastdbManagerInstance();
}

DbTransactionLoggerThread::~DbTransactionLoggerThread()
{

}

void DbTransactionLoggerThread::setStop( bool stop /*= true*/ )
{
	m_isStop = stop;
}

void DbTransactionLoggerThread::run()
{
	// 初始运行时为实时库设置事务日志文件
	m_fastdbManager->setTransactionLogger(NULL);
	string loggerName = getTransLoggerName().toStdString();
	m_fastdbManager->setTransactionLogger(loggerName, dbFile::truncate|dbFile::no_sync);
	emit outputOperationInfo(QStringLiteral("设置事务日志文件:%1").arg(QString().fromStdString(loggerName)));

	while(!m_isStop)
	{
		// 每隔指定时间更换一次日志文件
		if (isNeedChangeTransLogger())
		{
			string loggerName = getTransLoggerName().toStdString();
			m_fastdbManager->setTransactionLogger(loggerName, dbFile::truncate|dbFile::no_sync);
			emit outputOperationInfo(QStringLiteral("设置事务日志文件:%1").arg(QString().fromStdString(loggerName)));
		}

		QThread::msleep(1);
	}

	m_fastdbManager->closeTransactionLogger();
	m_fastdbManager->setTransactionLogger(NULL);
}

bool DbTransactionLoggerThread::isNeedChangeTransLogger()
{
	// 超过指定时间则需要处理  1min
	QDateTime currTime = QDateTime::currentDateTime();
	if ((currTime.toTime_t() - m_lastChangeLoggerTime.toTime_t()) >= 10)
	{
		m_lastChangeLoggerTime = currTime;
		return true;
	}
	return false;
}

QString DbTransactionLoggerThread::getTransLoggerName()
{
	QTime time = m_lastChangeLoggerTime.time();
	return QString("%1/%2-%3-%4.log").arg(LoggerPathName).arg(time.hour()).arg(time.minute()).arg(time.second());
}
