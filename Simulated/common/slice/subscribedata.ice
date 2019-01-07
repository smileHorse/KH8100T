/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：SubscribeData.ice
* 
*****************************************************/ 
 
#ifndef _SUBSCRIBEDATA_ICE
#define _SUBSCRIBEDATA_ICE
 
[["java:package:com.gridnt.subscribedata"]]
/**
* 
* 是否有订阅数据
* 
*/
module SubData
{
     struct DataInfo
     {
        string modulename;
        bool isSubscribe;
     };
	 
	 struct PartSec
     {
        string partition;
        string section;
     };
	 
	 /**
	 *
	 *前置机通讯信息
	 *
	 **/
	struct CommInfo
	{
		int  flag;  //标志   
		int  no;    //序号
		int  used;  //是否启用
		int  type;  //类型
		string paramLine; //通道参数
	};
	
	sequence<CommInfo> CommInfoSeq;
	
	/**
	 *
	 *前置机规约单元信息
	 *
	 **/
	struct PiUnitInfo
	{
		//单元ID
		string unitId;
		
		//单元节点
		string unitNode;
	};
	
	sequence<PiUnitInfo> PiUnitInfoSeq;
	
	/**
	 *
	 *前置机规约信息
	 *
	 **/
	struct FepProtocolInfo
	{
		int  flag;  //标志
		int  no;  //规约编号
		int  used; // 使用标志
		int  type; //规约类型
		int  active;  //激活
		int  runCount;  //运行个数
		int  needWatch; //需要监视
		string name;  //规约描述
		int  unitCount;  //单元个数
		PiUnitInfoSeq piunitSeq;
		int  commCount;  //通讯口数
		CommInfoSeq commSeq;
		string paramLine;     //参数
	};
	
	sequence<FepProtocolInfo> FepProtocolInfoSeq;
	

	struct UnitInfo
	{
		int   refCount;             // 引用计数
		string  name;  // 单元名称
		int   no;                   // 单元号
		int   used;                 // 使用标志
		int   type;                 // 类型
		int   stationNo;            // 站址，必须和终端一致
		int   nodeCount;            // 通道个数
		int   mainNodeNo;           // 主节点号?
		int   replyTimeouts;        // 超时时间
		int   faultJudgeCount;      // 故障认定次数
		int   isTrans;              // 是否转发单元：0-否；1-是。
		string  res;              // 备用
		int   manuGetRFW;           // 自动提取录波标志  
		string  paramLine;            // 扩展参数
		int   facNo;    // 厂号 【待定，可以先按照这个名称来定】
		int   facType;  // 厂站类型
		int   yxNum;    // 遥信个数
		int   ycNum;    // 遥测个数
		int   ddNum;    // 电度个数
	};
	
	sequence<UnitInfo> UnitInfoSeq;
	
	sequence<short> UnitQsSeq;
	struct UnitQsInfo
	{
		int   no;                   // 单元号
		UnitQsSeq	qsSeq;			// 趋势参数
	};
	sequence<UnitQsInfo> UnitQsInfoSeq;
 
    const string SubscribeDataTopic = "subscribe_data";  // 主题
    interface SubscribeDataInfo
    {
        void procSub(DataInfo data);
		void refreshParam(PartSec data,FepProtocolInfoSeq piSeq, UnitInfoSeq unitSeq,UnitQsInfoSeq qsSeq);
    };
};
 
#endif 