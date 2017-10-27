#include "ExecThread.h"

CExecThread::CExecThread( QObject* parent /*= 0*/ )
	: QThread(parent), m_stop(false), m_mutex(QMutex::NonRecursive)
{
	m_sqlExecPtr = CSqlExecInstance::getSqlExecInstance();
}

void CExecThread::setStop( bool stop )
{
	m_stop = stop;
}

void CExecThread::run()
{
	emit outputOperInfo(getOperInfo(QStringLiteral("开始执行线程")));

	while(!m_stop)
	{
		selectRecord();

		QThread::sleep(1);
	}

	m_stop = false;
	emit outputOperInfo(getOperInfo(QStringLiteral("结束执行线程")));
}

bool CExecThread::selectRecord()
{
	//QMutexLocker locker(&m_mutex);

	if (m_sqlExecPtr.isNull())
	{
		emit outputOperInfo(getOperInfo(QStringLiteral("数据库查询指针为空，无法继续")));
		return false;
	}

	QString sql = "select * from city";
	QSqlQuery query;
	if (!m_sqlExecPtr->exec(sql))
	{
		QString error = QString(QStringLiteral("执行查询操作出错: %1")).arg(m_sqlExecPtr->getError());
		emit outputOperInfo(getOperInfo(error));
		return false;
	}

	emit outputOperInfo(getOperInfo(QStringLiteral("执行查询操作")));
	return true;
}

QString CExecThread::getOperInfo( const QString& text )
{
	return QString(QStringLiteral("线程Id(%1): %2\n")).arg((unsigned long)(QThread::currentThreadId())).arg(text);
}
