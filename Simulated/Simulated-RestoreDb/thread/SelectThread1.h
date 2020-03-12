#ifndef SELECTTHREAD1_H
#define SELECTTHREAD1_H

#include <QtCore/QtCore>

class dbDatabase;

class CSelectThread1 : public QThread
{
	Q_OBJECT

public:
	CSelectThread1(dbDatabase* db, QObject* parent = 0);

protected:
	virtual void run();

signals:
	void outputThreadInfo(const QString&);

private:
	dbDatabase* m_db;
};
#endif