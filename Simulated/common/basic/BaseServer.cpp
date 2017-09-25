/*
* Copyright (c) 2010,GridNT,Inc
*All rights reserved.
*
* 文件名称：BaseServer.cpp
* 摘要：启动基本类实现文件
*/

#include <Ice/Ice.h>
#include <iostream>
#include <QtNetwork>
#include <QMessageBox>
#include <QObject>

#include "BaseServer.h"
#include "AmlRoleI.h"
#include "aml.h"

#pragma execution_character_set("utf-8")

/******************************************************
*
* 缺省构造函数
*
******************************************************/
CBaseServer::CBaseServer(void)
{
	m_bStop=false;
}

/******************************************************
*
* 缺省析造函数
*
******************************************************/
CBaseServer::~CBaseServer(void)
{

}

/******************************************************
*
* 得到主ams的ip及端口号
*
* @return void
*
******************************************************/
//void CBaseServer::getAmsAddr()
//{
//
//}

/******************************************************
*
* 向AMS Manager预订注册
*
* @return void
* @throws AmlException:其它原因
*
******************************************************/
void CBaseServer::AmsRegister()
{
	int num=1;//循环次数

	try
	{
		//查询CAmsApp代理
		char strAddr[100];
		memset(strAddr, 0, sizeof(strAddr));
		sprintf(strAddr, "amsAdapter : tcp -h %s -p %d", m_strAmsIp.c_str(), m_amsPort);
		Ice::ObjectPrx objectPrx = communicator()->stringToProxy(strAddr);
		m_appPrx = Amssrv::CAmsAppPrx::checkedCast(objectPrx->ice_timeout(ICE_TIMEOUT));

		vector<string> ipVect = getIpList();

		//得到预订角色
		m_strRole = m_appPrx->Register(m_strRegisterType, ipVect, num, m_serverInfo);

		//循环查询预订角色，直到角色为master或slave为止
		while (m_strRole == "unknow")
		{
			//#ifdef _DEBUG
			//		std::cout << m_strServerName << "向AMS Manager预订角色为:" + m_strRole << std::endl;
			//#endif
			IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(2));//等待指定时间

			num++;
			m_strRole = m_appPrx->Register(m_strRegisterType, ipVect, num, m_serverInfo);
		}

		//判断是否注册失败
		if(m_strRole == "fail")
		{
			//QMessageBox::warning(NULL, QObject::tr("提示"), QObject::tr("注册角色失败!"));
			QMessageBox msgBox(QMessageBox::Critical, QObject::tr("提示"), QObject::tr("注册角色失败!"));
			msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
			msgBox.exec();
		}

		//Aml::CAmlRolePtr rolePtr = new CAmlRoleI(m_adapterPtr);
		//rolePtr->SetRole(strRole);
#ifdef _DEBUG
		QString strTip;
		strTip = QString("%1 %2 %3").arg(m_serverInfo.name.c_str()).arg("向AMS Manager预订角色为:").arg(m_strRole.c_str());
		//QMessageBox::information(NULL, QObject::tr("提示"), strTip);
		QMessageBox msgBox(QMessageBox::Information, QObject::tr("提示"), strTip);
		msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
		msgBox.exec();
#endif
	}
	catch(const Amssrv::CAmsException &ex)
	{
		//QMessageBox::warning(NULL, QObject::tr("提示"), QObject::tr("预订角色失败!"));
		QMessageBox msgBox(QMessageBox::Critical, QObject::tr("提示"), QObject::tr("预订角色失败!"));
		msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
		msgBox.exec();
	}

	//std::cout << "等待20 秒" << std::endl;
	//IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(20));
}

/******************************************************
*
* 向AMS Manager完成注册
*
* @return void
*
******************************************************/
void CBaseServer::FinishRegister()
{
	try
	{
		m_appPrx->FinishRegister(m_serverInfo.name, m_strRole, m_district, m_section, m_districtName, m_sectionName);
	}
	catch(const Amssrv::CAmsException &ex)
	{
		//QMessageBox::warning(NULL, QObject::tr("提示"), QObject::tr("完成注册失败!"));
		QMessageBox msgBox(QMessageBox::Critical, QObject::tr("提示"), QObject::tr("完成注册失败!"));
		msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
		msgBox.exec();
	}
#ifdef _DEBUG
	//QMessageBox::information(NULL, QObject::tr("提示"), QObject::tr("完成注册!"));
	QMessageBox msgBox(QMessageBox::Critical, QObject::tr("提示"), QObject::tr("完成注册!"));
	msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
	msgBox.exec();
#endif
}


