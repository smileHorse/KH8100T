/* **************************************************
* Copyright(c) 2010, GridNT,Inc. All rights reserved.
*
* File：rdbdata.ice
* 数据服务器与应用服务器的接口定义文件
* 
*****************************************************/ 

#ifndef _REAL_DB_DATA_ICE
#define _REAL_DB_DATA_ICE

[["java:package:com.gridnt.icestorm"]]

//实时数据
module RdbRealData
{
	["java:type:java.util.ArrayList<String>"]
	sequence<string> Strings;

	//请求默认值
	struct RequestDefaultData
	{
		string	tableName;//查询表名
		
		//如果fieldName为空,表示根据主键(rid)查询,fieldValue值为主键值
		//如果如果fieldName非空,表示查询以fieldName值为列名,值为fieldValue的记录
		string 	fieldName;//指定列名
		string	fieldValue;//指定列查询条件值
	};
	["java:type:java.util.ArrayList<RequestDefaultData>"]
	sequence<RequestDefaultData> RequestDefaultDataSequence;
	
	struct RequestDefaultDataSeq
	{
		long		id;
		long		requestId; //请求编号
		string 		requestNode;	//请求节点

		bool		isStop; //是否停止此数据请求,ice接口忽略该项
		int			refreshFreq; //刷新频率,0表示只一次请求,ice接口忽略该项
	
		long		dataCount; //请求序列的个数
		RequestDefaultDataSequence	seq;
	};
	
	
	//请求指定列
	struct RequestSpecficData
	{
		string	tableName;//查询表名
		Strings	fieldSeq;//返回指定的列
		
		string 	fieldName;//指定列名
		string	fieldValue;//指定列查询条件值
	};
	["java:type:java.util.ArrayList<RequestSpecficData>"]
	sequence<RequestSpecficData> RequestSpecficDataSequence;
	
	struct RequestSpecficDataSeq
	{
		long		id;
		long		requestId; //请求编号
		string 		requestNode;	//请求节点

		bool		isStop; //是否停止此数据请求,ice接口忽略该项
		int			refreshFreq; //刷新频率,0表示只一次请求,ice接口忽略该项
	
		long		dataCount; //请求序列的个数
		RequestSpecficDataSequence	seq;
	};
	
	
	//请求全列
	struct RequestCompleteData
	{
		string	tableName;//查询表名
		
		string 	fieldName;//指定列名
		string	fieldValue;//指定列查询条件值
	};
	["java:type:java.util.ArrayList<RequestCompleteData>"]
	sequence<RequestCompleteData> RequestCompleteDataSequence;
	
	struct RequestCompleteDataSeq
	{
		long		id;     
		long		requestId; //请求编号
		string 		requestNode;	//请求节点

		bool		isStop; //是否停止此数据请求,ice接口忽略该项
		int			refreshFreq; //刷新频率,0表示只一次请求,ice接口忽略该项
	
		long		dataCount; //请求序列的个数
		RequestCompleteDataSequence	seq;
	};
	
	//返回"请求默认值"的结果
	struct RespondDefaultData
	{
		string	tableName;//查询表名
		
		string	dataRid;//主键 rid
		string	dataValue;//默认项值
	};
	["java:type:java.util.ArrayList<RespondDefaultData>"]
	sequence<RespondDefaultData> RespondDefaultDataSequence;
	
	struct RespondDefaultDataSeq
	{
		long		id;   //
		long		requestId; //请求编号
		string 		requestNode;	//请求节点

		long		dataCount; //请求序列的个数
		RespondDefaultDataSequence	seq;//请求序列
	};
	
	
	//返回"请求指定列"的结果
	struct RespondSpecficData
	{
		string	tableName;//查询表名
		Strings	fieldSeq;//返回指定的列名
		
		string	dataRid;//主键 rid
		Strings	dataValues;//指定列的值序列
	};
	["java:type:java.util.ArrayList<RespondSpecficData>"]
	sequence<RespondSpecficData> RespondSpecficDataSequence;
	
	struct RespondSpecficDataSeq
	{
		long		id;
		long		requestId; //请求编号
		string 		requestNode;	//请求节点

		long		dataCount; //请求序列的个数
		RespondSpecficDataSequence	seq;//请求序列
	};
	
	
	//返回"请求全列"的结果
	struct RespondCompleteData
	{
		string	tableName;//查询表名
		
		string	dataRid;//主键 rid
		Strings	dataValues;//全列的值序列
	};
	["java:type:java.util.ArrayList<RespondCompleteData>"]
	sequence<RespondCompleteData> RespondCompleteDataSequence;
	
