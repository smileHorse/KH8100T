#include "FdbTableDefine.h"

//需要实际创建的fastdb数据库表

REGISTER( FATopo );

REGISTER( GeographicalRegion );
REGISTER( SubGeographicalRegion );

REGISTER( Area );

REGISTER( RemoteUnit );

REGISTER( AnalogUnitPoint );
REGISTER( DiscreteUnitPoint );
REGISTER( ControlUnitPoint );
REGISTER( AccumulatorUnitPoint );
REGISTER( ProtectValue );

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

REGISTER( AnalogCurveData);

REGISTER( AnalogSectionData);
REGISTER( DiscreteSectionData);
REGISTER( AccumulatorSectionData);

REGISTER( FormulaDefinition);
REGISTER( VariableDefinition);

REGISTER(Step);
REGISTER(ControlLockingFormula);

REGISTER(ConfigFaultDesc);
REGISTER(ConfigYxDesc);
REGISTER(AccumulatorTimeSegment);

REGISTER(DevManufacturer);
REGISTER(DevDeviceType);
REGISTER(DevSettingLine);
REGISTER(DevSettingType);

REGISTER(FepPartition);
REGISTER(FepSection);
REGISTER(FepProtocol);
REGISTER(FepChannel);
REGISTER(FepProtocolUnit);

REGISTER(SystemRole);
REGISTER(SystemUser);
REGISTER(SystemPhone);
REGISTER(SystemMsgCfg);
REGISTER(GraphFile);

REGISTER(WavWaveConfig);
REGISTER(WavTrendCurve);

REGISTER(SecurityGroups);
REGISTER(SecurityGroupsUsers);
REGISTER(SecurityGroupsStations);