/******************************************************
*
* 资源的清理
*
* @return void
*
******************************************************/
void CBaseServer::CleanSource()
{
#ifdef _DEBUG
	//QMessageBox::information(NULL, QObject::tr("提示"), QObject::tr("清理资源!"));
	QMessageBox msgBox(QMessageBox::Information, QObject::tr("提示"), QObject::tr("清理资源!"));
	msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
	msgBox.exec();
#endif
}

/******************************************************
*
* Ice::Application的虚函数,必须实现
*
* @parameter argc:对应main方法的argc
* @parameter argv:对应main方法的argv
*
******************************************************/
int CBaseServer::run(int argc, char *argv[])
{
	shutdownOnInterrupt();

	m_bStop=false;
	try
	{
		//得到主ams的ip及端口号
		this->setAmsAddr();

		//向ams manager注册
		this->AmsRegister();

		//执行虚函数
		this->Execute(argc,argv);

		//完成注册
		this->FinishRegister();
	}
	catch(const Ice::Exception &e)
	{
		//QMessageBox::warning(NULL, QObject::tr("提示"), QObject::tr("注册失败!"));
		QMessageBox msgBox(QMessageBox::Critical, QObject::tr("提示"), QObject::tr("注册失败!"));
		msgBox.addButton(QObject::tr("确定"), QMessageBox::AcceptRole);
		msgBox.exec();
		return EXIT_FAILURE;
	}
	catch(const char *e)
	{
		//QMessageBox::warning(NULL, QObject::tr("提示"), QObject::tr("注册失败!"));
		QMessageBox msgBox(QMessageBox::Critical, QObject::tr("提示"), QObject::tr("注册失败!"));
		msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
		msgBox.exec();
		return EXIT_FAILURE;
	}
	catch(...)
	{
		//QMessageBox::warning(NULL, QObject::tr("提示"), QObject::tr("注册失败!"));
		QMessageBox msgBox(QMessageBox::Critical, QObject::tr("提示"), QObject::tr("注册失败!"));
		msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
		msgBox.exec();
		return EXIT_FAILURE;
	}
	//激活适配器
	m_adapterPtr->activate();
	Subscribe();
#ifdef _DEBUG
	//QMessageBox::information(NULL, QObject::tr("提示"), QObject::tr("开始接受请求!"));
	QMessageBox msgBox(QMessageBox::Information, QObject::tr("提示"), QObject::tr("开始接受请求!"));
	msgBox.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
	msgBox.exec();
#endif

	//激活等待请求
	communicator()->waitForShutdown();

#ifdef _DEBUG
	//QMessageBox::information(NULL, QObject::tr("提示"), QObject::tr("结束接受请求!"));
	QMessageBox msgBoxEnd(QMessageBox::Information, QObject::tr("提示"), QObject::tr("结束接受请求!"));
	msgBoxEnd.addButton(QObject::tr("确定"),QMessageBox::AcceptRole);
	msgBoxEnd.exec();
#endif

	//关闭
	this->Shutdown();

	return EXIT_SUCCESS;
}


/******************************************************
*
* 关闭Server
*
* @return void
*
******************************************************/
void CBaseServer::Shutdown()
{
	m_bStop=true;
	//关闭通讯器
	communicator()->shutdown();

	//资源清理
	this->CleanSource();

	//std::cout << CAmlConst::s_serverName << "1111111111111111111---------------------" << std::endl;
	//销毁通讯器
	communicator()->destroy();
	//std::cout << CAmlConst::s_serverName << "22222222222222222222---------------------" << std::endl;
}


//得到ip列表
vector<string> CBaseServer::getIpList()
{
	vector<string> ipVect;

	QList<QNetworkInterface>list = QNetworkInterface::allInterfaces();//获取所有网络接口的列表  
	foreach(QNetworkInterface interface,list)//遍历每个网络接口  
	{  

		//qDebug() << "Device:" << interface.name();//设备名  
		//qDebug() << "HardwareAddress:" << interface.hardwareAddress();//硬件地址  

		//获取IP地址条目列表，每个条目中包含一个IP地址，一个子网掩码和一个广播地址  
		QList<QNetworkAddressEntry> entryList = interface.addressEntries();  
		foreach(QNetworkAddressEntry entry,entryList)//遍历每一个IP地址条目  
		{  
			ipVect.push_back(entry.ip().toString().toStdString());
			//qDebug() << tr("-------------------------------------------------------");  
			//qDebug() << "IP Address:" << entry.ip().toString();//IP地址  
			//qDebug() << "Netmask:" << entry.netmask().toString();//子网掩码  
			//qDebug() << "Broadcast:" << entry.broadcast().toString();//广播地址  
			//qDebug() << tr("-------------------------------------------------------");  
		}  
		//qDebug() << endl;  
	}  

	return ipVect;
}

