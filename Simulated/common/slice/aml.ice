
/*
* Copyright (c) 2010,GridNT,Inc*All rights reserved.
*
* 文件名称：aml.ice* 摘要：应用管理接口ice文件
*/


#ifndef AML_ICE
#define AML_ICE


[["java:package:com.kh.framework"]] 
module Aml{
	/**
	 *
	 * AML异常
	 *
	 **/
	exception CAmlException
	{
		/**
		*
		*异常原因
		*
		**/
	    string reason;
	};
	
	
	
	/**
	 *
	 * 主站中Server的角色接口
	 *
	 **/
	interface CAmlRole
	{   
	    
	    /**
	     *
	     * 查询当前Server的角色
	     *
	     * @return string:返回角色值,master代表主,slave代表从
	     * @throws AmlException:其它原因
	     *
	     **/
	    ["cpp:const","nonmutating"] string GetRole()throws CAmlException;
	    
	    /**
	     *
	     * 查询前置机当前Server的角色
	     *
	     * @return string:返回角色值,master代表主,slave代表从
	     * @throws AmlException:其它原因
	     *
	     **/
	    ["cpp:const","nonmutating"] string GetFepRole(out string strPart, out string strSec)throws CAmlException;
	    
	    /**
	     *
	     * 设置当前Server的角色
	     *
	     * @parameter role:角色值,master代表主,slave代表从
	     * @return void
	     * @throws AmlException:其它原因
	     *
	     **/
	    void SetRole(string role)throws CAmlException;
	    
	    /**
	     *
	     * 设置当前Server的角色及配置参数
	     *
	     * @parameter role:角色值,master代表主,slave代表从
	     * @parameter strDistrict、strSection、strDistrictName、strSectionName:分区分段信息
	     * @return void
	     * @throws AmlException:其它原因
	     *
	     **/
	    void SetConfig(string role, string strDistrict, string strSection, string strDistrictName, string strSectionName, string isXbfep)throws CAmlException;
	};
};

#endif
