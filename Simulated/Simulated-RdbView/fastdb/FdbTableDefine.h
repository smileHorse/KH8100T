//-------------------------------------------------------------------*--------*
// FastDB数据库的表结构
//-------------------------------------------------------------------*--------*

/*
 * Copyright (c) 2010
 * All rights reserved.
 *
 * 文件名称：FdbTableDefine.h
 * 文件标识：FastDB数据库数据库结构的类定义
 * 摘要：FastDB数据库数据库结构的类定义
 */

/* 当前版本：0.0.1
 * 作者：邵宏强
 * 完成日期：2011-3-8
 *
 */

#ifndef FAST_DB_TABLE_DEFINE_SHQ_H 
#define FAST_DB_TABLE_DEFINE_SHQ_H

#include <fastdb.h>
#include <stdio.h>

#include <iostream>
#ifdef WIN32
#include "MyTime.h"
#endif
USE_FASTDB_NAMESPACE

class IdentifiedObject;
class GeographicalRegion;
class SubGeographicalRegion;
class PowerSystemResource;
class ConnectivityNodeContainer;
class Equipment;

class RemoteUnit;

class EquipmentContainer;
class Terminal;
class ConductingEquipment;
class BaseVoltage;
class Substation;
class VoltageLevel;

class Control;
class Command;

class Measurement;
class Analog;
class Discrete;
class Accumulator;

class Line;

class ConnectivityNode;

class Connector;
class BusbarSection;

class Conductor;
class ACLineSegment;

class Switch;
class Disconnector;
class ProtectedSwitch;
class Breaker;

class PowerTransformer;
class TransformerWinding;

class AnalogCurveData;

class FormulaDefinition;
class VariableDefinition;

class DevManufacturer;
class DevDeviceType;
class DevSettingLine;
class DevSettingType;

class FepPartition;
class FepSection;
class FepProtocol;
class FepChannel;

class SystemRole;
class SystemUser;

//为所有需要命名属性的类提供一个通用命名属性的基类
// This is a root class to provide common naming attributes for all classes
// needing naming attributes
class IdentifiedObject
{
public:
	// A Model Authority issues mRIDs. Given that each Model Authority has a
	// unique id and this id is part of the mRID, then the mRID is globally
	// unique.
	std::string mRID;

	// The name is a free text human readable name of the object. It may be non
	// unique and may not correlate to a naming hierarchy.
	std::string name;

	// The localName is a human readable name of the object. It is only used
	// with objects organized in a naming hierarchy. The simplest naming
	// hierarchy has just one parent (the root) giving a flat naming hierarchy.
	// However, the naming hierarchy usually has several levels, e.g.
	// Substation, VoltageLevel, Equipment etc. Children of the same parent
	// have names that are unique among them. If the uniqueness requirement
	// cannot be met IdentifiedObject.localName shall not be used, use
	// IdentifiedObject.name  instead.
	std::string localName;

	// The pathname is a system unique name composed from all
	// IdentifiedObject.localNames in a naming hierarchy path from the object
	// to the root.
	std::string pathName;

	// The aliasName is free text human readable name of the object alternative
	// to IdentifiedObject.name. It may be non unique and may not correlate to
	// a naming hierarchy.
	std::string aliasName;

	// The description is a free human readable text describing or naming the
	// object. It may be non unique and may not correlate to a naming
	// hierarchy.
	std::string description;

	TYPE_DESCRIPTOR((KEY(mRID, INDEXED|HASHED),
					KEY(name,INDEXED|HASHED),
					FIELD(localName),
					FIELD(pathName),
					FIELD(aliasName),
					FIELD(description)));

	//insert into IdentifiedObject values ('1','2','3','4','5','6');
};

// A power system resource can be an item of equipment such as a Switch, an
// EquipmentContainer containing many individual items of equipment such as a
// Substation, or an organisational entity such as Company or SubControlArea.
// This provides for the nesting of collections of PowerSystemResources within
// other PowerSystemResources. For example, a Switch could be a member of a
// Substation and a Substation could be a member of a division of a Company.
//##ModelId=35031B8E0006
class PowerSystemResource: public IdentifiedObject
{
public:
	// Classifying instances of the same class, e.g. overhead and underground
	// ACLineSegments. This classification mechanism is intended to provide
	// flexibility outside the scope of this standard, i.e. provide
	// customisation that is non standard.
	//##ModelId=4CF4562700BF
	std::string psrType;

	// The Measurements that are included in the naming hierarchy where the PSR
	// is the containing object
	//##ModelId=39FD839B00A9
	//dbArray< dbReference<Measurement> > measurements;
	//RELATION(measurements, powersystem_resource)

	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
					FIELD(psrType)));

};

// A base class for all objects that may contain ConnectivityNodes.
//##ModelId=463FCE6E031E
class ConnectivityNodeContainer: public PowerSystemResource
{
public:

	TYPE_DESCRIPTOR(( SUPERCLASS(PowerSystemResource)));
};

// The parts of a power system that are physical devices, electronic or
// mechanical
//##ModelId=3A8A6F9D007E
class Equipment: public PowerSystemResource
{
public:
	// The association is used in the naming hierarchy.
	//##ModelId=3A8BDEBD027B
	//dbReference<EquipmentContainer> equipment_container;

	std::string ec_type;//设备所属设备容器的psrType
	std::string ec_rid;//设备所属设备容器的RID

	TYPE_DESCRIPTOR((SUPERCLASS(PowerSystemResource),
					FIELD(ec_type),
					FIELD(ec_rid)));

};

// 量测采集设备类型
enum RemoteUnitType
{
	RemoteUnit_Rtu = 0,		// RTU
	RemoteUnit_Prp,			// 保护装置
	RemoteUnit_Ftu,			// FTU
	RemoteUnit_Stu,			// STU
	RemoteUnit_Other		// 其他
};

//量测采集设备
class RemoteUnit
{
public:
	std::string mRID;//记录RID

	int4 IEDID;//IED设备号
	std::string IEDName;//IED设备名称
	int4 IEDType;//IED设备类型

	std::string timeStamp;//状态更新时间
	int4 status;//采集单元状态值

	int4 channelState1;//通道1状态
	int4 channelState2;//通道2
	int4 errorRate;//误码率

	int4 safeDays;	// 安全天数，每天零时自动加1
	real8	dayRate;	// 日投运率
	real8	monthRate;	// 月投运率

	std::string ec_type;	//设备所属设备容器的psrType	
	std::string	ec_rid;		//设备所属设备容器的RID

	int4	unitType;	// 设备类型

	int			replyTimeouts;		// 无响应时间
	int			faultJudgeCount;	// 故障判定次数
	std::string	paramLine;			// 参数
	bool		isTrans;			// 是否转发
	bool		manuGetRFW;			// 是否人工录波
	int			statNo;				// 站址
	int			yxNum;				// 遥信个数
	int			ycNum;				// 遥测个数
	int			ddNum;				// 电度个数
	std::string	groupNo;			// 所属组
	std::string voltage;			// 电压
	std::string electric;			// 电流
	std::string facTime;			// 出厂时间
	std::string runTime;			// 投运时间
	std::string reserved;			// 备注

	dbReference<DevManufacturer>	manufacturer;	// 所属厂家
	dbReference<DevDeviceType>		deviceType;		// 设备型号

	int update_unitdata(std::string timeStamp, int status, int channelState1, int channelState2, int errorRate)
	{
		int  value_changed = 0;
		if (this->status != status)
			value_changed = 1;
		if ((this->channelState2 != channelState2) &&(channelState2==3 || channelState2==4))
			value_changed += 2;
		this->timeStamp = timeStamp;
		this->status = status;//采集单元状态值

		this->channelState1 = channelState1;//通道1状态
		this->channelState2 = channelState2;//通道2
		this->errorRate = errorRate;//误码率

		return value_changed;
	}

	TYPE_DESCRIPTOR((KEY(mRID,INDEXED|HASHED),

					FIELD(IEDID),
					FIELD(IEDName),
					FIELD(IEDType),

					FIELD(timeStamp),
					FIELD(status),

					FIELD(channelState1),
					FIELD(channelState2),
					FIELD(errorRate),
					FIELD(safeDays),
					FIELD(dayRate),
					FIELD(monthRate),
					FIELD(ec_type),
					FIELD(ec_rid),
					FIELD(unitType),
					FIELD(replyTimeouts),
					FIELD(faultJudgeCount),
					FIELD(paramLine),
					FIELD(isTrans),
					FIELD(manuGetRFW),
					FIELD(statNo),
					FIELD(yxNum),
					FIELD(ycNum),
					FIELD(ddNum),
					FIELD(groupNo),
					FIELD(voltage),
					FIELD(electric),
					FIELD(facTime),
					FIELD(runTime),
					FIELD(reserved),
					RELATION(manufacturer, remoteUnits),
					RELATION(deviceType, remoteUnits)
					));
};

//遥测点表（AnalogUnitPoint）
class AnalogUnitPoint
{
public:
	std::string mRID;//	string	记录ID	Index

	int IEDID;//	int	IED设备号
	int YcIndex;//	int	遥测点号
	std::string YcName;//	string	遥测描述

	TYPE_DESCRIPTOR((KEY(mRID,INDEXED|HASHED),

					FIELD(IEDID),
					FIELD(YcIndex),

					FIELD(YcName)
			));
};

//遥信点表(DiscreteUnitPoint)
class DiscreteUnitPoint
{
public:
	std::string mRID;//	string	记录ID	Index

	int IEDID;//	int	IED设备号
	int YxIndex;//	int	遥信点号
	std::string YxName;//	string	遥信名称

	TYPE_DESCRIPTOR((KEY(mRID,INDEXED|HASHED),

					FIELD(IEDID),
					FIELD(YxIndex),

					FIELD(YxName)
			));
};

//遥控点表(ControlUnitPoint)
class ControlUnitPoint
{
public:
	std::string mRID;//	string	记录ID	Index

	int IEDID;//	int	IED设备号
	int CommandID;//	int	遥控号
	std::string CommandName;//	string	遥控名称

	TYPE_DESCRIPTOR((KEY(mRID,INDEXED|HASHED),

					FIELD(IEDID),
					FIELD(CommandID),

					FIELD(CommandName)
			));
};

// The kind of regulation model.   For example regulating voltage, reactive
// power, active power, etc.
//##ModelId=3748506400EC
// Masf add 20171025 解决与QTextStream中fixed函数的编译冲突，增加命名空间
namespace FdbTableDefine
{
	enum RegulatingControlModeKind
	{
		// Voltage is specified.
		//##ModelId=4CF46A490370
		voltage,

		// Active power is specified.
		//##ModelId=4CF46A5C00C4
		activePower,

		// Reactive power is specified.
		//##ModelId=4CF46A7A03C1
		reactivePower,

		// Current flow is specified.
		//##ModelId=4CF46A9102BD
		currentFlow,

		// The regulation mode is fixed, and thus not regulating.
		//##ModelId=4CF46AAD02D1
		fixed,

		// Admittance is specified
		//##ModelId=4CF46AC802D1
		admittance,

		// Control switches on/off by time of day. The times may change on the
		// weekend, or in different seasons.
		//##ModelId=4CF46AE20123
		timeScheduled,

		// Control switches on/off based on the local temperature (i.e., a
		// thermostat).
		//##ModelId=4CF46AFE0376
		temperature,

