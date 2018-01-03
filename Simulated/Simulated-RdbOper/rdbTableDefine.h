#ifndef RDBTABLEDEFINE_H
#define RDBTABLEDEFINE_H

#include <map>
#include <string>
#include <vector>

#include "xmlStreamReader.h"

using namespace std;

// 实时库表工厂类
class RdbTableFactory
{
public:
	enum RdbTableEnum
	{
		RdbTable_GeographicalRegion = 0,
		RdbTable_SubGeographicalRegion,
		RdbTable_Area,
		RdbTable_RemoteUnit,
		RdbTable_AnalogUnitPoint,
		RdbTable_DiscreteUnitPoint,
		RdbTable_ControlUnitPoint,
		RdbTable_Analog,
		RdbTable_Discrete,
		RdbTable_Accumulator,
		RdbTable_Command,
		RdbTable_AnalogCurveData,
		RdbTable_BaseVoltage,
		RdbTable_Substation,
		RdbTable_VoltageLevel,
		RdbTable_BusbarSection,
		RdbTable_Line,
		RdbTable_ACLineSegment,
		RdbTable_Breaker,
		RdbTable_Disconnector,
		RdbTable_PowerTransformer,
		RdbTable_TransformerWinding,
		RdbTable_ConnectivityNode,
		RdbTable_Terminal,
		RdbTable_FormulaDefinition,
		RdbTable_VariableDefinition,
		RdbTable_Count
	};

public:
	// 获取工厂名
	static string getTableName(int index);

	// 初始化实时库结构
	static void initRdbStruct();

private:
	static string	sat_table_names[RdbTable_Count];

	static RdbStruct	_rdbStruct;
};

#endif