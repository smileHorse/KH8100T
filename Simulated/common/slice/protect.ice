/*
* Copyright (c) 2010,GridNT,Inc*All rights reserved.
*
* 文件名称：Protect.ice* 摘要：保护定值ice文件
*/


#ifndef PROTECT_ICE
#define PROTECT_ICE


[["java:package:com.kh.scada.protect"]] 
module Protect{

	enum OPERTYPE
	{
		READSETTING = 0,			//读取定值
		WRITESETTING,				//下发定值
		READGROUPNO,				//读定值区
		SWITCHGROUP					//切换定值区
	};
	sequence<byte> Bytes;
	
	struct ProtectValueInfo
	{
		short		UnitNo;			//终端编号
		short		ProLine;		//保护线路号
		OPERTYPE	OperType;		//操作类型
		short		FuncCode;		//定值功能码,0x91,0xCD,0xCE...
		short		GroupNo;		//定值组号
		short		ProNum;			//保护定值个数
		int			requestId;
		Bytes 		Vals;	    	//定值数据
	};

	const string ProtectValueAppTopic = "protectvalue_app_operation";
	interface CProtectApp
	{
		void processProtectSetting(ProtectValueInfo psInfo);
	};
	
	const string ProtectValueFepTopic  = "protectvalue_fep_operation";
	interface CProtectFep
	{
		void ProtectSettingExecute(ProtectValueInfo psInfo);
	};

};

#endif