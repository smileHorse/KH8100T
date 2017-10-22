
/*
* Copyright (c) 2010,GridNT,Inc*All rights reserved.
*
* 文件名称：amssrv.ice* 摘要：AMS Manager ice文件
*/


#ifndef AMSSRV_ICE
#define AMSSRV_ICE

#include "aml.ice"

[["java:package:com.kh.ams"]] 
module Amssrv{
	["java:type:java.util.ArrayList<String>"]sequence<string> Strings;
	/**
	 *
	 * AMS异常
	 *
	 **/
	exception CAmsException
	{
		/**
		*
		*异常原因
		*
		**/
	    string reason;
	};
	
	//节点信息
	struct ServerNode
	{
		string name;//节点名称
		int type;//节点类型
		string ip;//ip
		int port;//端口
		string partitionId;//分区id
		string partitionName;//分区名称
		string sectionId;//分段id
		string sectionName;//分段名称
		string preRole;//预定角色
	};
	
	
	/**
	 *
	 * AMS Manager自己接口
	 *
	 **/
	interface CAmsManager
	{ 	    	    
	    /**
	     *
	     * AMS Manager注册
	     *
	     **/
	    void Register(string strServerName, string strRole) throws CAmsException;
	    
	    //退出
	    void Quit();
	    
	    //心跳函数
	    void AmsHeartBeat(string strServerName);
	};
	
	
	/**
	 *
	 * AMS Manager对外接口
	 *
	 **/
	interface CAmsApp
	{ 	    	    
	    /**
	     *
	     * 预先注册，得到预订角色
	     *
	     * @parameter strType:节点类型
	     * @parameter ipVect:注册节点的ip列表
	     * @parameter num:注册次数
	     * @parameter ServerNode:返回节点的信息
	     * @return 预订角色(unknow/master/slave)
	     * @throws CAmsException:未知异常
	     *
	     **/
	    string Register(string strType, Strings ipVect, int num, out ServerNode serverInfo)throws CAmsException;
	    
	    /**
	     *
	     * 完成注册
	     *
	     * @parameter strServerName:Server名称
	     * @parameter strRole:确认角色
	     * @parameter district:分区
	     * @parameter section:分段
	     * @parameter districtName:分区名称
	     * @parameter sectionName:分段名称 
	     * @return void
	     * @throws CAmsException:未知异常
	     *
	     **/
	    void FinishRegister(string strServerName, string strRole, string district, string section)throws CAmsException;
	    
	    /**
	     *
	     * 退出
	     *
	     * @parameter serverInfo:Server信息
	     * @return void
	     *
	     **/
	    void Quit(ServerNode serverInfo);
	    
	    /**
	     *
	     * 查询指定Server名称的角色
	     *
	     * @return string:返回角色值,master代表主,slave代表从，unknow代表未知
	     * @throws Exception:其它原因
	     *
	     **/
	    ["cpp:const","nonmutating"] string GetRole(string strServerName)throws CAmsException;
	    
	    
	    //心跳函数
	    void HeartBeat(string strServerName);
	};
};

#endif
