/* **************************************************
* Copyright(c) 2010, KHDQ,Inc. All rights reserved.
*
* File：clientfepxb.ice
* 
* 界面与行波模块数据交互的slice接口定义文件。
*****************************************************/ 

#ifndef _CLIENT_FEP_XB_ICESTORM
#define _CLIENT_FEP_XB_ICESTORM

module FepXbData 
{
	["java:type:java.util.ArrayList<Byte>"] sequence<byte>		byteSeq;
	["java:type:java.util.ArrayList<String>"] sequence<string>	stringSeq;
	["java:type:java.util.ArrayList<Short>"] sequence<short>		shortSeq;
	
	struct TduCfg
	{
		string SettingFlag;           			//定值整定标志	"TDUCFG"  6b
		byte Version;  //参数数据结构版本号 初始为：0x01  1b
		byte ChannelCheckCode;      			//通道校验码  1---工厂版；2---用户版
		byte FilterWidth;				//滤波宽度
		byte FilterCtrlCode;				//滤波控制字
		byte ChannelNum;				//通道数	1~24
		byte SamplingRate;          			//采样频率 36MHz,12MHz,8MHz,6MHz,4.8 MHz,4 MHz, 3MHz, 2.4MHz,1.5 MHz,1MHz,500kHz
		byte Rs;
		byte RecordLength;	                        //记录长度	1ms～20ms
		short TriggerInterval;				//触发间隔 10ms,20ms, 50ms, 100ms, 200ms, 500ms, 1s		
		byte WorkFrequence;//工频频率	50---50Hz,60---60Hz  		
		byte YearL;
		byte YearH;
		byte IrigB;                                     //1---无效；2---有效
		byte Flash;//1000条，2000条，…5000条
		byte TDUStaAddr;						//tdu站址
		string DevID;							//装置 ID (12 bytes)
		string StatName;						//变电站名(20bytes)
		stringSeq LineName;					//线路名(8*16)
		byteSeq ChnlsLineNum;					//通道所属线路号(24个元素)		0~5
		byte TimeZoneHour;							//时区
		byte TimeZoneMinute;
		byte Rsved;								//保留1


		byteSeq EnChAndIsInvertPhase;					//是否倒相0~1(24个元素)
		byteSeq AIBoardTypeAndMesures;   		//AI板类型&测量量(48个元素)		1~8
		byteSeq DI;//(8个元素)
		byteSeq DI2;//墨西哥双DI 2014-9-17
		shortSeq VarRatio; //CT/PT变比原边值(2*8个元素)
		byteSeq VarRatioVice;//  CT/PT变比副边值(1*8个元素)
		shortSeq VolLevel;						//电压等级(2*8个元素)
		byteSeq ChannelGain;			//通道增益(24个元素)   1~255
		byteSeq TriggerThreshold;		//触发门槛(24个元素) 4-20
		byteSeq LineCurrent;//线路电流值(额定工频值)(8个元素) 5A，1A
		byteSeq LineVICorrection;//线路电流值校正值(8个元素)
		byteSeq StartPara;//启动整定值,每通道占用12bit(36个元素)
		byteSeq GainPara;//通道增益整定值，装置使用(24个元素)

		byteSeq MACAddr1;						//(6个元素)
		byteSeq IPAddr1;						//(4个元素)
		byteSeq NetMask1;						//(4个元素)
		byteSeq Gateway1;						//(4个元素)
		short Port1;							//(2个元素)
		byteSeq MACAddr2;						//(6个元素)
		byteSeq IPAddr2;						//(4个元素)		
		byteSeq NetMask2;						//(4个元素)
		byteSeq Gateway2;						//(4个元素)
		short Port2;							//(2个元素)
		short NetReserve;						//Net备用(2个元素)
		byteSeq remoteIP1;                                               //(4个元素)
		short remotePort1;                                             //(4个元素)
		byteSeq remoteIP2;                                               //(4个元素)
		short remotePort2;                                             //(2个元素)
		byte Com1ProtocolType; //COM1规约类型
		byteSeq Com1ProtocolPara;				//COM1规约参数(6个元素)
		byte LinkAddr103; //103链路地址域
		byte FunType103;//103功能类型
		byte Com2ProtocolType; //COM1规约类型
		byteSeq Com2ProtocolPara;				//COM1规约参数(6个元素)
		byte LinkAddr2103; //103链路地址域
		byte FunType2103;//103功能类型
		string ModemPhone;//Modem远端电话号码(20 bytes)
		string ModemInitCmd;					//MODEM初始化命令(20 bytes)
		byte ModemState;//Modem工作状态 0：不使用；1，主动；2：被动
		byte Rsved3;	//保留3
	};
	
	struct SettingValueInfo
	{
		string		deviceId;					//终端编号
		TduCfg 		tdu100Cfg;	    			//tdu100配置
	};
	
	//手动触发
	struct ManualTrigger
	{
		short UnitNo;//单元
		long 	Time;//时间
		float distant;//距离
	};
	sequence<ManualTrigger> ManualTriggerSeq;
	
	//前置机到客户端
	const string FepClientTopic = "fep_client_data";
	interface CFepClientApp
	{
		void readSettingValueReply(SettingValueInfo settingVal,bool flag,string tip); //读定值结果返回,,,flag：成功失败；tip：描述
		void writeSettingValueReply(string deviceId,bool flag,string tip); //写定值结果返回
		void manTriggerReply(string deviceId, bool flag, string tip);//手动触发返回
		void resetReply(string deviceId, bool flag, string tip);//远程复位结果
	};
	
	//客户端到前置机
	const string ClientFepTopic  = "client_fep_operation";
	interface CSettingValueFep
	{
		void readSettingValue(string deviceId);//读定值
		void writeSettingValue(SettingValueInfo settingVal);//写定值
		void sendManTrigger(ManualTriggerSeq manTriggerSeq);//发送手动触发命令
		void sendReset(string deviceId);//远程复位
	};
	

};

#endif  