	struct RespondCompleteDataSeq
	{
		long		id;
		long		requestId; //请求编号
		string 		requestNode;	//请求节点

		long		dataCount; //请求序列的个数
		RespondCompleteDataSequence	seq;//请求序列
	};
	
	//返回查询结果集记录的个数
	struct RespondDataCount
	{
		string	tableName;//查询表名
		
		//如果fieldName为空,表示根据主键(rid)查询,fieldValue值为主键值
		//如果如果fieldName非空,表示查询以fieldName值为列名,值为fieldValue的记录
		string 	fieldName;//指定列名
		string	fieldValue;//指定列查询条件值
		int 	dataCount;//结果集记录的个数
	};
	["java:type:java.util.ArrayList<RespondDataCount>"]
	sequence<RespondDataCount> RespondDataCountSequence;
	
	//查询设备树形列表时返回的结构
	struct EquipTree
	{
		string parentRid;//父设备RID
		string parentName;//父设备名称
		string parentType;//父设备类型
		
		string equipRid;//设备RID
		string equipName;//设备名称
		string equipType;//设备类型
	};
	["java:type:java.util.ArrayList<EquipTree>"]
	sequence<EquipTree> EquipTreeSequence;
	
	//设备所属线路及站信息
	struct LineAndStationInfo
	{
		string lineRid;
		string lineName;
		string linePath;
		
		string stationRid;
		string stationName;
		string stationPath;
	};
	
	//请求拓扑数据的信息
	struct  RequestTopoDataSeq 
	{
		long	id;
		long	requestId;   //请求编号
		string 	requestNode;	//请求节点

		bool	isStop; //是否停止此数据请求，对于Ice接口此字段可不用
		int		refreshFreq; //刷新频率，刷新频率为零意味着是走ICEStorm的一次性请求
		Strings	ridSeq;//需要查询的连接点ID
	};
	
	//返回数据结构
	struct ConNodeState//连接点带电状态
	{
		string	rid;//连接点号      连接点ID或者是设备Id
		byte	state;//带电状态：0-失电 ，1-带电
	};
	
	["java:type:java.util.ArrayList<ConNodeState>"]
	sequence<ConNodeState>  ConNodeStateSeq;

	struct  ReceiveTopoDataSeq 
	{
		long	id;
		long    requestId;   //请求编号
		string 	requestNode;	//请求节点
		ConNodeStateSeq cnStateSeq;
 	};
 	
 	//电源点关联的非0遥测信息
	struct PowerAnalogInfo
	{
		string analogRID;
		string analogName;
		double analogValue;
	};
	["java:type:java.util.ArrayList<PowerAnalogInfo>"]
	sequence<PowerAnalogInfo> PowerAnalogInfoSeq;
	
	//电源点信息
	struct PowerInfo
	{
		string powerType;
		string powerRID;
		string powerName;
		
		PowerAnalogInfoSeq analogInfoes;
	};
	["java:type:java.util.ArrayList<PowerInfo>"]
	sequence<PowerInfo> PowerInfoSeq;
	
	//端点信息
	struct TerminalInfo
	{
		string termRID;
		string deviceType;
		string deviceRID;
		string deviceName;
	};
	["java:type:java.util.ArrayList<TerminalInfo>"]
	sequence<TerminalInfo> TerminalInfoSeq;
	
	//连接点信息
	struct ConnectivityNodeInfo
	{
		string nodeRID;
		TerminalInfoSeq terminalInfoes;
	};
	["java:type:java.util.ArrayList<ConnectivityNodeInfo>"]
	sequence<ConnectivityNodeInfo> ConnectivityNodeInfoSeq;
	
	
	//拓扑岛信息
	struct TopoIslandInfo
	{
		int islandID; 
		ConnectivityNodeInfoSeq nodeInfoes;
		PowerInfoSeq powerInfoes;
		bool powerStatus;
	};
	["java:type:java.util.ArrayList<TopoIslandInfo>"]
	sequence<TopoIslandInfo> TopoIslandInfoSeq;

	//请求实时数据接口
	const string strRealRequestTopic = "rdb_real_request";
	interface RdbRealDataRequest
	{
		void RequestDefaultData(RequestDefaultDataSeq  seq); //请求默认数据
		void RequestSpecificData(RequestSpecficDataSeq  seq);	//请求指定数据
		void RequestCompleteData(RequestCompleteDataSeq  seq);	//请求全数据
		void SendTopoDataRequest (RequestTopoDataSeq  seq); //请求拓朴数据
	};

