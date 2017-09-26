#include "FdbTableDefine.h"

//需要实际创建的fastdb数据库表


REGISTER( GeographicalRegion );
REGISTER( SubGeographicalRegion );

REGISTER( RemoteUnit );

REGISTER( AnalogUnitPoint );
REGISTER( DiscreteUnitPoint );
REGISTER( ControlUnitPoint );

REGISTER( Terminal );
REGISTER( BaseVoltage);
REGISTER( Substation );
REGISTER( VoltageLevel );

REGISTER( Command);

REGISTER( Analog );
REGISTER( Discrete );
REGISTER( Accumulator );

REGISTER( Line);

REGISTER( ConnectivityNode);

REGISTER( BusbarSection);

REGISTER( ACLineSegment);

REGISTER( Disconnector);
REGISTER( Breaker);

REGISTER( PowerTransformer);
REGISTER( TransformerWinding);

REGISTER( FormulaDefinition);
REGISTER( VariableDefinition);
//zhoucw20121218---------------------------->>
//REGISTER( ManufacturerDevice);
//REGISTER( RemoteUnitMap);
//REGISTER( ProtectModifyValue);
REGISTER( ProtectEventT);
//zhoucw20121218----------------------------<<
