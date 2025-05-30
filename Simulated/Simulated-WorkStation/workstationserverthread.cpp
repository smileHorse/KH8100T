
#include "workstationserverthread.h"
#include "BaseIceStorm.h"
#include "fepdata.h"
#include "FepDataManagerI.h"
#include "OperationInfo.h"
#include "RdbAlarmDataI.h"
#include "RdbRealDataI.h"
#include "transferfile.h"
#include "warningmsg.h"
#include "WarningMsgI.h"
#include "YkDataManagerI.h"
#include "RequestDataDialog.h"
#include "GlobalVariable.h"
#include "BatchRequestDataThread.h"

#include <string>

using namespace std;

template<class T> string transferToString(const T& value);

WorkStationServerThread::WorkStationServerThread( QObject* parent /*= 0*/ )
	: QThread(parent)
{
	m_rdbRequestSubIdentity = "rdbrequest-subscriber";
	m_rdbRespondSubIdentity = "rdbrespond-subscriber";
	m_alarmSubIdentity = "alarm-subscriber";
	m_fepDataSubIdentity = "fepdata-subscriber";
	m_ykFepSubIdentity = "ykfep-subscriber";
	m_ykAppSubIdentity = "ykapp-subscriber";
	m_warningMsgSubIdentity = "warningmsg-subscriber";

	m_batchRequestThread = 0;
}

void WorkStationServerThread::setCommunicatorPtr( Ice::CommunicatorPtr ptr )
{
	m_communicatorPtr = ptr;
}

void WorkStationServerThread::setObjectAdapterPtr( Ice::ObjectAdapterPtr ptr )
{
	m_objectAdapterPtr = ptr;
}

void WorkStationServerThread::outputOperationData( const QString& text )
{
	emit outputReceiveData(text);
}

// 遥控选择
void WorkStationServerThread::select()
{
	if (!getYkFepPublisher())
	{
		return;
	}

	QTime currTime = QDateTime::currentDateTime().time();
	QString strTime = QString("%1:%2:%3.%4").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec());

	YkCommand command;
	command.step = Execute;
	command.type = DirectlyOperate;
	command.requestId = 1;
	command.rID = strTime.toStdString();
	command.unitNo = currTime.hour();
	command.ykNo = currTime.minute();
	command.yxIndex = currTime.second();
	command.value = Yk::Open;
	m_ykFepManagerPrx->select(command);

	QString text = QString("%1:%2:%3.%4 发送 %5 请求 %6\t%7").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg(currTime.msec()).arg("select").arg(command.requestId).arg(QString().fromStdString(command.rID));

	outputOperationData(text);
}

// 选择确认
void WorkStationServerThread::selectEcho( YkCommand cmd, bool flag, string tip )
{
	if (!getYkAppPublisher())
	{
		return;
	}

	cmd.rID = "Simulated";
	m_ykAppManagerPrx->selectEcho(cmd, flag, tip);

	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1-%2-%3 发送 %4 命令 %5\t%6").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg("echo").arg(cmd.requestId).arg(QString().fromStdString(cmd.rID));

	outputOperationData(text);
}

// 选择返校
void WorkStationServerThread::selectReturn( YkCommand cmd, bool flag, string tip )
{
	if (!getYkAppPublisher())
	{
		return;
	}

	cmd.rID = "Simulated";
	m_ykAppManagerPrx->selectReturn(cmd, flag, tip);

	QTime currTime = QDateTime::currentDateTime().time();
	QString text = QString("%1-%2-%3 发送 %4 命令 %5\t%6").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second()).
		arg("return").arg(cmd.requestId).arg(QString().fromStdString(cmd.rID));

	outputOperationData(text);
}

void WorkStationServerThread::run()
{
	try 
	{
		connect(&m_ykSelectTimer, SIGNAL(timeout()), this, SLOT(select()));

		while(true)
		{
			requestCompleteData();

			IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(3));
		}
	}
	catch(const Ice::Exception& ex)
	{
		string error = ex.what();
	}	
}

bool WorkStationServerThread::getRdbRealDataRequestPublisher()
{
	if (m_rdbRealDataRequestPrx)
	{
		return true;
	}

	// 获取实时数据请求的发布者
	string topicName = RdbRealData::strRealRequestTopic;
	QString type = "实时数据请求";
	Ice::ObjectPrx proxy;
	if (!getPublisher(topicName, type, proxy))
	{
		return false;
	}
	m_rdbRealDataRequestPrx = RdbRealData::RdbRealDataRequestPrx::uncheckedCast(proxy);
	return true;
}

