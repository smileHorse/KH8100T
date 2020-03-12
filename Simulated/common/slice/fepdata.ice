/* **************************************************
* Copyright(c) 2010, KHDQ,Inc. All rights reserved.
*
* File：FepData.ice
* 
* 前置与数据服务器数据交互的slice接口定义文件。
*****************************************************/ 

#ifndef _FEP_DATA_ICESTORM
#define _FEP_DATA_ICESTORM

module FepData {
    
    // 遥信状态  
    enum DiscreteValue {
        Open,     // 0-分闸
        Close,    // 1-合闸
		DblOpen,  // 2-双分
		DblClose, // 3-双合
        Unknown   // 4-未知状态
    };
    
    // 变化遥测
    struct ChangedAnalog {
        long     timeStamp;  // 时标
        short    unitNo;     // 终端编号
        short    index;      // 点号
        int      value;      // 值
    };    
    
    // 终端状态值
    enum State {
        NotInstall,    // 0-未安装，副通道不存在时的默认值
        OffLine,       // 1-离线，系统初始化时终端/通道的默认值
        New,           // 2-投运，终端初次投入
        Run,           // 3-运行
        Stop,          // 4-退出，终端/通道运行后又退出
        HighErrorRate  // 5-误码率高
    };
    
    // 终端/通道状态，误码率
    struct Unit {
        short    unitNo;          // 终端编号
        State    unitState;       // 单元状态，离线、运行、退出
        State    channelState1;   // 主通道状态
        State    channelState2;   // 副通道状态
        byte     errorRate;       // 误码率，0~100，不计小数
    };
    
    /**
    * 数据类型：
    *   AllDataType - unitNo、analogs、discretes、accmulators有效。
    *   ChangedAnalogType - changedAnalogs有效。
    *   AccumulatorType - unitNo、accmulators有效。
    *   UnitStateType - units有效。
    */
    enum DataType {
        AllDataType,        // 0-全数据
        ChangedAnalogType,  // 1-变化遥测
        AccumulatorType,    // 2-电度
        UnitStateType       // 3-终端状态
    };
    
    sequence<Unit>  UnitSeq;        // 终端状态
    sequence<int>   AnalogSeq;      // 遥测
    sequence<int>  DiscreteSeq;    // 遥信
    sequence<int>   AccmulatorSeq;  // 电度
    sequence<ChangedAnalog> ChangedAnalogSeq;  // 变化遥测
    struct DataPacket {
        byte        id;       // 数据包ID
        string        fepNode;  // 所属前置机    
        DataType    type;     // 数据类型
        short       unitNo;   // 单元号
        UnitSeq             units;           // 终端状态
        AnalogSeq           analogs;         // 遥测
        DiscreteSeq         discretes;       // 遥信
        AccmulatorSeq       accmulators;     // 电度
        ChangedAnalogSeq    changedAnalogs;  // 变化遥测
    };
    
    // 故障事项，处理FA和小电流接地故障
    sequence<short>   FaultValueSeq;
    struct FaultEvent {
	    short    unitNo;         // 单元编号
	    long     timeStamp;      // 时标
	    short    source;         // 故障源
	    short    eventType;      // 事项类型。0-短路；1-接地；2-选线；3-故障结束
	    short    directionFlag;  // 方向系数标志。0-无效；1-有效
        short    faultType;      // 故障类型
	    short    lineNo;         // 故障线路号
	    short    duration;       // 持续时间。短路-毫秒；接地-秒
	    short    direction;      // 故障方向。0x000F=正向；0x00F0=反向
	    short    distance;       // 故障距离。单位：KM
	    short    directionMultiplier;  // 方向系数
	    short    credibility;          // 可信度
	    short    v0Amp;                // 零序电压幅值
	    short    waveType;             // 波形类型
	    FaultValueSeq    values;
    };

    // 故障事项数据包
    sequence<FaultEvent>  FaultEvents;
    struct FaultPacket {
         byte         id;       // 数据包ID
         string         fepNode;  // 所属前置机    
         FaultEvents  events;   // 故障事项
    };
    
    // 遥信变位
    struct ChangedDigital
    {
        short            unitNo;     // 终端编号
        short            index;      // 遥信点号
        DiscreteValue    value;      // 遥信值
        long             timeStamp;  // 时标
    };
    
