
#include "FastdbManager.h"
#include "common.h"

QMutex	m_mutex(QMutex::NonRecursive);

FastdbManager* FastdbManagerInstance::getFastdbManagerInstance()
{
	if (m_fastdbManager)
	{
		return m_fastdbManager;
	}

	m_fastdbManager = new FastdbManager();
	return m_fastdbManager;
}

void FastdbManagerInstance::releaseFastdbManagerInstance()
{
	if (m_fastdbManager)
	{
		delete m_fastdbManager;
		m_fastdbManager = NULL;
	}
}

FastdbManager*	FastdbManagerInstance::m_fastdbManager = NULL;

FastdbManager::FastdbManager(QObject* parent)
	: QObject(parent), m_bSetTransactionLogger(false)
{

}

FastdbManager::~FastdbManager()
{

}

bool FastdbManager::openDatabase(const string& dbName)
{
	if (m_db.isOpen())
	{
		return true;
	}

	if(!m_db.open(dbName.c_str()))
	{
		emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库打开失败"), FrameService));
		return false;
	}
	m_dbName = dbName;
	emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库打开成功"), FrameService));

	return true;
}

bool FastdbManager::reopenDatabase(const string& dbName)
{
	if (m_db.isOpen())
	{
		m_db.close();
	}

	if(!m_db.open(dbName.c_str()))
	{
		emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库打开失败"), FrameService));
		return false;
	}
	emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库打开成功"), FrameService));

	return true;
}

bool FastdbManager::closeDatabase()
{
	if (!m_db.isOpen())
	{
		emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库已关闭"), FrameService));
		return true;
	}

	m_db.close();
	emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库关闭成功"), FrameService));

	return true;
}

void FastdbManager::setTransactionLogger( dbTransactionLogger* logger )
{
	m_db.setTransactionLogger(logger);

	m_bSetTransactionLogger = logger != NULL;
}

void FastdbManager::setTransactionLogger(const string& path, int flags, bool crc)
{
	QMutexLocker lock(&g_mutex);

	openDatabase(m_dbName);

	closeTransactionLogger();

	if (!openTransactionLogger(path.c_str(), flags, crc))
	{
		setTransactionLogger(NULL);
		return;
	}

	setTransactionLogger(&m_tl);
}

bool FastdbManager::openTransactionLogger( const string& path, int flags /*= dbFile::no_sync*/, bool crc /*= true*/ )
{
	return m_tl.open(path.c_str(), flags, crc);
}

bool FastdbManager::closeTransactionLogger()
{
	if (hasSetTransactionLogger())
	{
		m_tl.close();
	}

	return true;
}

bool FastdbManager::restoreDatabase( const string& logger )
{
	// 通过事务日志恢复实时库时，必须关闭实时库，并重新打开，才能进行后续恢复操作
	if(!openDatabase(m_dbName))
	{
		return false;
	}

	if (!openTransactionLogger(logger, dbFile::read_only))
	{
		return false;
	}

	size_t n;
	dbFileTransactionLogger::RestoreStatus status = m_tl.restore(m_db, n);
	if (status == dbFileTransactionLogger::rsOK)
	{
		m_tl.close();
		emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("恢复实时库成功"), FrameService));
		return true;
	}
	else
	{
		m_tl.rollback();
		emit outputOperationInfo(LoggerInfo::getLoggerInfo(
			QStringLiteral("恢复实时库失败:%1").arg(QString().fromStdString(getRestoreStatus(status))), 
			FrameService));
		return false;
	}
}

void FastdbManager::commit()
{	
	QMutexLocker lock(&g_mutex);

	try
	{
		m_db.commit();
	}
	catch (const dbException& ex)
	{
		emit outputOperationInfo(QStringLiteral("commit时抛出异常:%1").arg(QString().fromStdString(ex.what())));
	}
}

bool FastdbManager::hasSetTransactionLogger()
{
	return m_bSetTransactionLogger;
}

dbDatabase* FastdbManager::getDbDatabase()
{
	if(!openDatabase(m_dbName))
	{
		return NULL;
	}

	return &m_db;
}

dbFileTransactionLogger* FastdbManager::getDbFileTransactionLogger()
{
	return &m_tl;
}

string FastdbManager::getRestoreStatus( dbFileTransactionLogger::RestoreStatus status )
{
	switch(status)
	{
	case dbFileTransactionLogger::rsOK:
		return "rsOK";
	case dbFileTransactionLogger::rsCRCMismatch:
		return "rsCRCMismatch";
	case dbFileTransactionLogger::rsReadFailed:
		return "rsReadFailed";
	case dbFileTransactionLogger::rsTableNotFound:
		return "rsTableNotFound";
	case  dbFileTransactionLogger::rsOIDMismatch:
		return "rsOIDMismatch";
	default:
		return "Unknown";
	}
}
