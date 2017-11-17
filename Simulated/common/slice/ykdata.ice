/* **************************************************
* Copyright(c) 2011-, KHDQ, inc. All rights reserved.
*
* File：yk.ice
* 遥控操作slice接口定义文件。
*
*****************************************************/ 

#ifndef _FEP_YK_ICESTORM
#define _FEP_YK_ICESTORM

[["java:package:com.gridnt.icestorm"]]

module Yk
{
    enum YkValue
    {
        Open,  // 分闸  
        Close  // 合闸
    };

    /**
    *  1. AppSrv发送Select、Execute或Cancel给FEP，FEP及时反应是否正确接收命令。
    *  2. FEP发送命令报文给Unit时，以Echo通知AppSrv。
    *  3. FEP接收到Unit返校报文，以Return通知AppSrv。
    */
    enum YkStep  // 遥控步骤
    {
	    Select,         // 选择
	    FepRecvSelect,  // 前置选择确认
	    SelectReturn,   // 终端选择返校
	    Execute,        // 执行
	    FepRecvExecute, // 前置执行确认 
	    ExecuteReturn,  // 终端执行返校
	    Cancel,         // 撤销
	    FepRecvCancel,  // 前置撤销确认命令
	    CancelReturn    // 撤销返校
    };

    enum YkType  // 遥控类型
    {
        SelectBeforeOperate,  // 选控
        DirectlyOperate       // 直控
    };

    /**
    * 操作员在调度员工作站上执行一项操作(选择/执行/撤销)，应用服务器
    * 将此命令转给前置机，前置机再将命令发送给终端。
    */
    struct YkCommand  // 遥控命令
    {
        YkStep    step;     // 直控时，step = Execute
        YkType    type;          
	    int    requestId;
        string    rID;      // 开关rID
        short     unitNo;   // 终端编号
        short     ykNo;     // 遥控号
        short     yxIndex;  // 遥信点号
	    YkValue   value;    // 遥控值
    };
    /**
	*遥控审核发给监管机
	*/
	struct YkRequestMsg
	{
	    int    requestId;
	    short    unitNo;
	    YkStep   step;     // 直控时，step = Execute
	    string   breakerRid;   
	    short    breakerControlID;
	    YkValue  breakerNormalValue;
	    string    oper; //操作员
	    bool   isAgree;     
		string  yxType;
	};
	/**
	*人机界面发布监管请求
	*
	*/
	const string YkMmiTopic = "yk_mmi_operation";
	interface YkMmiManager
	{
	   void sendYkRequest(YkRequestMsg msg);
	   void sendValidateRequest(YkRequestMsg msg);
	};
    /**
    * 命令下行（WKS->AppSrv->FEP->Unit），AppSrv为publisher，FEP为subscriber
    * 需实现接口YkFepManager。
    */
    const string YkFepTopic = "yk_fep_operation";
    interface YkFepManager
    {
        /**
        * @param: cmd - 遥控命令。
        */
        void select(YkCommand cmd);   // 选择
        void execute(YkCommand cmd);  // 执行
        void cancel(YkCommand cmd);   // 撤销
    };
    
    /**
    * 命令上行（Unit->FEP->AppSrv->WKS），FEP为publisher，AppSrv为subscriber
    * 需实现接口YkAppManager。
    */
    const string YkAppTopic = "yk_app_operation";
    interface YkAppManager
    {
        /**
        * @param: cmd - 遥控命令。
        * @param: flag - true表示操作正常，false表示操作异常。
        * @param: tip - flag=true，tip=""；flag=false，tip提示出错原因。
        */
        void selectEcho(YkCommand cmd, bool flag, string tip);    // 选择确认
        void selectReturn(YkCommand cmd, bool flag, string tip);  // 选择返校
        void executeEcho(YkCommand cmd, bool flag, string tip);   // 执行确认
        void executeReturn(YkCommand cmd, bool flag, string tip); // 执行返校
        void cancelEcho(YkCommand cmd, bool flag, string tip);    // 撤销确认
        void cancelReturn(YkCommand cmd, bool flag, string tip);  // 撤销返校
    };
};

#endif  //  _FEP_YK_ICESTORM