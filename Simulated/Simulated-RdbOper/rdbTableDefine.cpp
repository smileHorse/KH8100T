
#include <QtWidgets/QtWidgets>

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

RdbStruct RdbTableFactory::_rdbStruct = RdbStruct();

// 获取工厂名
std::string RdbTableFactory::getTableName(int index)
{
	//if (index < 0 || index >= RdbTable_Count)
	//{
	//	throw "getTableName() out of range";
	//}

	//return sat_table_names[index];

	initRdbStruct();
	return _rdbStruct.tables[index].name.toStdString();
}

void RdbTableFactory::initRdbStruct()
{
	if (!_rdbStruct.isEmpty())
	{
		return;
	}

	XmlStreamReader reader;
	if(!reader.readFile("conf/rdbtable.xml", _rdbStruct))
	{
		QMessageBox::warning(0, QStringLiteral("初始实时库结构"), QStringLiteral("初始失败"));
		return;
	}
}

