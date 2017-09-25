/*
* Copyright (c) 2010,GridNT,Inc
*All rights reserved.
*
* 文件名称：BaseServer.h
* 摘要：启动基本类头文件
*/

#ifndef BASESERVER_H
#define BASESERVER_H


#include <Ice/Application.h>
#include <string>
#include <vector>

#include "amssrv.h"
#include "AmlConst.h"

using namespace std;


/*****************************************************************************/
/* 启动基本类                                                                */
/*****************************************************************************/
class CBaseServer :	public Ice::Application
{
public:

	/******************************************************
	*
	* 缺省构造函数
	*
	******************************************************/
	CBaseServer(void);

	/******************************************************
	*
	* 缺省析造函数
	*
	******************************************************/
	virtual ~CBaseServer(void);

	/******************************************************
	*
	* Ice::Application的虚函数,必须实现
	*
	* @parameter argc:对应main方法的argc
	* @parameter argv:对应main方法的argv
	*
	******************************************************/
	virtual int run(int argc, char *argv[]);

	/******************************************************
	*
	* 继承类必须实现,建立adapter,adapter映射表
	*
	* @parameter argc:对应main方法的argc
	* @parameter argv:对应main方法的argv
	*
	******************************************************/
	virtual void Execute(int argc, char *argv[])=0;

	/******************************************************
	*
	* 得到主ams的ip及端口号，设置注册的类型
	*
	* @return void
	*
	******************************************************/
	virtual void setAmsAddr(string strIp = "", int port = 20000) = 0;

	/******************************************************
	*
	* 向AMS Manager预定注册
	*
	* @return void
	*
	******************************************************/
	virtual void AmsRegister();

	/******************************************************
	*
	* 向AMS Manager完成注册
	*
	* @return void
	*
	******************************************************/
	virtual void FinishRegister();

	/******************************************************
	*
	* 资源的清理
	*
	* @return void
	*
	******************************************************/
	virtual void CleanSource();


	/******************************************************
	*
	* 关闭Server
	*
	* @return void
	*
	******************************************************/
	void Shutdown();

	//得到ip列表
	vector<string> getIpList();

	//订阅及取消订阅函数
	virtual void Subscribe()=0;

private:
	Amssrv::CAmsAppPrx m_appPrx;//CAmsApp代理
	
protected:
	Ice::ObjectAdapterPtr m_adapterPtr;//适配器对象
	string m_district, m_section, m_districtName, m_sectionName;//分区分段信息
	Amssrv::ServerNode m_serverInfo;//节点信息，注册时返回
	string m_strRole;//注册返回的角色
	string m_strAmsIp;//ip地址
	int m_amsPort;//端口号
	string m_strRegisterType;//注册节点的类型

public:
	bool m_bStop;
};
#endif
