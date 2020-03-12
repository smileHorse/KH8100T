#ifndef SELECTTHREAD2_H
#define SELECTTHREAD2_H

#include <QtCore/QtCore>

class dbDatabase;

class CSelectThread2 : public QThread
{
	Q_OBJECT

public:
	CSelectThread2(dbDatabase* db, QObject* parent = 0);

protected:
	virtual void run();

signals:
	void outputThreadInfo(const QString&);

private:
	dbDatabase* m_db;
};
#endif