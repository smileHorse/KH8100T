/* **************************************************
* Copyright(c) 2011-, KHDQ, inc. All rights reserved.
*
* File：TDU100GeneralCmd.ice
* tdu100一般命令slice接口定义文件。
*
*****************************************************/ 

#ifndef _FEP_TDU100GENERALCMD_ICESTORM
#define _FEP_TDU100GENERALCMD_ICESTORM

[["java:package:com.kh.icestorm"]]

module TDU100GeneralCmd
{

		
    struct TDU100GeneralCmdMsg
    {
    	string deviceId;
			string inf;//信息序号--1：手动触发;2：远程复位装置；
			string triggerTime;//10:59:02 102340
    };
    ["java:type:java.util.ArrayList<TDU100GeneralCmdMsg>"] sequence<TDU100GeneralCmdMsg>	tduReqSeq;
    
    struct TDU100GeneralCmdReplyMsg
    {
    	string deviceId;
    	string bz;//信息序号
    	string message;
   	};
	["java:type:java.util.ArrayList<TDU100GeneralCmdReplyMsg>"]sequence<TDU100GeneralCmdReplyMsg> replyMsgSeq;
	
	struct TDUProtocolMsg
	{
		string deviceId;//单元号
		string name;//名称
		short protocolType;//规约号
		short channelNum;//通道个数
		short	noResponTime;//无响应时间
		string extParam;//扩展参数
		short faultNum;//失败次数
		string deviceType;//设备类型
		string used;//是否启用
		string param;//通道参数
		short updateBz;//0:update 1:add
	};
	["java:type:java.util.ArrayList<TDUProtocolMsg>"]sequence<TDUProtocolMsg> protocolMsgSeq;
    
    /**
    * tdu100一般命令
    * TDU100GeneralCmdManager
    */
    const string TDU100GeneralCmdRequestTopic = "tdu100generalcmd_operation_Request";
    const string TDU100GeneralCmdResponseTopic = "tdu100generalcmd_operation_Response";
    interface TDU100GeneralCmdManagerRequest
    {
    	void TDU100GeneralCmdRoute(tduReqSeq reqSeq);	
    	void updateProtocolParam(protocolMsgSeq protocolSeq);
    };
    
    interface TDU100GeneralCmdManagerResponse
    {
    	void TDU100GeneralCmdReply(replyMsgSeq replySeq);
    };
};

#endif  //  _FEP_TDU100GENERALCMD_ICESTORM