#ifndef RANDOMINSERTTHREAD_H
#define RANDOMINSERTTHREAD_H

#include <QtCore/QtCore>
#include <string>

#include "rdbdata.h"

using namespace RdbRealData;
using namespace std;

class RandomInsertThread : public QThread
{
	Q_OBJECT

public:
	RandomInsertThread(QObject* parent = 0);

	void setStop(bool stop);
	void setTableName(const QString& tableName);
	void setCount(int count);
	void setRdbDataOptPrx(const RdbDataOptPrx& rdbDataOptPrx);

protected:
	virtual void run();

private:
	void getRandomDataValues(const QString& tableName, RdbRealData::Strings& dataValues);
	void logData(const QString& tableName, RdbRealData::Strings& dataValues);

	void readTableMRIDs();
	void saveTableMRIDs();

signals:
	void updateInsertCount(int value);
	void updateResultText(const QString& text);

private:
	bool			m_stop;
	QString			m_tableName;
	int				m_count;
	RdbDataOptPrx	m_rdbDataOptPrx;
};
#endif