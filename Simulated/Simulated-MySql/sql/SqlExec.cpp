
#include "SqlExec.h"

QSharedPointer<CSqlExec> CSqlExecInstance::sqlExecInstance = QSharedPointer<CSqlExec>(0);

QSharedPointer<CSqlExec> CSqlExecInstance::getSqlExecInstance()
{
	if (sqlExecInstance.isNull())
	{
		sqlExecInstance = QSharedPointer<CSqlExec>(new CSqlExec());
	}

	return sqlExecInstance;
}

CSqlExec::CSqlExec()
	: m_mutex(QMutex::NonRecursive)
{
	m_database = QSqlDatabase::addDatabase("QMYSQL");

	m_databaseName = "world";
	m_userName = "root";
	m_password = "root";
	m_hostname = "localhost";
	m_port = 3306;
}

bool CSqlExec::openDb()
{
	if (m_database.isOpen())
	{
		return true;
	}

	m_database.setDatabaseName(m_databaseName);
	m_database.setUserName(m_userName);
	m_database.setPassword(m_password);
	m_database.setHostName(m_hostname);
	m_database.setPort(m_port);
	if(!m_database.open())
	{
		m_error = m_database.lastError().text();
		return false;
	}

	return true;
}

// Ö´ÐÐ²éÑ¯²Ù×÷
bool CSqlExec::exec( const QString& sql )
{
	QMutexLocker locker(&m_mutex);

	if (!openDb())
	{
		return false;
	}

	QSqlQuery query;
	if(!query.prepare(sql))
	{
		m_error = query.lastError().text();
		return false;
	}

	if (!query.exec())
	{
		m_error = query.lastError().text();
		return false;
	}

	return true;
}

bool CSqlExec::exec(QSqlQuery& query, const QString& sql)
{
	QMutexLocker locker(&m_mutex);

	if (!openDb())
	{
		return false;
	}

	if(!query.prepare(sql))
	{
		m_error = query.lastError().text();
		return false;
	}

	if (!query.exec())
	{
		m_error = query.lastError().text();
		return false;
	}

	return true;
}


QString CSqlExec::getError()
{
	return m_error;
}
