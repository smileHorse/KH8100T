
#include "rdbTableDefine.h"

string RdbTableFactory::sat_table_names[] = 
{
	"GeographicalRegion",
	"SubGeographicalRegion",
	"Area",
	"RemoteUnit",
	"AnalogUnitPoint",
	"DiscreteUnitPoint",
	"ControlUnitPoint",
	"Analog",
	"Discrete",
	"Accumulator",
	"Command",
	"AnalogCurveData",
	"BaseVoltage",
	"Substation",
	"VoltageLevel",
	"BusbarSection",
	"Line",
	"ACLineSegment",
	"Breaker",
	"Disconnector",
	"PowerTransformer",
	"TransformerWinding",
	"ConnectivityNode",
	"Terminal",
	"FormulaDefinition",
	"VariableDefinition"
};

// 获取工厂名
std::string RdbTableFactory::getTableName(int index)
{
	if (index < 0 || index >= RdbTable_Count)
	{
		throw "getTableName() out of range";
	}

	return sat_table_names[index];
}