		//##ModelId=4CF46B1B0380
		powerFactor

	};
}

// Transformer tap changer type. Indicates the capabilities of the tap changer
// independent of the operating mode.
//##ModelId=3A8A8FE600F1
enum TapChangerKind
{
	//##ModelId=3A8A8FF3019A
	fix,

	//##ModelId=3A8A8FFE01FA
	voltageControl,

	//##ModelId=3A8A901E0296
	phaseControl,

	//##ModelId=3A8A902501EC
	voltageAndPhaseControl

};

// A modeling construct to provide a root class for all Equipment classes
//##ModelId=3A8A6ED701BA
class EquipmentContainer: public ConnectivityNodeContainer
{
public:
	// The association is used in the naming hierarchy.
	//##ModelId=3A8BDEBD026C
	//dbArray< dbReference<Equipment> > equipments;

	TYPE_DESCRIPTOR(( SUPERCLASS(ConnectivityNodeContainer)));

};

// A geographical region of a power system network model.
//##ModelId=4472BD2F0061
class GeographicalRegion: public IdentifiedObject
{
public:
	// The association is used in the naming hierarchy.
	//##ModelId=4472BDC3021D
	dbArray<dbReference<SubGeographicalRegion> > sub_regions;

	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
					RELATION(sub_regions, region)));

	//insert into GeographicalRegion values ('a','b','c','d','e','f',#0);
	//insert into GeographicalRegion values ("a");

};

// A subset of a geographical region of a power system network model.
//##ModelId=4472BC870272
class SubGeographicalRegion: public IdentifiedObject
{
public:
	// The association is used in the naming hierarchy.
	//##ModelId=4472BD3B0109
	//dbArray<dbReference<Substation> > stations;

	// The association is used in the naming hierarchy.
	//##ModelId=4472BDC3021F
	dbReference<GeographicalRegion> region;

	//##ModelId=4472C1340165
	//dbArray<dbReference<Line> > lines;

	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
					//RELATION(stations, sub_region),
					RELATION(region, sub_regions)
					//RELATION(lines, sub_region)
			));

};

class Area: public IdentifiedObject
{
public:
	std::string psrType;
	std::string ec_type;
	std::string ec_rid;
		
	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
		FIELD(psrType),
		FIELD(ec_type),
		FIELD(ec_rid)
		));

};

// An electrical connection point to a piece of conducting equipment. Terminals
// are connected at physical connection points called "connectivity nodes". 
//##ModelId=351588B4030C
class Terminal: public IdentifiedObject
{
public:
	/*
	 // The terminal connection status.   True implies the terminal is
	 // connected, and false implies the terminal is not connected. This is the
	 // result of topoplogical processing of a detailed Connectivity node and
	 // Switch model whether present in the model or not.   A terminal that is
	 // not connected cannot support a current flow.   A terminal that is
	 // connected may have flow.  In general a multi-terminal device may
	 // simultaneously have connected and disconnected terminals.  No other
	 // aspect of the algorithm
	 //##ModelId=4CF459090366
	 bool connected;

	 // The orientation of the terminal connections for a multiple terminal
	 // conducting equipment.  The sequence numbering starts with 1 and
	 // additional terminals should follow in increasing order.   The first
	 // terminal is the "starting point" for a two terminal branch.   In the
	 // case of class TransformerWinding only one terminal is used so its
	 // sequenceNumber must be 1.
	 //##ModelId=4CF4590C0075
	 int4 sequenceNumber;

	 // 所属设备ID。
	 //##ModelId=4CF4590D0228
	 int4 deviceId;

	 // 关联设备类型
	 //##ModelId=4D00904303E4
	 int4 relatedDeviceType;//需要注意:这里为enum类型

	 // ConductingEquipment has 1 or 2 terminals that may be connected to other
	 // ConductingEquipment terminals via ConnectivityNodes
	 //##ModelId=351691CB024E
	 //dbReference<ConductingEquipment> conducting_equipment;
	 */

	//端点关联导电设备的psrType
	std::string ce_type;

	//端点关联导电设备的RID
	std::string ce_rid;

	// One or more measurements may be associated with a terminal in the
	// network
	//##ModelId=352FA37000DA
	//dbArray<dbReference<Measurement> > measurements;

	// Terminals interconnect with zero impedance at a node.  Measurements on a
	// node apply to all of its terminals.
	//##ModelId=3516909E021C
	dbReference<ConnectivityNode> connectivity_node;

	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),

					//FIELD(connected),
					//FIELD(sequenceNumber),
					//FIELD(deviceId),
					//FIELD(relatedDeviceType),

					FIELD(ce_type),
					FIELD(ce_rid),

					//RELATION(conducting_equipment, terminals),
					//RELATION(measurements, terminal),
					RELATION(connectivity_node, terminals)));

};

// Enumeration of phase identifiers. Allows designation of phases for both
// transmission and distribution equipment, circuits and loads.
// Residential and small commercial loads are often served from single-phase,
// or split-phase, secondary circuits. Phases 1 and 2 refer to hot wires that
// are 180 degrees out of phase, while N refers to the neutral wire. Through
// single-phase transformer connections, these secondary circuits may be served
// from one or two of the primary phases A, B, and C. For three-phase loads,
// use the A, B, C phase codes instead of s12N.
//##ModelId=372FD56E0255
enum PhaseCode
{
	//##ModelId=372FD5B603B7
	ABCN,

	//##ModelId=45C20E7E0100
	ABC1,

	//##ModelId=45C20E8201CA
	ABN,

	//##ModelId=45C20E8700FA
	ACN,

	//##ModelId=45C20E89027D
	BCN,

	//##ModelId=45C20E8D02C4
	AB,

	//##ModelId=45C20E95004D
	AC,

	//##ModelId=45C20E990352
	BC,

	//##ModelId=45C20E9E030F
	AN,

	//##ModelId=45C20EA00351
	BN,

	//##ModelId=45C20EA202E8
	CN,

	//##ModelId=45C20EA40383
	A,

	//##ModelId=45C20EA60125
	B,

	//##ModelId=45C20EA702C3
	C,

	//##ModelId=45C20EA803AC
	N,

	// Secondary phase 1 and neutral.
	//##ModelId=4CF454D2020D
	s1N,

	// Secondary phase 2 and neutral.
	//##ModelId=4CF455860105
	s2N,

	// Secondary phases 1, 2, and neutral.
	//##ModelId=4CF455A001E5
	s12N

};

// The parts of the power system that are designed to carry current or that are
// conductively connected therewith. ConductingEquipment is contained within an
// EquipmentContainer that may be a Substation, or a VoltageLevel or a Bay
// within a Substation.
//##ModelId=350370600303
class ConductingEquipment: public Equipment
{
public:
	// Describes the phases carried by a conducting equipment. Possible values
	// { ABCN , ABC, ABN, ACN, BCN, AB, AC, BC, AN, BN, CN, A, B, C, N }.
	//##ModelId=372FD0DA028C
	int4 phase;//enum 类型  PhaseCode

	bool powerPoint;//是否为电源点设备

	// ConductingEquipment has 1 or 2 terminals that may be connected to other
	// ConductingEquipment terminals via ConnectivityNodes
	//##ModelId=351691CB024F
	//dbArray< dbReference<Terminal> > terminals;

	//##ModelId=3B5770B800C5
	//dbReference<BaseVoltage> base_voltage;
	std::string base_voltage;

	TYPE_DESCRIPTOR((SUPERCLASS(Equipment),
					FIELD(phase),
					FIELD(powerPoint),
					//RELATION(terminals, conducting_equipment),
					//RELATION(base_voltage, conducting_equipments)
					FIELD(base_voltage)

			));

};

// Combination of conducting material with consistent electrical
// characteristics, building a single electrical system, used to carry current
// between points in the power system.
//##ModelId=351578480122
class Conductor: public ConductingEquipment
{
public:
	// Segment length for calculating line section capabilities
	//##ModelId=3522E5BC033A
	real8 line_length;

	TYPE_DESCRIPTOR((SUPERCLASS(ConductingEquipment),
					FIELD(line_length)));
};

// Imaginary part of admittance.
//##ModelId=37A3A7630185
struct Susceptance
{
public:
	//##ModelId=37A3A797037F
	real8 value;

	//##ModelId=45C3786E032D
	//static UnitSymbol unit;

	//##ModelId=45C3786E0337
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));

};

// Factor by which voltage must be multiplied to give corresponding power lost
// from a circuit. Real part of admittance.
//##ModelId=37A3A35E037A
struct Conductance
{
public:
	//##ModelId=37A3A4470158
	real8 value;

	//##ModelId=3E232B5C0113
	//static UnitSymbol unit;

	//##ModelId=45C32D4F034A
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));

};

// Resistance (real part of impedance), in ohms
//##ModelId=350C6E0400F3
struct Resistance
{
public:
	//##ModelId=350C6E0F0153
	real8 value;

	//##ModelId=45C37691005F
	//static UnitSymbol unit;

	//##ModelId=45C376910060
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));

};

// Reactance (imaginary part of impedance), in ohms, at rated frequency
//##ModelId=350C6D01015F
struct Reactance
{
public:
	//##ModelId=350C6D14026A
	real8 value;

	//##ModelId=45C37582025E
	//static UnitSymbol unit;

	//##ModelId=45C37582025F
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));
};

// A wire or combination of wires, with consistent electrical characteristics,
// building a single electrical system, used to carry alternating current
// between points in the power system.
//##ModelId=35109047027F
class ACLineSegment: public Conductor
{
public:
	// Zero sequence shunt (charging) susceptance, uniformly distributed, of
	// the entire line section.
	//##ModelId=4CF489D00180
	//Susceptance b0ch;
	real8 b0ch;

	// Positive sequence shunt (charging) susceptance, uniformly distributed,
	// of the entire line section.
	//##ModelId=4CF489D20086
	//Susceptance bch;
	real8 bch;

	// Zero sequence shunt (charging) conductance, uniformly distributed, of
	// the entire line section.
	//##ModelId=4CF489D302A7
	//Conductance g0ch;
	real8 g0ch;

	// Positive sequence shunt (charging) conductance, uniformly distributed,
	// of the entire line section.
	//##ModelId=4CF489D501D5
	//Conductance gch;
	real8 gch;

	// Positive sequence series resistance of the entire line section.
	//##ModelId=4CF489DE019E
	//Resistance r;
	real8 r;

	// Zero sequence series resistance of the entire line section.
	//##ModelId=4CF489E5019E
	//Resistance r0;
	real8 r0;

	// Positive sequence series reactance of the entire line section.
	//##ModelId=4CF489E90185
	//Reactance x;
	real8 x;

	// Zero sequence series reactance of the entire line section.
	//##ModelId=4CF48D27035C
	//Reactance x0;
	real8 x0;

	TYPE_DESCRIPTOR((SUPERCLASS(Conductor),
					//FIELD(b0ch),
					FIELD(b0ch),
					//FIELD(b0ch_multiplier),
					//FIELD(bch),
					FIELD(bch),
					//FIELD(bch_multiplier),
					//FIELD(g0ch),
					FIELD(g0ch),
					//FIELD(g0ch_multiplier),
					//FIELD(gch),
					FIELD(gch),
					//FIELD(gch_multiplier),
					//FIELD(r),
					FIELD(r),
					//FIELD(r_multiplier),
					//FIELD(r0),
					FIELD(r0),
					//FIELD(r0_multiplier),
					//FIELD(x),
					FIELD(x),
					//FIELD(x_multiplier),
					//FIELD(x0)
					FIELD(x0)
					//FIELD(x0_multiplier)
			));

};