// 获取遥控发布对象
bool WorkStationServerThread::getYkFepPublisher()
{
	if (m_ykFepManagerPrx)
	{
		return true;
	}

	// 获取遥控操作的发布者
	string topicName = Yk::YkFepTopic;
	QString type = "遥控操作";
	Ice::ObjectPrx proxy;
	if (!getPublisher(topicName, type, proxy))
	{
		return false;
	}
	m_ykFepManagerPrx = Yk::YkFepManagerPrx::uncheckedCast(proxy);
	return true;
}

// 获取遥控返回发布对象
bool WorkStationServerThread::getYkAppPublisher()
{
	if (m_ykAppManagerPrx)
	{
		return true;
	}

	// 获取遥控命令的发布者
	string topicName = Yk::YkAppTopic;
	QString type = "遥控命令";
	Ice::ObjectPrx proxy;
	if (!getPublisher(topicName, type, proxy))
	{
		return false;
	}
	m_ykAppManagerPrx = Yk::YkAppManagerPrx::uncheckedCast(proxy);
	return true;
}

// 订阅IceStorm主题
bool WorkStationServerThread::subscribeTopic( Ice::ObjectPrx &objectPrx, const std::string &topic, const QString& type )
{
	try 
	{
		string strTopic = topic;
		string strDeliverModel = "oneway";
		string strReliability = "";
		string strRetryCount = "";

		//订阅实时数据请求数据
		bool result = BaseIceStorm::Subscriber(m_communicatorPtr, objectPrx, strTopic, strDeliverModel, strReliability,
			strRetryCount);
		if (!result)
		{
			OperationInfo info(TYPE_CLIENT);
			info.setOperationInfo(QStringLiteral("订阅%1失败").arg(type), QDateTime(), false, "");
			emit executeOperation(info);
			return false;
		}

		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅%1成功").arg(type));
		emit executeOperation(info);
		return true;
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅%1失败").arg(type), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return false;
	}
}

// 取消订阅IceStorm主题
bool WorkStationServerThread::unSubscribeTopic( Ice::ObjectPrx &objectPrx, const std::string &topic, const QString& type )
{
	try 
	{
		string strTopic = topic;

		bool result = BaseIceStorm::UnSubscriber(m_communicatorPtr, objectPrx, strTopic);
		if (!result)
		{
			OperationInfo info(TYPE_CLIENT);
			info.setOperationInfo(QStringLiteral("取消订阅%1失败").arg(type), QDateTime(), false, "");
			emit executeOperation(info);
			return false;
		}

		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("取消订阅%1成功").arg(type));
		emit executeOperation(info);
		return true;
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅%1失败").arg(type), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return false;
	}
}

// 获取发布对象
bool WorkStationServerThread::getPublisher( const std::string& topic, const QString& type, Ice::ObjectPrx& objectPrx )
{
	// 获取前置机数据的发布者
	string topicName = topic;
	string strDeliverModel = "";
	try 
	{
		objectPrx = BaseIceStorm::GetPublisher(m_communicatorPtr, topicName, strDeliverModel);
		if (!objectPrx)
		{
			OperationInfo info(TYPE_CLIENT);
			info.setOperationInfo(QStringLiteral("获取%1发布对象失败").arg(type), QDateTime(), false, "失败原因未知");
			emit executeOperation(info);
			return false;
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("获取%1发布对象失败").arg(type), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return false;
	}
	catch(...)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("获取%1发布对象失败").arg(type), QDateTime(), false, "失败原因未知");
		emit executeOperation(info);
		return false;
	}

	OperationInfo info(TYPE_CLIENT);
	info.setOperationInfo(QStringLiteral("获取%1发布对象成功").arg(type));
	emit executeOperation(info);
	return true;
}

