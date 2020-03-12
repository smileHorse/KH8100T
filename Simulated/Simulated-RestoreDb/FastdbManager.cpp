
#include "FastdbManager.h"
#include "common.h"
#include "FastdbTableDefine.h"
#include "thread/SelectThread1.h"
#include "thread/SelectThread2.h"
#include "thread/SelectThread3.h"
#include "thread/SelectThread4.h"

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

//bool FastdbManager::openDatabase(const string& dbName)
//{
//	if (m_db.isOpen())
//	{
//		return true;
//	}
//
//	if(!m_db.open(dbName.c_str()))
//	{
//		emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库打开失败"), FrameService));
//		return false;
//	}
//	m_dbName = dbName;
//	emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("实时库打开成功"), FrameService));
//
//	return true;
//}
bool FastdbManager::openDatabase(const string& dbName)
{
	emit outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("准备打开实时库"), FrameService));

	if (m_db.isOpen())
	{
		closeDatabase();
	}

	m_db.setConcurrency(0);
	if (!m_db.open(dbName.c_str(), NULL))
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

bool FastdbManager::backupDatabase()
{
	QMutexLocker lock(&g_mutex);

	QDateTime start = QDateTime::currentDateTime();
	QString fileSuffix = start.toString("yyyy-MM-dd hhmmss");
	string fileName = "backup - " + fileSuffix.toStdString() + ".fdb";
	try
	{
		m_db.backup(fileName.c_str(), false);
	}
	catch (const dbException& ex)
	{
		emit outputOperationInfo(QStringLiteral("backup时抛出异常:%1").arg(QString().fromStdString(ex.what())));
	}

	// 输出函数执行时间
	QDateTime end = QDateTime::currentDateTime();
	QString str = QString("backupDatabase ellapsed: %1 ms").arg(start.msecsTo(end));
	emit outputOperationInfo(str);
	return true;
}

bool FastdbManager::checkDatabase()
{
	QMutexLocker lock(&g_mutex);

	try
	{
		dbDatabase* db = &m_db;
		dbTableDescriptor* tables = db->getTables();
		for (dbTableDescriptor* desc= tables; desc != NULL; desc = desc->getNextTable())
		{ 
			dbAnyCursor cursor;
			cursor.setTable(desc);
			byte *record = dbMalloc(desc->size());
			memset(record, 0, desc->size());
			cursor.setRecord(record);
			int count = cursor.select();

			QDateTime startTime = QDateTime::currentDateTime();

			if (count > 0)
			{
				// 考虑到效率问题，每个实时库表最多查询1000条数据
				int InnerCount = 0;
				while (cursor.fetchNext())
				{
					if (++InnerCount > 100000)
					{
						break;
					}
				}
			}

			QDateTime endTime = QDateTime::currentDateTime();
			QString str = QStringLiteral("正在检测实时库表: %1,\t 数据个数: %2,\t 耗时: %3ms")
				.arg(desc->getName(), 30)
				.arg(count, 10)
				.arg(startTime.msecsTo(endTime));
			emit outputOperationInfo(str);

			dbFree(record);
		}
	}
	catch (const dbException& ex)
	{
		emit outputOperationInfo(QStringLiteral("check时抛出异常:%1").arg(QString().fromStdString(ex.what())));
	}

	return true;
}
//bool FastdbManager::checkDatabase()
//{
//	QMutexLocker lock(&g_mutex);
//
//	// 分为四个线程进行查询实时库操作
//	CSelectThread1* thread1 = new CSelectThread1(&m_db, this);
//	connect(thread1, SIGNAL(outputThreadInfo(const QString&)), this, SLOT(doThreadExec(const QString&)));
//	thread1->start();
//
//	CSelectThread2* thread2 = new CSelectThread2(&m_db, this);
//	connect(thread2, SIGNAL(outputThreadInfo(const QString&)), this, SLOT(doThreadExec(const QString&)));
//	thread2->start();
//
//	CSelectThread3* thread3 = new CSelectThread3(&m_db, this);
//	connect(thread3, SIGNAL(outputThreadInfo(const QString&)), this, SLOT(doThreadExec(const QString&)));
//	thread3->start();
//
//	CSelectThread4* thread4 = new CSelectThread4(&m_db, this);
//	connect(thread4, SIGNAL(outputThreadInfo(const QString&)), this, SLOT(doThreadExec(const QString&)));
//	thread4->start();
//
//	return true;
//}

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

void FastdbManager::doThreadExec(const QString& str)
{
	emit outputOperationInfo(str);
}