// Inductance, in millihenries
//##ModelId=350C5F6D019D
class Inductance
{
public:
	//##ModelId=350C5F9603B8
	real8 value;

	//##ModelId=45C35DC20142
	//static UnitSymbol unit;

	//##ModelId=45C35DC2014C
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));

};

// A conductor, or group of conductors, with negligible impedance, that serve
// to connect other conducting equipment within a single substation and are
// modelled with a single logical terminal.
//##ModelId=379BDF5F023B
class Connector: public ConductingEquipment
{
public:

	TYPE_DESCRIPTOR(( SUPERCLASS(ConductingEquipment)));
};

// A conductor, or group of conductors, with negligible impedance, that serve
// to connect other conducting equipment within a single substation. 
// Voltage measurements are typically obtained from VoltageTransformers that
// are connected to busbar sections. A bus bar section may have many physical
// terminals but for analysis is modelled with exactly one logical terminal.
// The typeName attribute indicates the type of bus bar section, e.g.: Main,
// Transfer.
//##ModelId=351574AF02E4
class BusbarSection: public Connector
{
public:

	TYPE_DESCRIPTOR(( SUPERCLASS(Connector)));

};

// The units defiend for usage in the CIM
//##ModelId=45C2107500CE
enum UnitSymbol
{
	// Apparent power in volt ampere
	//##ModelId=45C210B60058
	US_VA,

	// Active power in watt
	//##ModelId=45C210C90063
	US_W,

	// Reactive power in volt ampere reactive
	//##ModelId=45C210CA02E7
	US_VAr,

	// Apparent energy in volt ampere hours
	//##ModelId=45C2150C0195
	US_VAh,

	// Real energy in what hours
	//##ModelId=45C214E702CD
	US_Wh,

	// Reactive energy in volt ampere reactive hours
	//##ModelId=45C2153902C2
	US_VArh,

	// Voltage in volt
	//##ModelId=45C210CF015A
	US_V,

	// Resistance in ohm
	//##ModelId=45C211C801C1
	US_ohm,

	// Current in ampere
	//##ModelId=4638E374015E
	US_A,

	// Capacitance in farad
	//##ModelId=45C2126F031B
	US_F,

	// Inductance in henry
	//##ModelId=45C2148E0149
	US_H,

	// Relative temperature in degrees Celsius
	//##ModelId=45C213B100D3
	US_C,

	// Time in seconds
	//##ModelId=45C211350288
	US_s,

	// Time in minutes
	//##ModelId=463FC365015F
	US_min,

	// Time in hours
	//##ModelId=45C34586039A
	US_h,

	// Plane angle in degrees
	//##ModelId=45C2FF9500A7
	US_deg,

	// Plane angle in radians
	//##ModelId=45C214630113
	US_rad,

	// Energy in joule
	//##ModelId=45C214B801CE
	US_J,

	// Force in newton
	//##ModelId=45C2159401DD
	US_N,

	// Conductance in siemens
	//##ModelId=45C2FBBD0009
	US_S,

	// Dimension less quantity, e.g. count, per unit, etc.
	//##ModelId=45C30BB8017E
	US_none,

	// Frequency in hertz
	//##ModelId=45C3424602AB
	US_Hz,

	// Mass in kilogram
	//##ModelId=45C34CA5027B
	US_kg,

	// Pressure in pascal (n/m2)
	//##ModelId=45C371400050
	US_Pa,

	// Length in meter
	//##ModelId=45C361DB013F
	US_m,

	// Area in square meters
	//##ModelId=45C5B68E0206
	US_m2,

	// Volume in cubic meters
	//##ModelId=45C5B6990284
	US_m3,

	// Volt per volt ampere reactive
	//##ModelId=45C333780307
	US_V_VAr,

	// Watt per hertz
	//##ModelId=45C3423000C2
	US_W_Hz,

	// Joule per second
	//##ModelId=45C349070127
	US_J_s,

	// per second
	//##ModelId=4638E3580398
	US_s_1,

	// Mass per energy
	//##ModelId=4638E349031F
	US_kg_J,

	// Watt per second
	//##ModelId=45C36D9B0085
	US_W_s,

	//##ModelId=4CF4707000C0
	US_Hz_1

};

// The unit multipliers defined for the CIM
//##ModelId=45C210E00028
enum UnitMultiplier
{
	// Pico 10**-12
	//##ModelId=45C2134C0142
	p,

	// Nano 10**-9
	//##ModelId=45C21372039D
	n,

	// Micro 10**-6
	//##ModelId=45C2128C017D
	micro,

	// Milli 10**-3
	//##ModelId=45C210F60045
	m,

	// Centi 10**-2
	//##ModelId=45C21105004F
	c,

	// Deci 10**-1
	//##ModelId=45C2110A00AC
	d,

	// Kilo 10**3
	//##ModelId=45C21112009A
	k,

	// Mega 10**6
	//##ModelId=45C211190143
	M,

	// Giga 10**9
	//##ModelId=45C2111D0017
	G,

	// Tera 10**12
	//##ModelId=45C2111E01BE
	T,

	//##ModelId=45CA0C6A0109
	none

};

// Value representing kV
//##ModelId=3503727C02EB
struct Voltage
{
	// Value representing voltage
	//##ModelId=350372850398
	real8 value;

	//##ModelId=350C73570049
	//int4 unit;//enum 类型  UnitSymbol 单位

	//##ModelId=45C2177302B6
	int4 multiplier;//enum 类型  UnitMultiplier 倍率

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));
};

// Collection of BaseVoltages which is used to verify that the
// BusbarSection.BaseVoltage and other voltage attributes in the CIM are given
// a value existing in the collection.
//##ModelId=379C01D60026
class BaseVoltage: public IdentifiedObject
{
public:
	// The PowerSystemResource's base voltage.
	//##ModelId=379C01EF0298
	//Voltage nominalVoltage;

	real8 nominalVoltage;
	//int4 nominalVoltage_multiplier;

	//##ModelId=3B5770B80093
	//dbArray<dbReference<ConductingEquipment> > conducting_equipments;

	//##ModelId=39FD93460126
	dbArray<dbReference<VoltageLevel> > voltage_levels;

	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
					//FIELD(nominalVoltage),
					FIELD(nominalVoltage),
					//FIELD(nominalVoltage_multiplier),
					//RELATION(conducting_equipments,base_voltage ),
					RELATION(voltage_levels,base_voltage )));

};

// A collection of equipment for purposes other than generation or utilization,
// through which electric energy in bulk is passed for the purposes of
// switching or modifying its characteristics. 
//##ModelId=35158764019A
class Substation: public EquipmentContainer
{
public:
	// The association is used in the naming hierarchy.
	//##ModelId=4472BD3B0113
	//dbReference<SubGeographicalRegion> sub_region;

	std::string ec_type;
	std::string ec_rid;

	// The association is used in the naming hierarchy.
	//##ModelId=39F98B9C0210
	//dbArray<dbReference<VoltageLevel> > voltage_levels;

	// The association is used in the naming hierarchy.
	//##ModelId=3A8929F703BE
	//dbArray<dbReference<Bay> > bays;

	TYPE_DESCRIPTOR((SUPERCLASS(EquipmentContainer),
					//RELATION(sub_region,stations ),
					FIELD(ec_type),
					FIELD(ec_rid)
					//RELATION(voltage_levels,substation )
					//RELATION(bays,substation )
			));

};

// A collection of equipment at one common system voltage forming a switchgear.
// The equipment typically consist of breakers, busbars, instrumentation,
// control, regulationand protection devices as well as assemblies of all
// these.
//##ModelId=39F9836D0193
class VoltageLevel: public EquipmentContainer
{
public:
	// The bus bar's high voltage limit
	//##ModelId=39FEC47D01F7
	//Voltage highVoltageLimit;
	real8 highVoltageLimit;
	//int4 highVoltageLimit_multiplier;

	// The bus bar's low voltage limit
	//##ModelId=39FEC48202E1
	//Voltage lowVoltageLimit;
	real8 lowVoltageLimit;
	//int4 lowVoltageLimit_multiplier;

	// The association is used in the naming hierarchy.
	//##ModelId=39F98B9C0211
	//dbReference<Substation> substation;

	std::string ec_type;
	std::string ec_rid;

	//##ModelId=39FD93460116
	dbReference<BaseVoltage> base_voltage;

	// The association is used in the naming hierarchy.
	//##ModelId=39F98BA10378
	//dbArray<dbReference<Bay> > bays;

	TYPE_DESCRIPTOR((SUPERCLASS(EquipmentContainer),
					//FIELD(highVoltageLimit),
					//FIELD(lowVoltageLimit),
					FIELD(highVoltageLimit),
					//FIELD(highVoltageLimit_multiplier),
					FIELD(lowVoltageLimit),
					//FIELD(lowVoltageLimit_multiplier),
					//RELATION(substation,voltage_levels ),
					FIELD(ec_type),
					FIELD(ec_rid),
					RELATION(base_voltage,voltage_levels )
					//RELATION(bays,voltage_level )
			));

};

// Switching arrangement for Bay. 
//##ModelId=37480C0B00B1
enum BreakerConfiguration
{
	//##ModelId=37480E2C0191
	singleBreaker,

	//##ModelId=37480E6F01B5
	breakerAndAHalf,

	//##ModelId=37480E8B0314
	doubleBreaker,

	//##ModelId=37480E9B008C
	noBreaker

};

// Busbar layout for Bay.
//##ModelId=37480ED90027
enum BusbarConfiguration
{

	//##ModelId=37480F3603D8
	singleBus,

	//##ModelId=37480F48010D
	doubleBus,

	//##ModelId=37480F570227
	mainWithTransfer,

	//##ModelId=37480F800370
	ringBus

};


// Date and time as "yyyy-mm-ddThh:mm:ss.sss", which conforms with ISO 8601.
// UTC time zone is specified as "yyyy-mm-ddThh:mm:ss.sssZ". A local timezone
// relative UTC is specified as "yyyy-mm-ddThh:mm:ss.sss-hh:mm".
// AbsoluteDateTime can be used both for calender time, e.g. 2007-02-07T10:30,
// and for relative time, e.g. 10:30.
//##ModelId=35036D32004F
struct AbsoluteDateTime
{
	// String representation of date and time, refer to description of the
	// class.
	//##ModelId=45C5B89B032D
	std::string value;

	TYPE_DESCRIPTOR(( FIELD(value)));

};

// Product of RMS value of the voltage and the RMS value of the quadrature
// component of the current (Megavoltamperes Reactive)
//##ModelId=350C6D4C03CA
struct ReactivePower
{
public:
	//##ModelId=350C6D570343
	real8 value;

	//##ModelId=45C32DB20191
	//static UnitSymbol unit;

	//##ModelId=45C216CC00C6
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));

};

// Phase angle in radians
//##ModelId=350C672C0072
struct AngleRadians
{
public:
	//##ModelId=350C674002D3
	real8 value;

	//##ModelId=45C338D00162
	//static UnitSymbol unit;

	//##ModelId=45C3001503BB
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));
};

// Product of RMS value of the voltage and the RMS value of the in-phase
// component of the current
//##ModelId=350890990101
struct ActivePower
{
public:
	//##ModelId=350890A700ED
	real8 value;

