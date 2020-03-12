#ifndef BATCHREQUESTDATA_THREAD_H
#define BATCHREQUESTDATA_THREAD_H

#include "rdbdata.h"
#include <QtCore/QThread>

class BatchRequestDataThread : public QThread {
	Q_OBJECT

public:
	BatchRequestDataThread(const RdbRealData::RdbRealDataRequestPrx& proxy, QObject* parent = 0);

protected:
	virtual void run();

private:
	RdbRealData::RdbRealDataRequestPrx	m_rdbRealDataRequestPrx;
};
#endif