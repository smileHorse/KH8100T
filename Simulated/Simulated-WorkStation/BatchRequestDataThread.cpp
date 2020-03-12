
#include "BatchRequestDataThread.h"
#include "RequestDataDialog.h"

#include <IceUtil/IceUtil.h>

BatchRequestDataThread::BatchRequestDataThread(const RdbRealData::RdbRealDataRequestPrx& proxy, 
	QObject* parent /*= 0*/)
{
	m_rdbRealDataRequestPrx = proxy;
}

void BatchRequestDataThread::run()
{
	try 
	{
		while(true)
		{

			// 循环处理所有的请求
			QList<RdbRealData::RequestCompleteDataSeq> requestSeq = CRequestManager::getBatchCompleteDataSeq();
			Q_FOREACH(RdbRealData::RequestCompleteDataSeq request, requestSeq)
			{
				m_rdbRealDataRequestPrx->RequestCompleteData(request);
				if (request.isStop)
				{
					CRequestManager::removeBatchCompleteDataSeq(request.requestId);
				}
			}

			IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(3));
		}
	}
	catch(const Ice::Exception& ex)
	{
		string error = ex.what();
	}	
}