	//##ModelId=45C32D5503DE
	//static UnitSymbol unit;

	//##ModelId=45C20FCD013F
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));
};

// Control is used for supervisory/device control. It represents control
// outputs that are used to change the state in a process, e.g. close or open
// breaker, a set point value or a raise lower command.
//##ModelId=39DDD77502C4
class Control: public IdentifiedObject
{
public:
	// The last time a control output was sent
	//##ModelId=39DDD77502C8
	std::string timeStamp;

	// Indicates that a client is currently sending control commands that has
	// not completed
	//##ModelId=39DDD77502C9
	bool operationInProgress;

	int bilaFlag;//双点遥控标志

	// Specifies the type of Control, e.g. BreakerOn/Off,
	// GeneratorVoltageSetPoint, TieLineFlow etc. The ControlType.name shall be
	// unique among all specified types and describe the type. The
	// ControlType.aliasName is meant to be used for localization.
	//##ModelId=4CF4AD99037D
	//std::string type;

	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
					FIELD(timeStamp),
					FIELD(operationInProgress),
					FIELD(bilaFlag)
					//FIELD(type)
			));

};

// A Command is a discrete control used for supervisory control.
//##ModelId=44731DDF01FD
class Command: public Control
{
public:
	std::string psr_type;//遥控关联的电力资源的psrType
	std::string psr_rid;//遥控关联的电力资源的RID

	int ftuUnitId;//IED设备单元号
	int ftuPointId;//IED设备遥控号
	std::string ftuVlDesc;//IED设备61850遥控描述

	bool	directControl;	// 直控标志

	TYPE_DESCRIPTOR((SUPERCLASS(Control),
					FIELD(psr_rid),
					FIELD(psr_type),
					FIELD(ftuUnitId),
					FIELD(ftuPointId),
					FIELD(ftuVlDesc),
					FIELD(directControl)
			));
};

// A Measurement represents any measured, calculated or non-measured
// non-calculated quantity. Any piece of equipment may contain Measurements,
// e.g. a substation may have temperature measurements and door open
// indications, a transformer may have oil temperature and tank pressure
// measurements, a bay may contain a number of  power flow measurements and a
// Breaker may contain a switch status measurement. 
// The PSR - Measurement association is intended to capture this use of
// Measurement and is included in the naming hierarchy based on
// EquipmentContainer. The naming hierarchy typically has Measurements as
// leafs, e.g. Substation-VoltageLevel-Bay-Switch-Measurement.
// Some Measurements represent quantities related to a particular sensor
// location in the network, e.g. a voltage transformer (PT) at a busbar or a
// current transformer (CT) at the bar between a breaker and an isolator. The
// sensing position is not captured in the PSR - Measurement association.
// Instead it is captured by the Measurement - Terminal association that is
// used to define the sensing location in the network topology. The location is
// defined by the connection of the Terminal to ConductingEquipment. 
// Two possible paths exist:
// 1) Measurement-Terminal- ConnectivityNode-Terminal-ConductingEquipment
// 2) Measurement-Terminal-ConductingEquipment
// Alternative 2 is the only allowed use. 
// When the sensor location is needed both Measurement-PSR and
// Measurement-Terminal are used. The Measurement-Terminal association is never
// used alone.
//##ModelId=351580050154
class Measurement: public IdentifiedObject
{
public:
	// Specifies the type of Measurement, e.g. IndoorTemperature,
	// OutDoorTemperature, BusVoltage, GeneratorVoltage, LineFlow etc.
	//##ModelId=4CF4A22102D8
	//std::string measurementType; yening del in 2011.5.26字符串描述方式不是合于我们目前的应用
	int4 measurementType;

	// The unit multiplier of the measured quantity.
	//##ModelId=4CF4A2230085
	int4 unitMultiplier;//enum 类型  UnitMultiplier 倍率

	// The unit of measure of the measured quantity.
	//##ModelId=4CF4A225000D
	int4 unitSymbol;//enum 类型  UnitSymbol 单位

	// 基准值是否校准，因而测量值是否正确。
	//##ModelId=4CF4A3E30167
	bool badReference;

	// true表示数值被状态估计器替换。
	//##ModelId=4CF4A40D02C5
	bool estimatorReplaced;

	// 此标识符表示监督功能已经检测到一个内部或外部故障，例如，通讯故障。
	//##ModelId=4CF4A42B02C0
	bool failure;

	// 在指定的时间间隔中，量测值没有被成功更新，测量值是旧的数据，并且可能是无效的。
	//##ModelId=4CF4A45D0220
	bool oldData;

	// 量测值阻塞，其传输不能进行。
	//##ModelId=4CF4A4800036
	bool operatorBlocked;

	// 为了防止通讯过负荷，检测和抑制振荡的（快速变化的）二进制数据输入是明智的。如果在一个规定的时间（tosc）内某个信号在同一方向上（从0到1或从1到0）变化了两次，就检查出振荡，具体的质量标识符"oscillatory"就被置1。如果检测到振荡，就可能掠过预设的瞬时变化数。这时，validity
	// 的状态设置为"questionable"。如果在这一预设变化数后还继续处于振荡状态，就应该把值设置为和以前的稳定值相反的状态，或者设置为预定义的缺省值。在这种情况下，只要振荡存在，validity状态"questionable"被置0，"invalid"被置1。假如配置得不允许瞬时变化掠过，在把具体质量标识符
	// "oscillatory"置1（只用于状态信息）的同时也立即把validity状态"invalid"置1。
	//##ModelId=4CF4A4AF03CE
	bool oscillatory;

	// 量测值超出预定义的值范围。
	//##ModelId=4CF4A4D90144
	bool outOfRange;

	// 量测值不在能够合理表示的范围内。例如，计数器从最大值溢出而复位到零。
	//##ModelId=4CF4A4FE020C
	bool overFlow;

	// Source给出与数值来源相关的信息。该值可以从过程中获取，可以是缺省值或是替代值。
	//##ModelId=4CF4A51F02ED
	bool source;

	// 一个对比功能发现值和其它值不一致。通常由网络状态估计器设置。
	//##ModelId=4CF4A5560018
	bool suspect;

	// 量测值是为了测试的目的传输。
	//##ModelId=4CF4A5760374
	bool test;

	// Validity可以是good（好的）、questionable（可疑的）或者invalid（无效的）。
	//##ModelId=4CF4A5A1005E
	bool validity;

	// 替代MeasurementValueSource。MeasurementValueSource describes the
	// alternative sources updating a MeasurementValue. User conventions for
	// how to use the MeasurementValueSource attributes are described in the
	// introduction to IEC 61970-301.
	//##ModelId=4CF4A5C3007C
	std::string valueSource;

	// The limit, expressed as a percentage of the sensor maximum, that errors
	// will not exceed when the sensor is used under  reference conditions.
	//##ModelId=4CF4A333019D
	real8 sensorAccuracy;

	// The time when the value was last updated
	//##ModelId=4CF4A3310026
	std::string timeStamp;

	// The PowerSystemResource that contains the Measurement in the naming
	// hierarchy
	//##ModelId=39FD839B00C8
	//dbReference<PowerSystemResource> powersystem_resource;

	//是否保存断面数据
	bool saveSection;

	//是否封锁
	bool lockFlag;

	//是否抑制告警
	bool holdFlag;

	//PowerSystemResource对象的类型
	std::string psr_type;

	// PowerSystemResource对象的RID
	std::string psr_rid;

	// One or more measurements may be associated with a terminal in the
	// network
	//##ModelId=352FA37000CB
	//dbReference<Terminal> terminal;

	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
					FIELD(measurementType),
					FIELD(unitMultiplier),
					FIELD(unitSymbol),

					FIELD(badReference),
					FIELD(estimatorReplaced),
					FIELD(failure),
					FIELD(oldData),
					FIELD(operatorBlocked),
					FIELD(oscillatory),
					FIELD(outOfRange),
					FIELD(overFlow),
					FIELD(source),
					FIELD(suspect),
					FIELD(test),
					FIELD(validity),

					FIELD(valueSource),

					FIELD(timeStamp),
					FIELD(sensorAccuracy),

					KEY(saveSection,INDEXED|HASHED),

					FIELD(lockFlag),
					FIELD(holdFlag),

					FIELD(psr_type),
					FIELD(psr_rid)

					//RELATION(terminal, measurements)
			));

};

class AnalogType
{
public:
	enum
	{
		Ia = 0, Ib, Ic, Ua, Ub, Uc, Uab, Uca, Ubc, P, Q, Cos
	//有功，无功，功率因数
	};
};

//遥测越限状态
//0-越上上限；1-越上限；2-越限回归；3-越下限；4-越下下限

#define UP_HH_LIMIT			0 //越上上限
#define UP_H_LIMIT			1 //越上限
#define NORMAL_LIMIT		2 //越限回归
#define LOW_L_LIMIT			3 //越下限
#define LOW_LL_LIMIT		4 //越下下限
#define UP_HH_LIMIT_RET		5 //越上上限回归
#define UP_H_LIMIT_RET		6 //越上限回归
#define LOW_LL_LIMIT_RET	7 //越下下限回归
#define LOW_L_LIMIT_RET		8 //越下限回归
// Analog represents an analog Measurement.
//##ModelId=40E50BEE0226
class Analog: public Measurement
{
public:
	//是否保存曲线报表(保存曲线报表的就要统计最大值和最小值)
	bool saveReport;

	// Normal value range maximum for any of the MeasurementValue.values. Used
	// for scaling, e.g. in bar graphs or of telemetered raw values.
	//##ModelId=447315DB03D3
	real8 maxValue;

	// Normal value range minimum for any of the MeasurementValue.values. Used
	// for scaling, e.g. in bar graphs or of telemetered raw values
	//##ModelId=447315E20026
	real8 minValue;

	// Normal measurement value, e.g., used for percentage calculations.
	//##ModelId=4473166C00D9
	real8 normalValue;

	// If true then this measurement is an active power, reactive power or
	// current with the convention that a positive value measured at the
	// Terminal means power is flowing into the related PowerSystemResource.
	//##ModelId=447317F0019F
	bool positiveFlowIn;

	//遥测变比
	real8 ratioFactor;

	// The value to supervise.
	//##ModelId=4CF4A6A60285
	real8 value;

	//是否检测越限值
	bool checkLimit;

	// 上上限值
	//##ModelId=4CF4A7EB0343
	real8 hhLimitValue;

	// 上限值
	//##ModelId=4CF4A746020D
	real8 hLimitValue;

	// 下限值
	//##ModelId=4CF4A80D023F
	real8 lLimitValue;

	// 下下限值
	//##ModelId=4CF4A82B0230
	real8 llLimitValue;

	// 上限死区
	real8 hlimitDead;

	// 下限死区
	real8 llimitDead;

	// Tells if the limit values are in percentage of normalValue or the
	// specified Unit for Measurements and Controls.
	//##ModelId=4CF4A6E802BC
	bool isPercentageLimits;

	//越限状态
	int limitStatus;

	//采集设备单元号
	int4 ftuUnitId;

	//采集设备点号
	int4 ftuPointId;

	//61850规约值描述
	std::string ftuVlDesc;
	
	//关联的曲线数据
	dbReference<AnalogCurveData> analog_curve;

	//关联的计算公式
	dbReference<FormulaDefinition> analog_formula;

