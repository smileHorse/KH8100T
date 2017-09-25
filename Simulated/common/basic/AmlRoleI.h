/*
* Copyright (c) 2010,GridNT,Inc
*All rights reserved.
*
* 文件名称：AmlRoleI.h
* 摘要：角色头文件
*/

#ifndef AMLROLEI_H
#define AMLROLEI_H

#include <iostream>
#include <string>
#include <Ice/Ice.h>

#include "aml.h"

/****************************************************************************/
/* 配网主站中Server的进程接口 实现amsregistry、completeshutdown、getRole方法*/
/***************************************************************************/
class CAmlRoleI :	public Aml::CAmlRole
{
public:
	/******************************************************
	*
	* 构造函数
	*
	*parameter adapterPtr:适配器对象
	******************************************************/
explicit CAmlRoleI(const Ice::ObjectAdapterPtr &adapterPtr);

	/******************************************************
	*
	* 缺省析造函数
	*
	******************************************************/
	virtual ~CAmlRoleI(void);
	
	/******************************************************
	*
	* 查询当前Server的角色
	*
	* @return string:返回角色值,master代表主,slave代表从
	* @throws AmlException:其它原因
	*
	******************************************************/
	virtual std::string GetRole(const ::Ice::Current &current) const;

	/******************************************************
	*
	* 设置当前Server的角色
	*
	* @parameter role:角色值,master代表主,slave代表从
	* @return void
	* @throws AmlException:其它原因
	*
	******************************************************/
	virtual void SetRole(const std::string &strRole, const ::Ice::Current &current);
	
	/**
	*
	* 设置当前Server的角色及配置参数
	*
	* @parameter role:角色值,master代表主,slave代表从
	* @parameter configMapin:配置参数列表，输入参数
	* @parameter configMapout:配置参数列表，输出参数
	* @return void
	* @throws AmlException:其它原因
	*
	**/
	virtual void SetConfig(const ::std::string& strRole, const ::std::string& strDistrict, const ::std::string& strSection, const ::std::string& strDistrictName, const ::std::string& strSectionName, const ::Ice::Current &current);

public:
	static std::string s_serverRole;//各个应用Server的角色
protected:
	Ice::ObjectAdapterPtr m_adapterPtr;//适配器对象
};

#endif
