#ifndef _FEP_RFW_ICESTORM
#define _FEP_RFW_ICESTORM
[["java:package:com.kh.scada"]] 
module wavefepapp
{
	enum RFWTYPE
	{
		RFWKHP = 0,			//保护单元录波
		RFWKHSM = 1,			//信号电源录波
		RFWKHROD = 2,			//信号电源告警
		RFWKHHAVERFW = 3,		//有录波数据，需要手工提取
		RFWKHRFWDIR = 4        //录波文件目录(zhanxg add 2008.9.3 for BJTG101)
	};
		
	struct SRFWSOE
	{
		long     		TimeStamp; //时钟
		short			yxNo;			 //遥信号
		short			value;		//遥信值
	};
	sequence<SRFWSOE> SRFWSOESeq;
	
	struct SRFWHead
	{
		int			UnitNo;		//
		RFWTYPE	Type;		//录波类型
		int			FaultType;	//故障类型，FRD，ROD.//mao 2004.6.3
		long		Time;		//时间，PTU&STU
		int			Period;		//持续时间？？，PTU
		int			Source;		//ID号，PTU&STU
		int			Reason;		//起动方式，PTU
		int			Distance;	//距离，PTU
		int			ValueCount;	//点的个数，PTU应该为14400个，STU为12＊160=1920个 
		int         IRate;      //电流的满刻度
		int         U1Rate;     //第一路电压的满刻度
		int         U2Rate;     //第二路电压的满刻度
		int         SampeRate;  //每周波的采样点数
		int         ChannelNum; //通道数
		int         DotNumBeforeFault;     //故障前点数
		int         DotNumAfterFault;      //故障后点数
		SRFWSOESeq	soes; //科汇保护可以保存12个Soe事项
	};

	sequence<int>   SRFWDataSeq;
 struct SUnitRFW
	{
		SRFWHead    Head;
		SRFWDataSeq	Data; //8000T初步定义20000个int
	} ;

	
	// 波形数据包(只有一个波形文件，一次传一个）
	struct RFWPacket 
	{
         byte         id;       // 数据包ID
         string       fepNode;  // 所属前置机  
	 SUnitRFW     frw;
    };

    enum RfwOperateType // 操作类型
    {
			DUGRFWDIR,// 提取录波_目录
			DUGRFWFIL,// 提取录波_文件
			MANURFW // 人工录波
    };
    struct RfwOperate
    {
    	short 					unitno;
		short					lineno;
    	RfwOperateType  		operateType;
		short					filename;
    	int    					requestId;
    };
	
    struct RfwDir
	{
		short					lineno;
		short					filename;
		short 					filelength;
		short					filestate;
		long					filetime;
	};
	sequence<RfwDir>   SRFWDir;
	
	struct	RFWDirPacket
	{
		byte		id;	// 数据包ID
		string      fepNode;  // 所属前置机 
		short		unitno;	  
		SRFWDir		rfwDir;
	};
	
    /**
    * 命令下行（WKS->AppSrv->FEP->Unit），AppSrv为publisher，FEP为subscriber
    * RfwFepManager
    */
    const string RfwFepTopic = "rfw_fep_operation";
    interface RfwFepManager
    {
        /**
        * @param: unitno - 终端号。
        * @param: operateType - 操作类型。
        */
       void rfwExecute( RfwOperate operate );
    };
    
    /**
    * 命令上行（Unit->FEP->AppSrv->WKS），FEP为publisher，AppSrv为subscriber
    * 需实现接口RfwAppManager。
    */
    const string RfwAppTopic = "rfw_app_operation"; 
    interface RfwAppManager
    {
         /**
        * @param: unitno - 终端号。
        * @param: operateType - 操作类型。
        * @param: tip - flag=true，tip=""；flag=false，tip提示出错原因。
        */
        void rfwExecuteEcho(RfwOperate operate ,bool flag ,string tip);
		
        void processRfwDir(RFWDirPacket aRfwDirPacket);//录波文件目录
        void processRFW(RFWPacket aRFWPacket);  // 录波数据 
    };
    
    //实现接收工作站录波请求，并返回数据
    sequence<string>   FileDirSeq;
    sequence<byte>   	 ByteSeq;
    interface clientManager
    {
    	/*
    	*根据时间段请求文件列表
    	*@param unitNo:单元号
    	*@param startDate,endDate，请求起始、结束时间
    	*@param fileList:返回文件列表
    	* 返回值 true：成功,false：失败
    	*/
    	bool getFileDirList(int unitNo, string startDate, string endDate, out FileDirSeq fileList);
    	
    	/*
    	*根据文件名返回文件内容
    	*@param strFileName，文件名称
    	*@param strInf, strcfg, rfwDatas:分别为comtrade文件的inf、cfg、dat文件
    	* 返回值 true：成功,false：失败
    	*/
    	bool getFileCont(int unitNo, string strFileName, out string strInf, out string strCfg, out ByteSeq rfwDatas);
   	};
};

#endif