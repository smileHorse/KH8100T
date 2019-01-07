/*
* Copyright (c) 2018,KeHui,Inc*All rights reserved.
*
* 文件名称：fepcmd.ice* 摘要：后台与前置机其他命令交互ice文件
*/


#ifndef FEPCMD_ICE
#define FEPCMD_ICE


[["java:package:com.kh.scada.fepcmd"]] 
module FepCmd{

	enum OPERTYPE
	{
		CALLALLDEV = 0,			//手动召唤装置全数据
		CALLALLMEM				//手动召唤前置机全数据
	};
	
	struct CallAllInfo
	{
		short		UnitNo;			//终端编号
		OPERTYPE	OperType;		//操作类型
	};

	const string CallAllFepTopic  = "callall_fep_operation";
	interface CCallAllFep
	{
		void CallAllExecute(CallAllInfo caInfo);
	};

};

#endif