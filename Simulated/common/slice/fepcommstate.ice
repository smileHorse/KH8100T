/* **************************************************
* Copyright(c) 2018, KHDQ,Inc. All rights reserved.
*
* File：FepCommState.ice
* 
* 前置与网管机交互的slice接口定义文件。
*****************************************************/ 

#ifndef _FEP_COMSTATE_ICESTORM
#define _FEP_COMSTATE_ICESTORM
[["java:package:com.kh.icestorm"]]
module FepCommState {
    
    // 通道状态  
    enum StateValue {
        disconnection,     // 0-通道中断
        cutoff,    		   // 1-物理通道正常
	connection,  	   // 2-通道正常
        unknown   		   // 3-未知状态
    };
    
    
    // 终端/通道状态，误码率
    struct UnitChannel {
        short    	  unitNo;          // 终端编号
	byte		  channelNo;	   // 通道编号,当前只使用0，后期双通道时，可为1。 20180918
	string		  unitName;		   // 终端名称
	string		  gateWay;		   // 本地网关
	string		  rtuAddr;		   // 终端IP
        StateValue    unitState;       // 终端通道状态
    };

    ["java:type:java.util.ArrayList<UnitChannel>"]
    sequence<UnitChannel>  UnitChannelSeq;        // 终端通道状态
    
	
    struct StateDataPacket {
        byte        	id;       // 数据包ID
        string        	fepNode;  // 所属前置机    
		int			requestId;
        UnitChannelSeq	unitchannels;           // 终端通道状态
    };
	
	const string CommStateValueAppTopic = "commstatevalue_app_operation";
	interface CComStateApp
	{
		void processCommState(StateDataPacket csInfo);
	};
	
	const string CommStateValueFepTopic  = "commstatevalue_fep_operation";
	interface CComStateFep
	{
		void processCommState(StateDataPacket csInfo);
	};
    
    // 前置通道数据定义
    const string strCommStateTopic = "fep_commdata_state";
    interface FepCommStateManager {
        void processData(StateDataPacket aPacket);     // 实时通道状态数据
    };  
    
};

#endif  // _FEP_COMSTATE_ICESTORM