	//检测越限状态是否改变
	bool checkLimitStatus(double& limit_vl)
	{
		double actual_vl = this->value;

		//判断是否需要检测越限
		if (!checkLimit)
			return false;

		int new_stauts = NORMAL_LIMIT;
		//获取新状态,说明:遥测值达到设定边界即为越限
		if (actual_vl >= hhLimitValue)
		{
			limit_vl = hhLimitValue;
			new_stauts = UP_HH_LIMIT;
		}
		else if ((actual_vl < hhLimitValue) && (actual_vl >= hLimitValue))
		{
			limit_vl = hLimitValue;
			new_stauts = UP_H_LIMIT;
		}
		else if ((actual_vl < hLimitValue) && (actual_vl > lLimitValue))
		{
			limit_vl = 0;
			new_stauts = NORMAL_LIMIT;
		}
		else if ((actual_vl <= lLimitValue) && (actual_vl > llLimitValue))
		{
			limit_vl = lLimitValue;
			new_stauts = LOW_L_LIMIT;
		}
		else if (actual_vl <= llLimitValue)
		{
			limit_vl = llLimitValue;
			new_stauts = LOW_LL_LIMIT;
		}

		//判断状态是否改变
		if (new_stauts != limitStatus)
		{
			limitStatus = new_stauts;
			return true;
		}
		else
			return false;
	}

	//更新遥测值，如果越限状态变化则返回值为ture,否则为false
	bool update_data(std::string tm, double org_vl, double& limit_vl)
	{
		//封锁值
		if (lockFlag)
			return false;

		double actual_vl = org_vl * this->ratioFactor;
		
		if(actual_vl <0.000001 && actual_vl> -0.000001)
			actual_vl=0.00;
		
		this->timeStamp = tm;
		this->value = actual_vl;
		limit_vl = 0;

		//判断是否需要检测越限
		if (!checkLimit)
			return false;

		int new_stauts = NORMAL_LIMIT;
		//获取新状态,说明:遥测值达到设定边界即为越限
		if (actual_vl >= hhLimitValue)
		{
			limit_vl = hhLimitValue;
			new_stauts = UP_HH_LIMIT;
		}
		else if ((actual_vl < hhLimitValue) && (actual_vl >= hLimitValue))
		{
			limit_vl = hLimitValue;
			new_stauts = UP_H_LIMIT;
		}
		else if ((actual_vl < hLimitValue) && (actual_vl > lLimitValue))
		{
			limit_vl = 0;
			new_stauts = NORMAL_LIMIT;
		}
		else if ((actual_vl <= lLimitValue) && (actual_vl > llLimitValue))
		{
			limit_vl = lLimitValue;
			new_stauts = LOW_L_LIMIT;
		}
		else if (actual_vl <= llLimitValue)
		{
			limit_vl = llLimitValue;
			new_stauts = LOW_LL_LIMIT;
		}

		//判断状态是否改变
		if (new_stauts != limitStatus)
		{
			limitStatus = new_stauts;
			return true;
		}
		else
			return false;

	}

	// 检查是否产生复归告警
	bool checkResetStatus(int oldStatus, int& status, double& limitValue)
	{
		bool result = true;
		switch(limitStatus)
		{
		case NORMAL_LIMIT:
			{
				switch(oldStatus)
				{
				case UP_HH_LIMIT:
					status = UP_HH_LIMIT_RET;
					limitValue = hhLimitValue;
					break;
				case UP_H_LIMIT:
					status = UP_H_LIMIT_RET;
					limitValue = hLimitValue;
					break;
				case LOW_L_LIMIT:
					status = LOW_L_LIMIT_RET;
					limitValue = lLimitValue;
					break;
				case LOW_LL_LIMIT:
					status = LOW_LL_LIMIT_RET;
					limitValue = llLimitValue;
					break;
				default:
					result = false;
					break;
				}
			}
			break;
		case UP_HH_LIMIT:
			{
				switch(oldStatus)
				{
				case LOW_L_LIMIT:
					status = LOW_L_LIMIT_RET;
					limitValue = lLimitValue;
					break;
				case LOW_LL_LIMIT:
					status = LOW_LL_LIMIT_RET;
					limitValue = llLimitValue;
					break;
				default:
					result = false;
					break;
				}
			}
			break;
		case UP_H_LIMIT:
			{
				switch(oldStatus)
				{
				case UP_HH_LIMIT:
					status = UP_HH_LIMIT_RET;
					limitValue = hhLimitValue;
					break;
				case LOW_L_LIMIT:
					status = LOW_L_LIMIT_RET;
					limitValue = lLimitValue;
					break;
				case LOW_LL_LIMIT:
					status = LOW_LL_LIMIT_RET;
					limitValue = llLimitValue;
					break;
				default:
					result = false;
					break;
				}
			}
			break;
		case LOW_L_LIMIT:
			{
				switch(oldStatus)
				{
				case UP_HH_LIMIT:
					status = UP_HH_LIMIT_RET;
					limitValue = hhLimitValue;
					break;
				case UP_H_LIMIT:
					status = UP_H_LIMIT_RET;
					limitValue = hLimitValue;
					break;
				case LOW_LL_LIMIT:
					status = LOW_LL_LIMIT_RET;
					limitValue = llLimitValue;
					break;
				default:
					result = false;
					break;
				}
			}
			break;
		case LOW_LL_LIMIT:
			{
				switch(oldStatus)
				{
				case UP_HH_LIMIT:
					status = UP_HH_LIMIT_RET;
					limitValue = hhLimitValue;
					break;
				case UP_H_LIMIT:
					status = UP_H_LIMIT_RET;
					limitValue = hLimitValue;
					break;
				default:
					result = false;
					break;
				}
			}
			break;
		default:
			result = false;
			break;
		}

		return result;
	}

	TYPE_DESCRIPTOR((SUPERCLASS(Measurement),

					KEY(saveReport,INDEXED|HASHED),

					FIELD(maxValue),
					FIELD(minValue),
					FIELD(normalValue),

					FIELD(positiveFlowIn),
					FIELD(ratioFactor),
					FIELD(value),

					FIELD(checkLimit),
					FIELD(hhLimitValue),
					FIELD(hLimitValue),
					FIELD(lLimitValue),
					FIELD(llLimitValue),

					FIELD(hlimitDead),
					FIELD(llimitDead),

					FIELD(isPercentageLimits),
					FIELD(limitStatus),

					KEY(ftuUnitId,INDEXED|HASHED),
					KEY(ftuPointId,INDEXED|HASHED),
					KEY(ftuVlDesc,INDEXED|HASHED),

					OWNER(analog_curve, analog),
					OWNER(analog_formula, analog)
			));

};

//曲线点值
struct CurvePointValue
{
	bool isValid;//是否为有效值
	real8 value;//值

	TYPE_DESCRIPTOR((FIELD(isValid),
					FIELD(value)));
};

//极值
struct Extremum
{
	std::string tm;//极值时间
	real8 value;//极值

	TYPE_DESCRIPTOR((FIELD(tm),
					FIELD(value)));
};

//曲线数据表
class AnalogCurveData
{
public:
	dbDateTime date;//时间(年月日)

	dbReference<Analog> analog;//关联的遥测项ID

	dbArray<CurvePointValue> pointValues;//数据点数据

	bool updateLimit;//是否更新了极值
	Extremum maxValue;//最大值
	Extremum minValue;//最小值

	//更新极值
	bool UpdateExtreumValue(std::string tm, double vl)
	{
		if (!updateLimit)
		{
			maxValue.tm = minValue.tm = tm;
			maxValue.value = minValue.value = vl;

			updateLimit = true;
			return true;
		}

		bool is_update = false;

		if (maxValue.value < vl)
		{
			maxValue.tm = tm;
			maxValue.value = vl;

			is_update = true;
		}

		if (minValue.value > vl)
		{
			minValue.tm = tm;
			minValue.value = vl;

			is_update = true;
		}

		return is_update;
	}

	//获得当前有效的点数
	int getValidPointCount()
	{
		dbDateTime tm = dbDateTime::current();
		int count = tm.hour() * 12 + tm.minute() / 5 + 1;

		return count;
	}

	//曲线统计值
	bool getStatisticsValue(double& ave_vl, std::string& max_tm, double& max_vl, std::string& min_tm, double& min_vl)
	{
		double vl_total = 0;
		int vl_count = 0;

		int is_update = false;

		ave_vl = 0;
		int max_id = 0;
		int min_id = 0;
		max_vl = 0;
		min_vl = 0;

		for (size_t i = 0; i < pointValues.length(); i++)
		{
			if (pointValues.getat(i).isValid)
			{
				vl_total += pointValues.getat(i).value;
				vl_count++;

				if (!is_update)
				{
					max_id = min_id = i;
					max_vl = min_vl = pointValues.getat(i).value;

					is_update = true;
				}
				else
				{
					if (pointValues.getat(i).value > max_vl)
					{
						max_id = i;
						max_vl = pointValues.getat(i).value;
					}

					if (pointValues.getat(i).value < min_vl)
					{
						min_id = i;
						min_vl = pointValues.getat(i).value;
					}
				}
			}
		}

		dbDateTime max_date(date.asTime_t() + max_id * 5 * 60);
		char temp[128];
		snprintf(temp, 128, "%04d-%02d-%02d %02d:%02d:%02d", max_date.year(), max_date.month(), max_date.day(),
			max_date.hour(), max_date.minute(), max_date.second());
		max_tm = temp;

		dbDateTime min_date(date.asTime_t() + min_id * 5 * 60);
		snprintf(temp, 128, "%04d-%02d-%02d %02d:%02d:%02d", min_date.year(), min_date.month(), min_date.day(),
			min_date.hour(), min_date.minute(), min_date.second());
		min_tm = temp;

		if (vl_count > 0)
			ave_vl = vl_total / vl_count;
		return true;
	}

	//获得点值组成的字符串
	std::string getPointValues(bool only_valid = false)
	{
		std::string strData;
		char chtmp[128];

		int count = pointValues.length();
		if (only_valid)
		{
			int valid_count = getValidPointCount();
			if (valid_count < count)
				count = valid_count;
		}

		for (int i = 0; i < count; i++)
		{
			if (pointValues.getat(i).isValid)
			{
				snprintf(chtmp, 128, "%.2f;", pointValues.getat(i).value);
				strData += chtmp;
			}
			else
				strData += ";";
		}

		return strData;
	}

	bool updatePointValues(std::string str_values)
	{
		dbArray<double> actual_values;
		double value;

		int begin_pos = 0;
		int pos = -1;

		while ((pos = str_values.find(';', begin_pos)) != -1)
		{
			value = atof(std::string(str_values.begin() + begin_pos, str_values.begin() + pos).c_str());
			begin_pos = pos + 1;

			actual_values.append(value);
		}

		size_t valid_count = actual_values.length();
		if (valid_count > pointValues.length())
			valid_count = pointValues.length();

		CurvePointValue vl;
		for (size_t i = 0; i < pointValues.length(); i++)
		{
			if (i < valid_count)
			{
				vl.isValid = true;
				vl.value = actual_values.getat(i);
				pointValues.putat(i, vl);
			}
			else
			{
				vl.isValid = false;
				vl.value = 0;
				pointValues.putat(i, vl);
			}
		}

		return true;
	}

