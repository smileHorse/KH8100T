
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

bool FastdbManager::openDatabase()
{
	if (m_db.isOpen())
	{
		return true;
	}

	if(!m_db.open(DatabaseName))
	{
		emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库打开失败"), FrameService));
		return false;
	}
	emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库打开成功"), FrameService));

	return true;
}

bool FastdbManager::reopenDatabase()
{
	if (m_db.isOpen())
	{
		m_db.close();
	}

	if(!m_db.open(DatabaseName))
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

	openDatabase();

	closeTransactionLogger();

	if (!m_tl.open(path.c_str(), flags, crc))
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
	if(!openDatabase())
	{
		return NULL;
	}

	return &m_db;
}

dbFileTransactionLogger* FastdbManager::getDbFileTransactionLogger()
{
	return &m_tl;
}
