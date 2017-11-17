#ifndef WORKSTATIONSERVERTHREAD_H
#define WORKSTATIONSERVERTHREAD_H

#include "rdbdata.h"
#include "ykdata.h"

#include <string>
#include <QtCore/QtCore>
#include <Ice/ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>

using namespace std;
using namespace Yk;

class OperationInfo;

#pragma execution_character_set("utf-8")

class WorkStationServerThread : public QThread
{
	Q_OBJECT

public:
	WorkStationServerThread(QObject* parent = 0);

public:
	void setCommunicatorPtr(Ice::CommunicatorPtr ptr);
	void setObjectAdapterPtr(Ice::ObjectAdapterPtr ptr);

	// 输出操作信息
	void outputOperationData(const QString& text);
	
protected:
	virtual void run();

private:
	bool getRdbRealDataRequestPublisher();
	// 获取遥控发布对象
	bool getYkFepPublisher();
	// 获取遥控返回发布对象
	bool getYkAppPublisher();

	// 订阅IceStorm主题
	bool subscribeTopic(Ice::ObjectPrx &objectPrx, const std::string &topic, const QString& type);
	// 取消订阅IceStorm主题
	bool unSubscribeTopic(Ice::ObjectPrx &objectPrx, const std::string &topic, const QString& type);
	// 获取发布对象
	bool getPublisher(const std::string& topic, const QString& type, Ice::ObjectPrx& objectPrx);
signals:
	void executeOperation(const OperationInfo& text);
	void outputReceiveData(const QString& text);

public slots:
	// 遥控选择
	void select();

	// 选择确认
	void selectEcho(YkCommand cmd, bool flag, string tip);

	// 选择返校
	void selectReturn(YkCommand cmd, bool flag, string tip);

private slots:
	void requestCompleteData();
	void selectCompleteData();
	void subscriberRdbRequest(bool isStop);
	void subscriberRdbRespond(bool isStop);
	void subscriberAlarmData(bool isStop);
	void subscriberFepData(bool isStop);
	void subscriberYkFep(bool isStop);
	void subscriberYkApp(bool isStop);
	void ykSelect(bool isStop);

private:
	Ice::CommunicatorPtr m_communicatorPtr;
	Ice::ObjectAdapterPtr	m_objectAdapterPtr;

	Ice::ObjectPrx	m_rdbDataPrx;			// 实时数据请求订阅接口
	Ice::ObjectPrx  m_rdbRespondDataPrx;	// 实时数据响应订阅接口
	Ice::ObjectPrx	m_alarmDataPrx;			// 告警数据订阅接口
	Ice::ObjectPrx  m_fepDataPrx;			// 前置机数据响应订阅接口
	Ice::ObjectPrx  m_ykFepPrx;				// 遥控数据请求订阅接口
	Ice::ObjectPrx  m_ykAppPrx;				// 遥控数据响应订阅接口

	RdbRealData::RdbRealDataRequestPrx	m_rdbRealDataRequestPrx;
	Yk::YkFepManagerPrx					m_ykFepManagerPrx;
	Yk::YkAppManagerPrx					m_ykAppManagerPrx;

	QTimer			m_ykSelectTimer;	// 遥控选择定时器

	string		m_rdbRequestSubIdentity;
	string		m_rdbRespondSubIdentity;
	string		m_alarmSubIdentity;
	string		m_fepDataSubIdentity;
	string		m_ykFepSubIdentity;
	string		m_ykAppSubIdentity;
};

#endif