	//复位数组
	bool resetValue(dbDateTime& new_date, bool isBeginDay = false)
	{
		//取时间中的年月日部分
		dbDateTime rec_date(new_date.year(), new_date.month(), new_date.day());

		//printf("曲线记录新时间:%04d-%02d-%02d\n", new_date.year(), new_date.month(), new_date.day());

		CurvePointValue vl;
		vl.isValid = false;
		vl.value = 0;

		//设置大小
		if (pointValues.length() != 289)
		{
			pointValues.resize(289);//289
			if (0 < pointValues.length())
				pointValues.putat(0, vl);
		}
		else
		{
			//新的开始的一天的曲线数据
			if (isBeginDay)
			{
				if (0 < pointValues.length())
					pointValues.putat(0, pointValues.getat(288));
				//printf("设置起始时间点的值!!!\n");
			}
		}

		//设置时间
		date = rec_date;

		//复位值(注意：从1开始)
		for (size_t i = 1; i < pointValues.length(); i++)
			pointValues.putat(i, vl);

		updateLimit = false;
		maxValue.tm = "";
		maxValue.value = 0;
		minValue.tm = "";
		minValue.value = 0;

		return true;
	}

	//设置值
	bool setValue(int seq_id, double value, dbDateTime& )
	{
		if ((seq_id < 0) || (seq_id >= (int)pointValues.length()))
			return false;

		if (seq_id < (int)pointValues.length())
		{
			CurvePointValue vl;
			vl.isValid = true;
			vl.value = value;
			pointValues.putat(seq_id, vl);
		}

		CompleteData(seq_id, value);//补全当前数据


		return true;
	}

	//补全当前数据
	void CompleteData(int seq_id, double value)
	{
		//printf("come here 1 CompleteData:%d - %g\r\n",seq_id,value);

		if ((seq_id < 0) || (seq_id >= (int)pointValues.length()))
			return;

		//printf("come here 2 CompleteData:%d - %g\r\n",seq_id,value);

		for (int i = seq_id - 1; i >= 0; i--)
		{
			if (!pointValues.getat(i).isValid)//补全
			{
				if (i < (int)pointValues.length())
				{
					CurvePointValue vl;
					vl.isValid = true;
					vl.value = value;
					pointValues.putat(i, vl);
				}

				//printf("come here 3 CompleteData:%d - %g\r\n",i,value);
			}
			else
				value = pointValues.getat(i).value;
		}
	}

	TYPE_DESCRIPTOR((FIELD(date),

		RELATION(analog, analog_curve),

		FIELD(pointValues),

		FIELD(updateLimit),
		FIELD(maxValue),
		FIELD(minValue)));
};

/**
 * An AnalogLimitSet specifies a set of Limits that are associated with an Analog
 * measurement.
 */
/*
 class AnalogLimitSet
 {

 public:
 dbReference<Analog> analog;//关联的遥测项ID

 bool isPercentageLimits;

 //越限状态
 int limitStatus;

 //检测越限状态
 bool CheckOutLimitStatus(double vl, double hhLimitValue, double hLimitValue, double lLimitValue,
 double llLimitValue, int& new_stauts, double& limit_vl)
 {
 new_stauts = 2;

 if (vl >= hhLimitValue)
 {
 new_stauts = UP_HH_LIMIT;
 limit_vl = hhLimitValue;
 }
 else if ((vl < hhLimitValue) && (vl >= hLimitValue))
 {
 new_stauts = UP_H_LIMIT;
 limit_vl = hLimitValue;
 }
 else if ((vl < hLimitValue) && (vl > lLimitValue))
 {
 new_stauts = NORMAL_LIMIT;
 limit_vl = 0;
 }
 else if ((vl <= lLimitValue) && (vl > llLimitValue))
 {
 new_stauts = LOW_L_LIMIT;
 limit_vl = lLimitValue;
 }
 else if (vl <= llLimitValue)
 {
 new_stauts = LOW_LL_LIMIT;
 limit_vl = llLimitValue;
 }

 //判断状态是否改变
 if (new_stauts != limitStatus)
 {
 limitStatus = new_stauts;
 return true;
 }
 else
 return false;
 }

 TYPE_DESCRIPTOR((
 RELATION(analog, analog_limitset),

 FIELD (isPercentageLimits),
 FIELD(limitStatus)));
 };
 */

//断面数据类型
class SecDataType
{
public:
	enum
	{
		YC_SEC = 1, YX_SEC, DD_SEC
	//遥测断面数据，遥信断面数据，电度断面数据
	};
};

//断面数据的个数
#define SEC_DATA_LEN 16


/*********************************************************************************************************
 * 当前版本：0.0.1
 * 作者：yening
 * 完成日期：2011-5-26
 * 离散量类型定义
 *******************************************************************************************************/
class DiscreteType
{
public:
	enum
	{
		_Break_State = 0,//开关状态
		_Remote_Local,//就地远方
		_Protect
	//保护

	};
};
class Discrete: public Measurement
{
public:
	// Normal value range maximum for any of the MeasurementValue.values. Used
	// for scaling, e.g. in bar graphs or of telemetered raw values.
	//##ModelId=447316E0011C
	int4 maxValue;

	// Normal value range minimum for any of the MeasurementValue.values. Used
	// for scaling, e.g. in bar graphs or of telemetered raw values
	//##ModelId=447316E50137
	int4 minValue;

	// Normal measurement value, e.g., used for percentage calculations.
	//##ModelId=447316EA0102
	int4 normalValue;

	// 遥信取反。
	//##ModelId=4CF4A9A7009C
	bool reverse;

	// The value to supervise.
	//##ModelId=4CF4A9480150
	int4 value;

	// The ValueAliasSet used for translation of a MeasurementValue.value to a
	// name
	//##ModelId=44731B9101E3
	//ValueAliasSet *ValueAliasSet;

	//采集设备单元号
	int4 ftuUnitId;

	//采集设备点号
	int4 ftuPointId;

	//##ModelId=447325F900F1
	//Command *Command;
	//dbReference<Command> command;RELATION(command,discrete)

	//61850规约值描述
	std::string ftuVlDesc;


	//判断更新数据是否为开关开关变位
	bool IsBreakChangeValue(unsigned char old_vl, unsigned char vl)
	{
		bool breakChangeFlag = ((old_vl != vl) && (this->measurementType == DiscreteType::_Break_State));

		return breakChangeFlag;
	}

	//返回值表示是否出现开关变位
	bool update_data(std::string tm, unsigned char vl)
	{
		//封锁值
		if (lockFlag)
			return false;

		unsigned char actual_value = vl;
		if (reverse)
			actual_value = !vl;

		bool breakChangeFlag = ((this->value != actual_value) && (this->measurementType == DiscreteType::_Break_State));

		//更新
		this->timeStamp = tm;
		this->value = actual_value;

		return breakChangeFlag;
	}

	TYPE_DESCRIPTOR((SUPERCLASS(Measurement),
					FIELD(maxValue),
					FIELD(minValue),
					FIELD(normalValue),
					FIELD(reverse),
					FIELD(value),
					KEY(ftuUnitId,INDEXED|HASHED),
					KEY(ftuPointId,INDEXED|HASHED),
					KEY(ftuVlDesc,INDEXED|HASHED)
			));

};

// Accumulator represents a accumulated (counted) Measurement, e.g. an energy
// value.
//##ModelId=40E50BAC0293
class Accumulator: public Measurement
{
public:
	// Normal value range maximum for any of the MeasurementValue.values. Used
	// for scaling, e.g. in bar graphs or of telemetered raw values.
	//##ModelId=447316F803BF
	int4 maxValue;

	// The value to supervise.
	//##ModelId=4CF4A9480150
	int4 value;

	//采集设备单元号
	int4 ftuUnitId;

	//采集设备点号
	int4 ftuPointId;

	//61850规约值描述
	std::string ftuVlDesc;


	TYPE_DESCRIPTOR((SUPERCLASS(Measurement),
					FIELD(maxValue),
					FIELD(value),
					KEY(ftuUnitId,INDEXED|HASHED),
					KEY(ftuPointId,INDEXED|HASHED),
					KEY(ftuVlDesc,INDEXED|HASHED)
			));

};

// A component part of a system extending between adjacent substations or from
// a substation to an adjacent interconnection point.
//##ModelId=35158B650032
class Line: public EquipmentContainer
{
public:
	//##ModelId=4472C1340167
	//dbReference<SubGeographicalRegion> sub_region;

	std::string voltageLevel;

	std::string	ec_type;
	std::string	ec_rid;

	real8	ctRatio;	// CT变比
	real8	ptRatio;	// PT变比

	int4	lineType;	// 线路类型
	int4	lineNo;		// 线路号

	TYPE_DESCRIPTOR((SUPERCLASS(EquipmentContainer),
		FIELD(voltageLevel),
		FIELD(ec_type),
		FIELD(ec_rid),
		FIELD(ctRatio),
		FIELD(ptRatio),
		FIELD(lineType),
		FIELD(lineNo)
		));
};

/*// An electrically connected subset of the network. Topological islands can
 // change as the current network state changes (i.e., disconnect switches,
 // breakers, etc. change state).
 //##ModelId=35158A68037A
 class TopologicalIsland: public IdentifiedObject
 {
 public:
 // A topological node belongs to a topological island
 //##ModelId=351690090169
 dbArray<dbReference<TopologicalNode> > topological_nodes;

 TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
 RELATION(topological_nodes,topological_island )));

 };

 // A set of connectivity nodes that, in the current network state, are
 // connected together through any type of closed switches, including  jumpers.
 // Topological nodes can change as the current network state changes (i.e.,
 // switches, breakers, etc. change state).
 //##ModelId=35158A7B037A
 class TopologicalNode: public IdentifiedObject
 {
 public:
 // True if node energized
 //##ModelId=3527C65D00EC
 bool energized;

 // True if node is load carrying
 //##ModelId=3527C6710177
 bool loadCarrying;

 // Net injection reactive power
 //##ModelId=3527C6860019
 ReactivePower netInjectionQ;

 // Net injection active power
 //##ModelId=3527C69A013A
 ActivePower netInjectionP;

 // The observability status of the node.
 //##ModelId=3527C6B501C5
 bool observabilityFlag;

 // Phase angle of node
 //##ModelId=3527C6C80078
 AngleRadians phaseAngle;

 // Voltage of node
 //##ModelId=3527C7160387
 Voltage voltage;

 // Several ConnectivityNode(s) may combine together to form a single
 // TopologicalNode, depending on the current state of the network.
 //##ModelId=35168F7A0155
 dbArray<dbReference<ConnectivityNode> > connectivity_nodes;

 // A topological node belongs to a topological island
 //##ModelId=351690090168
 dbReference<TopologicalIsland> topological_island;

 TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
 FIELD(energized),
 FIELD(loadCarrying),
 FIELD(netInjectionQ),
 FIELD(netInjectionP),
 FIELD(observabilityFlag),
 FIELD(phaseAngle),
 FIELD(voltage),
 RELATION(connectivity_nodes,topological_node ),
 RELATION(topological_island,topological_nodes )));
 };
 */

// Connectivity nodes are points where terminals of conducting equipment are
// connected together with zero impedance.
//##ModelId=351578890140
class ConnectivityNode: public IdentifiedObject
{
public:
	// 电源点标识
	//##ModelId=4D0090EB00CD
	//bool powerPoint;

	std::string fixed_terminal;//固定端点RID