    /**
    * SOE
    * Soe形式上与遥信变位完全一样，只是其time由终端上报。
    */
    struct Soe
    {
        short            unitNo;     // 终端编号
        short            index;      // 遥信点号
        DiscreteValue    value;      // 遥信值
        long             timeStamp;  // 时标
    };
    
    enum ProEventType
    {
        ProtectAction,  // 0-保护动作
        ProtectAlarm    // 1-保护发信号
    };
    
    struct ProValue
    {
        short index;
        double value;
    };
    
    sequence<ProValue> ProValues;
    
    // 保护事项
    struct ProtectEvent {
       short         unitNo;     // 终端编号 
       ProEventType  Type;       // 类型
       long          timeStamp;  // 时间
       short         moduleNo;   // 模块编号 
       short         moduleType; // 模块类型 
       short         infoNo;     // 信息序号
       short         state;      // 1-动作，0-复归
       ProValues     values;     // 保护值
     };
     
     // 变化的终端状态
     struct ChangedUnit {
        short    unitNo;          // 终端编号
        State    unitState;       // 终端状态，离线、运行、退出
        State    channelState1;   // 主通道状态
        State    channelState2;   // 副通道状态
        byte     errorRate;       // 误码率，0~100，不计小数
        long     timeStamp;       // 时标
     };
	 
	 enum ChannelEventType
    {
        ChannelSwitch,  // 0-通道切换
        ChannelRun      // 1-通道投入退出
    };
	
	 // 通道事项，包括通道切换，通道投入退出
	 struct UnitChannel {
        short    			 unitNo;          // 终端编号
        ChannelEventType	 type;			  // 类型
		byte				 channelNo;		  // 原始通道号；	通道号
		byte				 channelState;	  // 切换后通道号；	通道状态：投入or退出；
		long     			 timeStamp;       // 时标
     };
    
    // 事项类型
    enum EventType {
        YxType,     // 0-遥信变位
        SoeType,    // 1-SOE
        UnitType,   // 2-终端状态
        ProType,    // 3-保护事项
		ChannelType	// 4-通道事项
    }; 
    
    sequence<ChangedDigital> ChangedDigitalSeq;
    sequence<Soe> SoeSeq;
    sequence<ProtectEvent> ProtectEventSeq;
    sequence<ChangedUnit> ChangedUnitSeq;
	sequence<UnitChannel> ChangedUnitChannelSeq;

    // 事项数据包
    struct EventPacket {
        int       id;          // 标识号
        string      fepNode;     // 所属前置机
        EventType type;        // 告警类型
        ChangedDigitalSeq digitals;  // 遥信变位序列
        SoeSeq   soes;               // SOE序列
        ProtectEventSeq  protects;   // 保护事项序列
        ChangedUnitSeq   units;      // 变化的终端序列
		ChangedUnitChannelSeq	channels; // 通道事项序列
    };
	
	  sequence<short>   WaveValueSeq;
    struct WaveEvent {
	    short    unitNo;         // 单元编号
	    long     timeStamp;      // 时标
	    short    source;         // 故障源
	    short    eventType;      // 事项类型。0-短路；1-接地；2-选线；3-故障结束
	    short    directionFlag;  // 方向系数标志。0-无效；1-有效
        short    faultType;      // 故障类型
	    short    lineNo;         // 故障线路号
	    short    duration;       // 持续时间。短路-毫秒；接地-秒
	    short    direction;      // 故障方向。0x000F=正向；0x00F0=反向
	    short    distance;       // 故障距离。单位：KM
	    short    directionMultiplier;  // 方向系数
	    short    credibility;          // 可信度
	    short    v0Amp;                // 零序电压幅值
	    short    waveType;             // 波形类型
	    WaveValueSeq    values;
    };

    // 故障事项数据包
    sequence<WaveEvent>  WaveEvents;
	//添加保护事项故障录波事项数据包	
	struct WavePacket {
         byte         id;       // 数据包ID
         string       fepNode;  // 所属前置机    
         WaveEvents  events;   // 故障事项
    };
    
    // 前置数据和事项接口定义
    const string strDataTopic = "fep_data_event";
    interface FepDataManager {
        void processData(DataPacket aDataPacket);     // 实时数据
        void processFault(FaultPacket aFaultPacket);  // 故障
        void processEvent(EventPacket aEventPacket);  // 通用事项
		void processWave(WavePacket aWavePacket);  // 保护录波事项 
    };  
    
};

#endif  // _FEP_DATA_ICESTORM