	//响应实时数据接口
	const string strRealRespondTopic = "rdb_real_respond";
	interface RdbRealDataRespond
	{
		void RespondDefaultData (RespondDefaultDataSeq  seq); //响应默认数据
		void RespondSpecificData(RespondSpecficDataSeq  seq);	//响应指定数据
		void RespondCompleteData(RespondCompleteDataSeq  seq);	//响应全数据
		void SendTopoDataRespond (ReceiveTopoDataSeq  seq );//响应拓朴数据
	};

	//实时库ICE接口定义
	interface RdbDataOpt
	{
		//插入数据,errorSeq为操作失败的序列
		//说明:insert时必须描述所有列的值
		bool InsertData(RespondCompleteDataSeq dataSeq,out RespondCompleteDataSequence errorSeq);

		//查询数据
		bool SelectDefaultData(RequestDefaultDataSeq reqSeq,out RespondDefaultDataSeq repSeq);
		bool SelectSpecificData(RequestSpecficDataSeq reqSeq, out RespondSpecficDataSeq repSeq);
		bool SelectCompleteData(RequestCompleteDataSeq reqSeq, out RespondCompleteDataSeq repSeq);
		
		//查询结果集的个数
		bool  SelectDataCount(RequestDefaultDataSeq reqSeq,out RespondDataCountSequence repSeq);

		//更新数据,errorSeq为操作失败的序列
		bool UpdateCompleteData(RespondCompleteDataSeq reqSeq, out RespondCompleteDataSequence repSeq);

		//删除数据,errorSeq为操作失败的序列
		bool DeleteRdbData(RequestDefaultDataSeq dataSeq,out RequestDefaultDataSequence errorSeq);
		
		//查询设备树形列表
		bool GetEquipTree(string deviceType,string deviceRid,out EquipTreeSequence treeSeq);

		//查询指定子类型设备树形列表
		bool GetSpecificEquipTree(string deviceType,string deviceRid,string specDeviceType,out EquipTreeSequence treeSeq);
		
		//获得设备所属的变电站和线路
		bool GetEquipLineAndStationInfo(string deviceType,string deviceRid,out LineAndStationInfo info);
		
		//判定指定cim模型记录 是否为孤儿节点
		bool isOrphanNode(string dataType,string dataRid);
		
		//判定指定cim模型记录是否为空节点
		bool isEmptyNode(string dataType,string dataRid);
		
		//判断给定记录是否为非法数据
		bool IsInvalidDbData(string tableName,string dataRID);
		
		//获得拓扑结构
		bool GetTopoData(RequestTopoDataSeq  inDataSeq,out ReceiveTopoDataSeq  outDataSeq);
		
		//获得当前拓扑岛信息
		void GetTopoIslandInfo(out TopoIslandInfoSeq  islandInfoes);
		
		//刷新拓扑结构
		void UpdateTopoData();

		//获取断面数据
		bool GetSectionData(string deviceRid, out double analogValue, out int discreteValue);

	};
};

//告警数据
module RdbWarningData
{
	enum RdbWarningLevel {Critical,Major,Minor,Info};//告警等级
	
	//LimitEvent 越限, SoeEvent SOE, YxEvent 遥信变位, UnitEvent 终端状态, SysEvent 系统事项, OperEvent 操作事项, 
	//RelayEvent 保护事项, FaultEvent 故障事项, FaEvent FA告警, GraphEvent 画面事项, RfwEvent录波事项,AllEvent所有告警事项(客户端查询时用)
	enum RdbWarningType {LimitEvent,SoeEvent,YxEvent,UnitEvent,SysEvent,OperEvent,RelayEvent,FaultEvent,FaEvent,WaveEvent,GraphEvent, RfwEvent, AllEvent};//告警类型

	["java:type:java.util.ArrayList<String>"]
	sequence<string> Strings;

	//报警数据结构的定义
	struct  WarningInfo
	{
		string				id; //m_id标识号
		RdbWarningType	warnType; //告警类型（可以为枚举型）
		long			timeStamp; //告警时间，以毫秒表示
		RdbWarningLevel	warningLevel; //告警等级
		string			warnSource;// 告警源
		
		int				columnNum; //告警内容字段个数
		Strings			contents;//告警内容
	};
	
	["java:type:java.util.ArrayList<WarningInfo>"]
	sequence< WarningInfo >  WarningInfoSeq;

	//报警数据接口定义
	const string strAlarmDataTopic = "rdb_warning_data";
	interface RdbAlarmData
	{
		void SendAlarmData(WarningInfoSeq seq );
	};
};


#endif  // _REAL_DB_DATA_ICE