//void WorkStationServerThread::requestCompleteData()
//{
//	try 
//	{
//		int count = 0;
//		// 每隔30s发送一次请求
//		while(count == 0)
//		{
//			if (++count > 120)
//			{
//				break;
//			}
//
//			if (!getRdbRealDataRequestPublisher())
//			{
//				return;
//			}
//
//			RdbRealData::RequestCompleteDataSeq requestSeq;
//			requestSeq.id = 1;
//			requestSeq.requestId = 1;
//			requestSeq.requestNode = "test";
//			requestSeq.isStop = false;
//			requestSeq.refreshFreq = 3;
//
//			RdbRealData::RequestCompleteData requestData;
//			requestData.tableName = "Substation";
//			requestData.fieldName = "name";
//			requestData.fieldValue = "科汇站";
//			requestSeq.seq.push_back(requestData);
//			requestSeq.dataCount = requestSeq.seq.size();
//
//			// 获取实时数据请求的订阅者
//			string strTopic = ::RdbRealData::strRealRequestTopic;
//			IceStorm::TopicPrx topic = BaseIceStorm::GetTopicProxy(m_communicatorPtr, strTopic);
//			if (topic)
//			{
//				::Ice::IdentitySeq identitySeq = topic->getSubscribers();
//
//				QTime currTime = QDateTime::currentDateTime().time();
//				QString text = QString("%1-%2-%3").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second());
//				emit outputReceiveData(text);
//
//				if (identitySeq.empty())
//				{
//					emit outputReceiveData("\t不存在订阅者");
//				}
//				for (size_t i = 0; i < identitySeq.size(); ++i)
//				{
//					Ice::Identity identity = identitySeq.at(i);
//					emit outputReceiveData(QString("\t%1\t%2").arg(QString().fromStdString(identity.category)).
//						arg(QString().fromStdString(identity.name)));
//				}
//			}
//
//			m_rdbRealDataRequestPrx->RequestCompleteData(requestSeq);
//			
//			QString text("%1 请求全部实时数据 %2-%3-%4");
//			QTime currTime = QDateTime::currentDateTime().time();
//			text = text.arg(count).arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second());
//			emit outputReceiveData(text);
//
//			IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(2));
//		}
//	}
//	catch(const Ice::Exception& ex)
//	{
//		QString error = QString().fromStdString(ex.what());
//		emit outputReceiveData(QString("请求全部实时数据失败:%1").arg(error));
//	}
//}
void WorkStationServerThread::requestCompleteData()
{
	try 
	{
		if (!CRequestManager::isRequestChanged())
		{
			return;
		}

		if (!getRdbRealDataRequestPublisher())
		{
			return;
		}

		// 循环处理所有的请求
		QList<RdbRealData::RequestCompleteDataSeq> requestSeq = CRequestManager::getCompleteDataSeq();
		Q_FOREACH(RdbRealData::RequestCompleteDataSeq request, requestSeq)
		{
			m_rdbRealDataRequestPrx->RequestCompleteData(request);
			if (request.isStop)
			{
				CRequestManager::removeCompleteDataSeq(request.requestNode);
			}

			QString text("请求全部实时数据 %1-%2-%3");
			QTime currTime = QDateTime::currentDateTime().time();
			text = text.arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second());
			emit outputReceiveData(text);
		}

		CRequestManager::setRequestChanged(false);
	}
	catch(const Ice::Exception& ex)
	{
		QString error = QString().fromStdString(ex.what());
		emit outputReceiveData(QString("请求全部实时数据失败:%1").arg(error));
	}
}

void WorkStationServerThread::requestStormTopoData()
{
	try 
	{
		int count = 0;

		if (!getRdbRealDataRequestPublisher())
		{
			return;
		}

		RdbRealData::RequestTopoDataSeq	dataSeq;
		dataSeq.id = qrand();
		dataSeq.requestId = qrand();
		dataSeq.requestNode = CGlobalVariable::instance().getRdbRequestTopic().toStdString();
		dataSeq.isStop = false;
		dataSeq.refreshFreq = 3;

		m_rdbRealDataRequestPrx->SendTopoDataRequest(dataSeq);

		QString text("%1 请求拓扑数据 %2-%3-%4");
		QTime currTime = QDateTime::currentDateTime().time();
		text = text.arg(count).arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second());
		emit outputReceiveData(text);
	}
	catch(const Ice::Exception& ex)
	{
		QString error = QString().fromStdString(ex.what());
		emit outputReceiveData(QString("请求拓扑数据失败:%1").arg(error));
	}
}

void WorkStationServerThread::batchRequestStormData()
{
	try 
	{
		if (!getRdbRealDataRequestPublisher())
		{
			return;
		}
		
		if (!m_batchRequestThread)
		{
			m_batchRequestThread = new BatchRequestDataThread(m_rdbRealDataRequestPrx, this);
			m_batchRequestThread->start();
		}
	}
	catch(const Ice::Exception& ex)
	{
		QString error = QString().fromStdString(ex.what());
		emit outputReceiveData(QString("请求全部实时数据失败:%1").arg(error));
	}
}