	////##ModelId=463FD0020138
	//ConnectivityNodeContainer *EquipmentContainer;
	std::string ec_type;//连接点所属设备容器的psrType
	std::string ec_rid;//连接点所属设备容器的RID

	// Several ConnectivityNode(s) may combine together to form a single
	// TopologicalNode, depending on the current state of the network.
	//##ModelId=35168F7A0154
	//dbReference<TopologicalNode> topological_node;

	// Terminals interconnect with zero impedance at a node.  Measurements on a
	// node apply to all of its terminals.
	//##ModelId=3516909E021D
	dbArray<dbReference<Terminal> > terminals;

	//拓扑点的带电状态
	bool power_status;

	TYPE_DESCRIPTOR((SUPERCLASS(IdentifiedObject),
					//FIELD(powerPoint),
					FIELD(fixed_terminal),
					FIELD(ec_type),
					FIELD(ec_rid),
					RELATION(terminals,connectivity_node ),
					FIELD(power_status)

					//RELATION(topological_node,connectivity_nodes)
			));
};

// A generic device  designed to close, or open, or both, one or more electric
// circuits. The typeName attribute may be used to indicate that the database
// switch does not represent a corresponding real device but has been
// introduced for modeling purposes only.
//##ModelId=350375000070
class Switch: public ConductingEquipment
{
public:
	// The attribute is used in cases when no Measurement for the status value
	// is present. If the Switch has a status measurment the
	// Measurement.normalValue is expected to match with the Switch.normalOpen.
	//##ModelId=3526935003BF
	bool normalOpen;

	// The switch on count since the switch was last reset or initialized.
	//##ModelId=35269367010F
	int switchOnCount;

	// The date and time when the switch was last switched on.
	//##ModelId=3526937B038F
	std::string switchOnDate;

	std::string switchTranMode;//开关采集模式
	std::string switchCommandType;//开关遥控类型

	std::string brandFlag;//设备是否挂牌及挂牌信息

	TYPE_DESCRIPTOR((SUPERCLASS(ConductingEquipment),
					FIELD(normalOpen),
					FIELD(switchOnCount),
					FIELD(switchOnDate),
					FIELD(switchTranMode),
					FIELD(switchCommandType),
					FIELD(brandFlag)
			));

};

// A manually operated or motor operated mechanical switching device used for
// changing the connections in a circuit, or for isolating a circuit or
// equipment from a source of power. It is required to open or close circuits
// when negligible current is broken or made. 
//##ModelId=35157AEB010E
class Disconnector: public Switch
{
public:

	TYPE_DESCRIPTOR(( SUPERCLASS(Switch)));
};

// A ProtectedSwitch is a switching device that can be operated by
// ProtectionEquipment.
//##ModelId=4472D7340005
class ProtectedSwitch: public Switch
{
public:

	TYPE_DESCRIPTOR(( SUPERCLASS(Switch)));
};

// Current flow in Amps (positive flow is out of the ConductingEquipment into
// the ConnectivityNode)
//##ModelId=350375CC0100
struct CurrentFlow
{
public:
	//##ModelId=350375D402E2
	real8 value;

	//##ModelId=45C32D9E0357
	//static UnitSymbol unit;

	//##ModelId=45C218AE0250
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));

};

// A mechanical switching device capable of making, carrying, and breaking
// currents under normal circuit conditions and also making, carrying for a
// specified time, and breaking currents under specified abnormal circuit
// conditions e.g.  those of short circuit. The typeName is the type of
// breaker, e.g., oil, air blast, vacuum, SF6.
//##ModelId=35037565038C
class Breaker: public ProtectedSwitch
{
public:
	// Fault interrupting rating in amperes
	//##ModelId=350375830014
	//CurrentFlow ratedCurrent;
	real8 ratedCurrent;
	//int ratedCurrent_multiplier;

	// The transition time from open to close, in seconds
	//##ModelId=3503769B00C1
	real8 inTransitTime;

	bool faFlag;//是否为FA开关
	bool PowerPointFlag;//电源点标识
	real8 PPThreshold;//电源点容量

	int4 FTUNo;//站点号（如果站点号和开关所属站点号一致，则不需要配置）
	int4 EventID;//FTU上送的线路号

	bool WaveDirFlag;//是否参考电流方向

	real8 factorDL_I;//过流电流参数系数
	real8 factorJD_I;//接地电流数据系数

    bool breakerImportant;//负荷级别，0-普通，1-重要 zhoucw20130715

	TYPE_DESCRIPTOR((SUPERCLASS(ProtectedSwitch),
					//FIELD(ratedCurrent),
					FIELD(ratedCurrent),
					//FIELD(ratedCurrent_multiplier),
					FIELD(inTransitTime),

					FIELD(faFlag),
					FIELD(PowerPointFlag),
					FIELD(PPThreshold),

					FIELD(FTUNo),
					FIELD(EventID),

					FIELD (WaveDirFlag),
					FIELD (factorDL_I),
                    FIELD (factorJD_I),
                    FIELD (breakerImportant)));//zhoucw20130715
};

// An electrical device consisting of  two or more coupled windings, with or
// without a magnetic core, for introducing mutual coupling between electric
// circuits. Transformers can be used to control voltage and phase shift
// (active power flow). The typeName attribute indicates type of transformer.
//##ModelId=3503706E0276
class PowerTransformer: public Equipment
{
public:
	// Core shunt magnetizing susceptance in the saturation region, in per
	// cent.
	//##ModelId=350371EE0229
	real8 bmagSat;

	// The reference voltage at which the magnetizing saturation measurements
	// were made
	//##ModelId=350373E802B2
	//Voltage magBaseU;
	real8 magBaseU;
	//int4 magBaseU_multiplier;

	// Core magnetizing saturation curve knee flux level.
	//##ModelId=350374070086
	real8 magSatFlux;

	// Describes the phases carried by a power transformer. Possible values {
	// ABCN , ABC, ABN, ACN, BCN, AB, AC, BC, AN, BN, CN, A, B, C, N }.
	//##ModelId=3503743F0036
	int phases;

	// Type of transformer cooling
	//##ModelId=350374750297
	int transfCoolingType;

	//##ModelId=3A8A8F6C00F5
	int transformerType;

	// A transformer has windings
	//##ModelId=3530DBBF0121
	//TransformerWinding TransformerWindings;
	dbArray<dbReference<TransformerWinding> > transformer_windings;

	TYPE_DESCRIPTOR((SUPERCLASS(Equipment),
					FIELD(bmagSat),
					//FIELD(magBaseU),
					FIELD(magBaseU),
					//FIELD(magBaseU_multiplier),
					FIELD(magSatFlux),
					FIELD(phases),
					FIELD(transfCoolingType),
					FIELD(transformerType),
					RELATION(transformer_windings,power_tranformer )));
};

// Product of the RMS value of the voltage and the RMS value of the current
//##ModelId=350892520192
class ApparentPower
{
public:
	//##ModelId=3508925D0165
	real8 value;

	//##ModelId=45C32D890039
	//static UnitSymbol unit;

	//##ModelId=45C21651003C
	int multiplier;

	TYPE_DESCRIPTOR((FIELD(value),
					FIELD(multiplier)));

};

// The winding type, i.e., Primary, Secondary, Tertiary, Quaternary.
//##ModelId=350C71E700CC
enum WindingType
{
	//##ModelId=3B58636603DF
	primary,

	//##ModelId=3B58637002F3
	secondary,

	//##ModelId=3B58638300E7
	tertiary,

	//##ModelId=3B58638E02CE
	quaternary

};

// A winding is associated with each defined terminal of a transformer (or
// phase shifter).
//##ModelId=35158AF90190
class TransformerWinding: public ConductingEquipment
{
public:
	// Magnetizing branch susceptance (B mag).
	//##ModelId=3527C7AC0397
	//Susceptance b;
	real8 b;
	//int4 b_multiplier;

	// Zero sequence magnetizing branch susceptance.
	//##ModelId=4CF3471A00CD
	real8 b0;

	// The type of connection of the winding (e.g. Delta, Wye, zigzag)
	//##ModelId=3527C7D30307
	int connectionType;

	// The MVA that the winding can carry  under emergency conditions.
	//##ModelId=3527C7E702BF
	//ApparentPower emergencyS;
	real8 emergencyS;
	//int4 emergencyS_multiplier;

	// Magnetizing branch conductance (G mag).
	//##ModelId=3527C7F90329
	//Conductance g;
	real8 g;
	//int4 g_multiplier;

	// Zero sequence magnetizing branch conductance.
	//##ModelId=4CF347990097
	real8 g0;

	// Set if the winding is grounded.
	//##ModelId=3527C80D013D
	bool grounded;

	// Basic insulation level voltage rating
	//##ModelId=3527C7C1033D
	//Voltage insulationU;
	real8 insulationU;
	//int4 insulationU_multiplier;

	// Positive sequence series resistance of the winding.
	//##ModelId=3527C8200253
	//Resistance r;
	real8 r;
	//int4 r_multiplier;

	// Zero sequence series resistance of the winding.
	//##ModelId=3527C8320155
	//Resistance r0;
	real8 r0;
	//int4 r0_multiplier;

	// The rated voltage (phase-to-phase) of the winding, usually the same as
	// the neutral voltage.
	//##ModelId=3527C84301BD
	//Voltage ratedU;
	real8 ratedU;
	//int4 ratedU_multiplier;

	// The normal apparent power rating for the winding
	//##ModelId=3527C85702CA
	//ApparentPower ratedS;
	real8 ratedS;
	//int4 ratedS_multiplier;

	// Ground resistance path through connected grounding transformer.
	//##ModelId=3527C86A03B8
	//Resistance rground;
	real8 rground;
	//int4 rground_multiplier;

	// Apparent power that the winding can carry for a short period of time.
	//##ModelId=3527C87E029E
	//ApparentPower shortTermS;
	real8 shortTermS;
	//int4 shortTermS_multiplier;

	// The type of winding, i.e., Primary, Secondary, Tertiary, Quaternary.
	//##ModelId=3527C8A90174
	int windingType;

	// Positive sequence series reactance of the winding.
	//##ModelId=3527C8D100D1
	//Reactance x;
	real8 x;
	//int4 x_multiplier;

	// Zero sequence series reactance of the winding.
	//##ModelId=3527C8E50030
	//Reactance x0;
	real8 x0;
	//int4 x0_multiplier;

	// Ground reactance path through connected grounding transformer.
	//##ModelId=3527C8FE0202
	//Reactance xground;
	real8 xground;
	//int4 xground_multiplier;

	// A transformer has windings
	//##ModelId=3530DBBF0117
	//PowerTransformer *PowerTransformer;
	dbReference<PowerTransformer> power_tranformer;

	TYPE_DESCRIPTOR((SUPERCLASS(ConductingEquipment),
					FIELD (b),
					//FIELD (b_multiplier),
					FIELD (b0),

					FIELD(connectionType),
					FIELD(emergencyS),
					//FIELD(emergencyS_multiplier),

					FIELD(g),
					//FIELD(g_multiplier),
					FIELD(g0),
					FIELD(grounded),
					FIELD(insulationU),
					//FIELD(insulationU_multiplier),


					FIELD(r),
					//FIELD(r_multiplier),
					FIELD(r0),
					//FIELD(r0_multiplier),

					FIELD(ratedU),
					//FIELD(ratedU_multiplier),
					FIELD(ratedS),
					//FIELD(ratedS_multiplier),
					FIELD(rground),
					//FIELD(rground_multiplier),
					FIELD(shortTermS),
					//FIELD(shortTermS_multiplier),
					FIELD(windingType),

					FIELD(x),
					//FIELD(x_multiplier),
					FIELD(x0),
					//FIELD(x0_multiplier),
					FIELD(xground),
					//FIELD(xground_multiplier),

					RELATION(power_tranformer,transformer_windings)));
};

