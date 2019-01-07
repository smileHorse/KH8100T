/* **************************************************
* Copyright(c) 2010, KHDQ,Inc. All rights reserved.
*
* File：Fepxb.ice
* 
* 前置与行波模块数据交互的slice接口定义文件。
*****************************************************/ 

#ifndef _FEP_XB_ICESTORM
#define _FEP_XB_ICESTORM

module FepXbData 
{
	["java:type:java.util.ArrayList<Byte>"] sequence<byte>		byteSeq;
	["java:type:java.util.ArrayList<String>"] sequence<string>	stringSeq;
	["java:type:java.util.ArrayList<Short>"] sequence<short>		shortSeq;
	
	//定值
	struct SettingValueInfo
	{
		short		unitNo;					//终端编号
		byteSeq tdu100Cfg;	    //tdu100配置
	};
	
	//通道类型及是否越限SAC字节
	struct SAC
	{
		bool			bIsOverLimit;		//是否越限
		int				nBelongToLine;		//所属线路
		int				nMeasureType;		//1-8:Ia,Ib,Ic,Va,Vb,Vc,DC+,DC-.
	};
	
	//通道信息 
	struct SChannelData
	{
		SAC				sSAC;				//通道类型及是否越限
		short  wFreqCurrentM;		//工频电流幅值
	};
	["java:type:java.util.ArrayList<SChannelData>"] sequence<SChannelData>		SChannelDataSeq;
	
	//故障触发时间
	struct STriggerTime
	{
		int				nTimeType;			//0：GPS时间；1：内部时间
		string		szTimeString;	//时间字符串：YYYYMMDDHHMMSS
		double		dUs;				//微秒
	};
	
	//故障简报
	struct TduBrief
	{
		short  unitNo;//单元号
		int				FileType;
		bool			bIsDial;			//上送方式：true-拨号；false-网络
		STriggerTime	sTriggerTime;		//触发时间
		int				nTriggerType;		//0:线路触发；1：人工触发
		int				nStartChannel;		//初始通道号
		int				nTriggerChannel;	//触发通道	1-24
		bool			bIsOverLimit;		//触发通道工频是否越限
		int				nFaultPhase;		//故障相	1-A相;2-B相;3-C相;4-AB相;5-BC相;6-AC相;7-ABC相.
		SChannelDataSeq	sChannelInfo;	//通道信息
	};
	
	sequence<TduBrief>	TduBriefSeq;
	
	//时间
	struct AlarmTime
	{
		int year;
		int month; 
		int day;
		int hour;
		int minute;
		int second;
		int milliseconds;
	};
	
	//soe
	struct XbSOE
	{
		short unitNo;//单元号
		AlarmTime soeTime; //时标
		short Di1;//di1
		short Di2;//di2
		short Val;//值
		bool Alarm;//是否告警
		bool Update;//是否更新
	};
	sequence<XbSOE>	XbSOESeq;
	
	//遥信
	struct XbDiscrete
	{
		short unitNo;//单元号
		AlarmTime disTime; //时标
		short No;//点号
		short Val;//值
		bool Alarm;//是否告警
		bool Update;//是否更新
	};
	sequence<XbDiscrete>	XbDiscreteSeq;
	
	//单元状态
	struct XbUnitStatus
	{
		short	type;
		short unitNo;//单元号
		AlarmTime disTime; //时标
		short UnitState;
		short ChannelState;
		short ErrorRate;
	};
	sequence<XbUnitStatus>	XbUnitStatusSeq;
	
	//波形
	struct XbWave
	{
		short unitNo;//单元号
		string FileName;//文件名称
		byteSeq Wave;//波形
		bool Start;//是否开始
		bool End;//是否结束
	};
	
	
	
	//与数据处理的交互
	const string FepXbTopic = "fep_xb_data";
	interface CFepXb
	{
		void sendSettingValue(SettingValueInfo settingVal);//定值
		void sendBrief(TduBriefSeq briefSeq);//简报
		void sendSoe(XbSOESeq soeSeq);//soe
		void sendDiscrete(XbDiscreteSeq discreteSeq);//遥信
		void sendUnitStatus(XbUnitStatusSeq usSeq);//单元状态
		void sendWave(XbWave wave);//波形
	};
	

};

#endif  