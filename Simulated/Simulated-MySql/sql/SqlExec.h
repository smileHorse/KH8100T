#ifndef SQLEXEC_H
#define SQLEXEC_H

#include <QtSql/QtSql>
#include <QtCore/QtCore>

class CSqlExec;

class CSqlExecInstance
{
public:
	static QSharedPointer<CSqlExec>	getSqlExecInstance();

private:
	static QSharedPointer<CSqlExec>	sqlExecInstance;
};

class CSqlExec
{
friend class CSqlExecInstance;

private:
	CSqlExec();

public:
	// 打开数据库
	bool openDb();

	// 执行查询操作
	bool exec(QSqlQuery& query, const QString& sql);

	// 获取错误信息
	QString	getError();

private:
	QSqlDatabase	m_database;

	QMutex			m_mutex;
	QString			m_databaseName;
	QString			m_userName;
	QString			m_password;
	QString			m_hostname;
	int				m_port;
	QString			m_error;
};
#endif