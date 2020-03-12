#ifndef SELECTTHREAD4_H
#define SELECTTHREAD4_H

#include <QtCore/QtCore>

class dbDatabase;

class CSelectThread4 : public QThread
{
	Q_OBJECT

public:
	CSelectThread4(dbDatabase* db, QObject* parent = 0);

protected:
	virtual void run();

signals:
	void outputThreadInfo(const QString&);

private:
	dbDatabase* m_db;
};
#endif