void WorkStationServerThread::selectCompleteData()
{
	RdbRealData::RdbDataOptPrx rdbOptPrx = RdbRealData::RdbDataOptPrx::checkedCast(
		m_communicatorPtr->stringToProxy("rdb-opt:default -h 192.168.3.25 -p 10003 -t 5000"));
	if (rdbOptPrx)
	{
		RdbRealData::RequestCompleteDataSeq requestSeq;
		requestSeq.id = 0;
		requestSeq.requestId = 0;
		requestSeq.requestNode = "workstation";
		requestSeq.isStop = false;
		requestSeq.refreshFreq = 0;
		RdbRealData::RequestCompleteData	request;
		request.tableName = "RemoteUnit";
		requestSeq.seq.push_back(request);
		requestSeq.dataCount = requestSeq.seq.size();

		RdbRealData::RespondCompleteDataSeq respondSeq;
		rdbOptPrx->SelectCompleteData(requestSeq, respondSeq);

		::RdbRealData::TopoIslandInfoSeq islandInfoSeq;
		rdbOptPrx->GetTopoIslandInfo(islandInfoSeq);
	}
}

void WorkStationServerThread::requestWarningMsg()
{
	WarningMsg::WarningMsgFilePrx warningMsgOptPrx = WarningMsg::WarningMsgFilePrx::checkedCast(
		m_communicatorPtr->stringToProxy("warningmsg-opt:default -h 192.168.3.25 -p 10003 -t 5000"));
	if (warningMsgOptPrx)
	{
		Ice::Long pos = 0;
		Ice::Int length = 0;
		std::string fileName = "2017-11-20.msg";
		WarningMsg::WarningMsgContent content;
		content = warningMsgOptPrx->getContent(pos, length, fileName);
	}
}

