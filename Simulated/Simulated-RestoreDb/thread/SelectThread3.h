#ifndef SELECTTHREAD3_H
#define SELECTTHREAD3_H

#include <QtCore/QtCore>

class dbDatabase;

class CSelectThread3 : public QThread
{
	Q_OBJECT

public:
	CSelectThread3(dbDatabase* db, QObject* parent = 0);

protected:
	virtual void run();

signals:
	void outputThreadInfo(const QString&);

private:
	dbDatabase* m_db;
};
#endif