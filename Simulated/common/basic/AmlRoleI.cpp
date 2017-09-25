/*
 * Copyright (c) 2010,GridNT,Inc
 *All rights reserved.
 *
 * 文件名称：AmlRoleI.cpp
 * 摘要：Server角色实现文件
 */

#include "AmlRoleI.h"

//初始化角色对象
std::string CAmlRoleI::s_serverRole = "slave";

/******************************************************
 *
 * 缺省构造函数
 *
 ******************************************************/
CAmlRoleI::CAmlRoleI(const Ice::ObjectAdapterPtr &adapterPtr)
{
	m_adapterPtr = adapterPtr;
}
/******************************************************
 *
 * 缺省析造函数
 *
 ******************************************************/
CAmlRoleI::~CAmlRoleI(void)
{
}

/******************************************************
 *
 * 查询当前Server的角色
 *
 * @return string:返回角色值,master代表主,slave代表从
 * @throws AmlException:其它原因
 *
 ******************************************************/
std::string CAmlRoleI::GetRole(const ::Ice::Current &current) const
{
#ifdef _DEBUG
	//std::cout << CAmlConst::s_serverName << "角色:" << CAmlRoleI::s_serverRole << std::endl;
#endif

	return CAmlRoleI::s_serverRole;
}

/******************************************************
 *
 * 设置当前Server的角色
 *
 * @parameter role:角色值,master代表主,slave代表从
 * @return void
 * @throws AmlException:其它原因
 *
 ******************************************************/
void CAmlRoleI::SetRole(const std::string &strRole, const ::Ice::Current &current)
{
#ifdef _DEBUG
	//std::cout << CAmlConst::s_serverName << "设置角色为:" << strRole << std::endl;
#endif

	CAmlRoleI::s_serverRole = strRole;
}

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
void CAmlRoleI::SetConfig(const ::std::string& strRole, const ::std::string& strDistrict, const ::std::string& strSection, const ::std::string& strDistrictName, const ::std::string& strSectionName, const ::Ice::Current &current)
{

}