void WorkStationServerThread::subscriberRdbRequest(bool isStop)
{
	try 
	{
		if (m_rdbDataPrx == NULL)
		{
			// 创建订阅接口
			m_rdbDataPrx = m_objectAdapterPtr->add(new RdbRealDataRequestI(this), 
				m_communicatorPtr->stringToIdentity(m_rdbRequestSubIdentity));
		}

		string strTopic = RdbRealData::strRealRequestTopic;
		QString type = "实时数据请求";

		if (isStop)
		{
			unSubscribeTopic(m_rdbDataPrx, strTopic, type);
		}
		else
		{
			subscribeTopic(m_rdbDataPrx, strTopic, type);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅实时数据请求失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}	
}

void WorkStationServerThread::subscriberRdbRespond( bool isStop )
{
	try 
	{
		if (m_rdbRespondDataPrx == NULL)
		{
			m_rdbRespondDataPrx = m_objectAdapterPtr->add(new RdbRealDataRespondI(this), 
				m_communicatorPtr->stringToIdentity(m_rdbRespondSubIdentity));
		}

		string strTopic = CGlobalVariable::instance().getRdbRespondTopic().toStdString();
		QString type = "实时数据响应";

		if (isStop)
		{
			unSubscribeTopic(m_rdbRespondDataPrx, strTopic, type);
		}
		else
		{
			subscribeTopic(m_rdbRespondDataPrx, strTopic, type);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅实时数据响应失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}
}

void WorkStationServerThread::subscriberAlarmData( bool isStop )
{
	try 
	{
		// 创建订阅接口
		if (m_alarmDataPrx == NULL)
		{
			m_alarmDataPrx = m_objectAdapterPtr->add(new RdbAlarmDataI(this), 
				m_communicatorPtr->stringToIdentity(m_alarmSubIdentity));
		}

		string strTopic = RdbWarningData::strAlarmDataTopic;
		QString type = "报警数据";

		if (isStop)
		{
			unSubscribeTopic(m_alarmDataPrx, strTopic, type);
		}
		else
		{
			subscribeTopic(m_alarmDataPrx, strTopic, type);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅报警数据失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}
}

void WorkStationServerThread::subscriberFepData(bool isStop)
{
	try 
	{
		// 创建订阅接口
		if (m_fepDataPrx == NULL)
		{
			m_fepDataPrx = m_objectAdapterPtr->add(new FepDataManagerI(this), 
				m_communicatorPtr->stringToIdentity(m_fepDataSubIdentity));
		}

		string strTopic = FepData::strDataTopic;
		QString type = "前置机数据";

		if (isStop)
		{
			unSubscribeTopic(m_fepDataPrx, strTopic, type);
		}
		else
		{
			subscribeTopic(m_fepDataPrx, strTopic, type);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅前置机数据请求失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}
}

void WorkStationServerThread::subscriberYkFep( bool isStop )
{
	try 
	{
		// 创建订阅接口
		if (m_ykFepPrx == NULL)
		{
			m_ykFepPrx = m_objectAdapterPtr->add(new YkFepManagerI(this), 
				m_communicatorPtr->stringToIdentity(m_ykFepSubIdentity));
		}

		string strTopic = Yk::YkFepTopic;
		QString type = "遥控请求";

		if (isStop)
		{
			unSubscribeTopic(m_ykFepPrx, strTopic, type);
		}
		else
		{
			subscribeTopic(m_ykFepPrx, strTopic, type);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅遥控数据请求失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}

}

void WorkStationServerThread::subscriberYkApp( bool isStop )
{
	try 
	{
		// 创建订阅接口
		if (m_ykAppPrx == NULL)
		{
			m_ykAppPrx = m_objectAdapterPtr->add(new YkAppManagerI(this), 
				m_communicatorPtr->stringToIdentity(m_ykAppSubIdentity));
		}

		string strTopic = Yk::YkAppTopic;
		QString	type = "遥控响应";
		if (isStop)
		{
			unSubscribeTopic(m_ykAppPrx, strTopic, type);
		}
		else
		{
			subscribeTopic(m_ykAppPrx, strTopic, type);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅遥控响应失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}
}

void WorkStationServerThread::subscriberWarningMsg( bool isStop )
{
	try 
	{
		// 创建订阅接口
		if (m_warningMsgPrx == NULL)
		{
			m_warningMsgPrx = m_objectAdapterPtr->add(new SendWarningMsgI(this), 
				m_communicatorPtr->stringToIdentity(m_warningMsgSubIdentity));
		}

		string strTopic = WarningMsg::SendWarningMsgTopic;
		QString	type = "告警文件";
		if (isStop)
		{
			unSubscribeTopic(m_warningMsgPrx, strTopic, type);
		}
		else
		{
			subscribeTopic(m_warningMsgPrx, strTopic, type);
		}
	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("订阅/取消订阅告警文件失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}
}

void WorkStationServerThread::ykSelect( bool isStop )
{
	if (isStop)
	{
		m_ykSelectTimer.stop();
	}
	else
	{
		m_ykSelectTimer.start(10000);
	}
}

void WorkStationServerThread::transferCurveFile(QString date, QString fileName)
{
	OperationInfo info(TYPE_CLIENT);

	try
	{
		TransferFileSpace::TransferFilePrx proxy = TransferFileSpace::TransferFilePrx::checkedCast(
			m_communicatorPtr->stringToProxy("transferFile-master:default -h 192.168.3.25 -p 10003 -t 5000"));
		if (proxy)
		{
			string sDate = date.toStdString();
			string sFileName = fileName.toStdString();

			FILE* outf;
			if ((outf = fopen(sFileName.c_str(), "w+b")) == NULL)
			{
				info.setOperationInfo(QStringLiteral("请求历史曲线文件"), QDateTime(), false, QStringLiteral("打开文件失败"));
				emit executeOperation(info);
				return;
			}

			bool result = true;
			TransferFileSpace::FileContent fileContent;
			fileContent.pos = 0;
			do 
			{
				fileContent = proxy->requestHisCurveFile(sDate, sFileName, fileContent.pos);
				if (fileContent.status == false)
				{
					result = false;
					break;
				}

				try
				{
					size_t size = fileContent.content.size();
					char* szBuf = new char[size];  
					for (size_t i = 0; i < size; ++i)
					{
						szBuf[i] = fileContent.content[i];
					}
					fwrite(szBuf, sizeof(char), size, outf);
					delete [] szBuf;
				}
				catch(const exception& ex)
				{
					result = false;
					break;
				}

			} while (fileContent.pos != -1 && fileContent.status);

			fclose(outf);

			if (result)
			{
				info.setOperationInfo(QStringLiteral("请求历史曲线文件"), QDateTime(), true);
			}
			else
			{
				info.setOperationInfo(QStringLiteral("请求历史曲线文件"), QDateTime(), false, QStringLiteral("接收文件出错"));
			}
			emit executeOperation(info);
		}
	}
	catch(const exception& ex)
	{
		info.setOperationInfo(QStringLiteral("请求历史曲线文件"), QDateTime(), false, ex.what());
		emit executeOperation(info);
	}
}

void WorkStationServerThread::transferWarningFile(QString fileName)
{
	OperationInfo info(TYPE_CLIENT);

	try
	{
		TransferFileSpace::TransferFilePrx proxy = TransferFileSpace::TransferFilePrx::checkedCast(
			m_communicatorPtr->stringToProxy("transferFile-master:default -h 192.168.3.25 -p 10003 -t 5000"));
		if (proxy)
		{
			string sFileName = fileName.toStdString();

			FILE* outf;
			if ((outf = fopen(sFileName.c_str(), "w+b")) == NULL)
			{
				info.setOperationInfo(QStringLiteral("请求告警文件"), QDateTime(), false, QStringLiteral("打开文件失败"));
				emit executeOperation(info);
				return;
			}

			bool result = true;
			TransferFileSpace::FileContent fileContent;
			fileContent.pos = 0;
			do 
			{
				fileContent = proxy->requestWarningFile(sFileName, fileContent.pos);
				if (fileContent.status == false)
				{
					result = false;
					break;
				}

				try
				{
					size_t size = fileContent.content.size();
					char* szBuf = new char[size];  
					for (size_t i = 0; i < size; ++i)
					{
						szBuf[i] = fileContent.content[i];
					}
					fwrite(szBuf, sizeof(char), size, outf);
					delete [] szBuf;
				}
				catch(const exception& ex)
				{
					result = false;
					break;
				}

			} while (fileContent.pos != -1 && fileContent.status);

			fclose(outf);
			
			if (result)
			{
				info.setOperationInfo(QStringLiteral("请求告警文件"), QDateTime(), true);
			}
			else
			{
				info.setOperationInfo(QStringLiteral("请求告警文件"), QDateTime(), false, QStringLiteral("接收文件出错"));
			}
			emit executeOperation(info);
		}
	}
	catch(const exception& ex)
	{
		info.setOperationInfo(QStringLiteral("请求告警文件"), QDateTime(), false, ex.what());
		emit executeOperation(info);
	}
}

void WorkStationServerThread::getSubscribers(QString topicName)
{
	try 
	{
		// 获取订阅者
		string strTopic = topicName.toStdString();
		IceStorm::TopicPrx topic = BaseIceStorm::GetTopicProxy(m_communicatorPtr, strTopic);
		if (topic)
		{
			::Ice::IdentitySeq identitySeq = topic->getSubscribers();

			QTime currTime = QDateTime::currentDateTime().time();
			QString text = QString("%1-%2-%3").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second());
			emit outputReceiveData(text);

			if (identitySeq.empty())
			{
				emit outputReceiveData("\t不存在订阅者");
			}
			for (size_t i = 0; i < identitySeq.size(); ++i)
			{
				Ice::Identity identity = identitySeq.at(i);
				emit outputReceiveData(QString("\t%1\t%2").arg(QString().fromStdString(identity.category)).
					arg(QString().fromStdString(identity.name)));
			}
		}

	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("获取订阅者请求失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}
}

void WorkStationServerThread::getPublishers(QString topicName)
{
	try 
	{
		// 获取发布者
		string strTopic = topicName.toStdString();
		IceStorm::TopicPrx topic = BaseIceStorm::GetTopicProxy(m_communicatorPtr, strTopic);
		if (topic)
		{
			::Ice::ObjectPrx objectPrx = topic->getPublisher();

			QTime currTime = QDateTime::currentDateTime().time();
			QString text = QString("%1-%2-%3").arg(currTime.hour()).arg(currTime.minute()).arg(currTime.second());
			emit outputReceiveData(text);

			if (!objectPrx)
			{
				emit outputReceiveData("\t不存在发布者");
			}
			else
			{
				emit outputReceiveData(QString("\t%1").arg(QString().fromStdString(objectPrx->ice_toString())));
			}
		}

	}
	catch(const Ice::Exception& ex)
	{
		OperationInfo info(TYPE_CLIENT);
		info.setOperationInfo(QStringLiteral("获取发布者请求失败"), QDateTime(), false, ex.what());
		emit executeOperation(info);
		return;
	}
}