//公式定义
class FormulaDefinition
{
public:
	std::string mRID;
	std::string name;
	std::string formula;//公式定义

	std::string type;//公式类型，Analog表示遥测，Discrete表示遥信

	std::string ce_type;//所属设备类型
	std::string ce_rid;//所属设备RID

	std::string descr;//公式描述

	dbReference<Analog> analog;//关联Analog

	TYPE_DESCRIPTOR((KEY(mRID,INDEXED|HASHED),
					FIELD (name),
					FIELD (formula),

					FIELD (type),

					FIELD (ce_type),
					FIELD (ce_rid),

					FIELD (descr),

					OWNER(analog, analog_formula)
			));
};

//变量定义
class VariableDefinition
{
public:
	std::string mRID;
	std::string name;
	std::string variable;//公式变量

	int4 measure_type;//量测类型
	std::string measure_rid;//量测rid

	TYPE_DESCRIPTOR((KEY(mRID,INDEXED|HASHED),
					FIELD (name),
					FIELD (variable),
					FIELD (measure_type),
					FIELD (measure_rid)
			));
};

// 档位库
class Step
{
public:
	std::string	mRID;			// 档位id
	std::string	name;			// 档位名称
	std::string	psr_type;		// 所属容器类型
	std::string psr_rid;		// 所属容器id
	int			compute_type;	// 计算方法
	int			step;			// 当前档位值
	int			highStep;		// 档位上限
	int			lowStep;		// 档位下限

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(name),
		FIELD(psr_type),
		FIELD(psr_rid),
		FIELD(compute_type),
		FIELD(step),
		FIELD(highStep),
		FIELD(lowStep)
		));
};

// 告警描述表
class ConfigFaultDesc
{
public:
	std::string	mRID;					// 告警描述id
	std::string	manufactId;				// 设备厂商id
	int			lineType;				// 线路类型
	int			protectTypeNo;			// 保护类型号
	bool		protectHasValue;		// 有无事项值
	std::string	name;					// 名称
	std::string	units;					// 单位
	real8		modulus;				// 系数

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(manufactId),
		FIELD(lineType),
		FIELD(protectTypeNo),
		FIELD(protectHasValue),
		FIELD(name),
		FIELD(units),
		FIELD(modulus)
		));
};

// 遥信描述表
class ConfigYxDesc
{
public:
	std::string	mRID;		// 遥信描述id
	int			yxType;		// 遥信类型
	int			yxValue;	// 遥信值
	std::string	name;		// 名称

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(yxType),
		FIELD(yxValue),
		FIELD(name)
		));
};

// 设备厂家表
class DevManufacturer
{
public:
	std::string	mRID;		// 厂家id
	std::string	name;		// 厂家名称
	dbArray< dbReference<DevDeviceType> >	deviceTypes;	// 设备型号
	dbArray< dbReference<RemoteUnit> >	remoteUnits;		// 单元
	dbArray< dbReference<DevSettingLine> >	settingLines;	// 整定线路
	dbArray< dbReference<DevSettingType> >	settingTypes;	// 整定类型

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(name),
		RELATION(deviceTypes, manufacturer),
		RELATION(remoteUnits, manufacturer),
		RELATION(settingLines, manufacturer),
		RELATION(settingTypes, manufacturer)
		));
};

// 设备型号表
class DevDeviceType
{
public:
	std::string	mRID;		// 设备型号id
	std::string	name;		// 设备型号名称
	dbReference<DevManufacturer>	manufacturer;	// 所属厂家
	dbArray< dbReference<RemoteUnit> > remoteUnits;	// 单元
	dbArray< dbReference<DevSettingLine> >	settingLines;	// 整定线路
	dbArray< dbReference<DevSettingType> >	settingTypes;	// 整定类型

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(name),
		RELATION(manufacturer, deviceTypes),
		RELATION(remoteUnits, deviceType),
		RELATION(settingLines, deviceType),
		RELATION(settingTypes, deviceType)
		));
};

// 整定线路类型表
class DevSettingLine
{
public:
	std::string	mRID;			// 设备型号id
	int			no;				// 序号
	int			lineType;		// 线路类型
	int			settingWay;		// 整定方式
	int			startProNo;		// 起始保护号
	int			endProNo;		// 终止保护号
	int			protectNum;		// 保护个数
	int			fc;				// 功能码
	int			proTypeVal;		// 保护类型大小
	int			funType;		// 功能类型

	dbReference<DevManufacturer>	manufacturer;	// 设备厂商
	dbReference<DevDeviceType>		deviceType;		// 设备类型

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(no),
		FIELD(lineType),
		FIELD(settingWay),
		FIELD(startProNo),
		FIELD(endProNo),
		FIELD(protectNum),
		FIELD(fc),
		FIELD(proTypeVal),
		FIELD(funType),
		RELATION(manufacturer, settingLines),
		RELATION(deviceType, settingLines)
		));
};

// 整定类型表
class DevSettingType
{
public:
	std::string	mRID;			// 设备型号id
	int			no;				// 序号
	int			lineType;		// 线路类型
	int			settingNo;		// 整定类型号
	std::string	settingName;	// 整定类型名称
	int			proTypeVal;		// 保护类型号
	int			settingType;	// 定值类型
	int			saveType;		// 存储类型
	int			startSingle;	// 起始(单一)
	int			endSingle;		// 终止(单一)
	int			startLine;		// 起始(线路)
	int			endLine;		// 终止(线路)
	int			infoAddr;		// 信息地址
	real8		modulus;		// 系数
	std::string	units;			// 单位
	int			settingLevel;	// 整定级别
	int			funType;		// 功能类型
	int			qpm;			// QPM

	dbReference<DevManufacturer>	manufacturer;	// 设备厂商
	dbReference<DevDeviceType>		deviceType;		// 设备类型

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(no),
		FIELD(lineType),
		FIELD(settingNo),
		FIELD(settingName),
		FIELD(proTypeVal),
		FIELD(settingType),
		FIELD(saveType),
		FIELD(startSingle),
		FIELD(endSingle),
		FIELD(startLine),
		FIELD(endLine),
		FIELD(infoAddr),
		FIELD(modulus),
		FIELD(units),
		FIELD(settingLevel),
		FIELD(funType),
		FIELD(qpm),
		RELATION(manufacturer, settingTypes),
		RELATION(deviceType, settingTypes)
		));
};

// 分区表
class FepPartition
{
public:
	std::string	mRID;								// 分区id
	std::string	name;								// 分区名称
	dbArray< dbReference<FepSection> > sections;	// 分段
	dbArray< dbReference<FepProtocol> > protocols;	// 规约

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(name),
		RELATION(sections, partition),
		RELATION(protocols, partition)
		));
};

// 分段表
class FepSection
{
public:
	std::string	mRID;						// 分段id
	std::string	name;						// 分段名称
	dbReference<FepPartition> partition;	// 分区id
	dbArray< dbReference<FepProtocol> > protocols;	// 规约

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(name),
		RELATION(partition, sections),
		RELATION(protocols, section)
		));
};

// 规约表
class FepProtocol
{
public:
	std::string	mRID;						// 规约id
	int			protocolNo;					// 规约号码
	std::string	name;						// 规约名称
	int			type;						// 规约类型
	bool		isUse;						// 是否使用
	std::string	paramLine;					// 参数
	int			commCount;					// 通道个数
	int			unitCount;					// 单元个数
	dbReference<FepPartition>	partition;	// 所属分区
	dbReference<FepSection>		section;	// 所属分段
	dbArray< dbReference<FepChannel> > channels;	// 通道

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(protocolNo),
		FIELD(name),
		FIELD(type),
		FIELD(isUse),
		FIELD(paramLine),
		FIELD(commCount),
		FIELD(unitCount),
		RELATION(partition, protocols),
		RELATION(section, protocols),
		RELATION(channels, protocol)
		));
};

// 通道表
class FepChannel
{
public:
	std::string	mRID;			// 通道id
	int			channelId;		// 通道号
	std::string	param;			// 参数
	dbReference<FepProtocol>	protocol;		// 规约id

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(channelId),
		FIELD(param),
		RELATION(protocol, channels)
		));
};

// 规约单元关联表
class FepProtocolUnit
{
public:
	std::string	mRID;			// id
	std::string	protocolId;		// 所属规约
	int			deviceId;		// 所属单元

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(protocolId),
		FIELD(deviceId)
		));
};

// 角色表
class SystemRole
{
public:
	std::string	mRID;			// 角色id
	std::string	roleName;		// 名称
	std::string	permission;		// 权限状态
	std::string roleDesc;		// 描述

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(roleName),
		FIELD(permission),
		FIELD(roleDesc)
		));
};

// 用户表
class SystemUser
{
public:
	std::string	mRID;			// 用户id
	std::string	loginId;		// 登录id
	std::string	loginName;		// 登录名称
	std::string userPwd;		// 密码
	std::string roleId;			// 角色
	std::string userName;		// 名称

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(loginId),
		FIELD(loginName),
		FIELD(userPwd),
		FIELD(roleId),
		FIELD(userName)
		));
};

// 电话号码表
class SystemPhone
{
public:
	std::string	mRID;		// id
	std::string	name;		// 名称
	std::string	phone;		// 电话号码

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(name),
		FIELD(phone)
		));
};

// 短信配置表
class SystemMsgCfg
{
public:
	std::string	mRID;		// id
	int			type;		// 类型
	int			state;		// 状态

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(type),
		FIELD(state)
		));
};

// 录波参数表
class WavWaveConfig
{
public:
	std::string	mRID;			// id
	int			deviceId;		// 单元号
	int			configNo;		// 序号
	std::string dataName;		// 数据名称
	int			maxVal;			// 最大值
	std::string unit;			// 单位
	real8		modulus;		// 系数
	std::string color;			// 曲线颜色
	int			belongBmp;		// 所属画面
	int			protectLineNo;	// 所属线路号
	int			channelNo;		// 通道号

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(deviceId),
		FIELD(configNo),
		FIELD(dataName),
		FIELD(maxVal),
		FIELD(unit),
		FIELD(modulus),
		FIELD(color),
		FIELD(belongBmp),
		FIELD(protectLineNo),
		FIELD(channelNo)
		));
};

// 趋势曲线参数表
class WavTrendCurve
{
public:
	std::string	mRID;			// id
	int			deviceId;		// 单元号
	std::string	ycDesc;			// 描述
	int			no;				// 序号
	int			ycIndex;		// 遥测点号
	real8		modulus;		// 系数
	int			analogType;		// 遥测类型
	std::string curveColor;		// 曲线颜色
	int			coordinateNo;	// 坐标系号
	std::string reserved;		// 预留

	TYPE_DESCRIPTOR((
		KEY(mRID, INDEXED | HASHED),
		FIELD(deviceId),
		FIELD(ycDesc),
		FIELD(no),
		FIELD(ycIndex),
		FIELD(modulus),
		FIELD(analogType),
		FIELD(curveColor),
		FIELD(coordinateNo),
		FIELD(reserved)
		));
};


#endif

