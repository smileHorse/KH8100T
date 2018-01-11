#ifndef RDBTABLEDEFINE_H
#define RDBTABLEDEFINE_H

#include <QtCore/QtCore>
#include <string>
#include <vector>

#include "xmlStreamReader.h"

using namespace std;
typedef vector<string> StringVector;

/************************************************************************/
/* 基类表                                                               */
/************************************************************************/
class BaseTable
{
public:
	virtual QString getTableName() const = 0;
	virtual QString getTableDisplayName() const = 0;

	virtual void getRandomValues(StringVector& values);
	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);

protected:
	void generateCommonRandomValue(const RdbFieldStruct& field, StringVector& values);

	// 产生mRID
	string generateMRID();

	// 产生name
	string generateName();

	// 获取引用的对象的mRID
	string getMRIDOfReferenceObject(const QString& tableName, int index, int perValue);

};
typedef QSharedPointer<BaseTable> BaseTablePtr;

/************************************************************************/
/* 区域表                                                               */
/************************************************************************/
class GeographicalRegionTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<GeographicalRegionTable> GeographicalRegionTablePtr;

/************************************************************************/
/* 子区域表                                                             */
/************************************************************************/
class SubGeographicalRegionTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<SubGeographicalRegionTable> SubGeographicalRegionTablePtr;

/************************************************************************/
/* 区间表                                                             */
/************************************************************************/
class AreaTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<AreaTable> AreaTablePtr;

/************************************************************************/
/* 变电站表                                                             */
/************************************************************************/
class SubstationTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<SubstationTable> SubstationTablePtr;

/************************************************************************/
/* 母线表                                                             */
/************************************************************************/
class BusbarSectionTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<BusbarSectionTable> BusbarSectionTablePtr;

/************************************************************************/
/* 线路表                                                             */
/************************************************************************/
class LineTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<LineTable> LineTablePtr;

/************************************************************************/
/* 断路器表                                                             */
/************************************************************************/
class BreakerTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<BreakerTable> BreakerTablePtr;

/************************************************************************/
/* 刀闸表                                                             */
/************************************************************************/
class DisconnectorTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<DisconnectorTable> DisconnectorTablePtr;

/************************************************************************/
/* 变压器表                                                             */
/************************************************************************/
class PowerTransformerTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<PowerTransformerTable> PowerTransformerTablePtr;

/************************************************************************/
/* 绕组表                                                             */
/************************************************************************/
class TransformerWindingTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<TransformerWindingTable> TransformerWindingTablePtr;

/************************************************************************/
/* 单元表                                                             */
/************************************************************************/
class RemoteUnitTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);

};
typedef QSharedPointer<RemoteUnitTable> RemoteUnitTablePtr;

/************************************************************************/
/* 遥测点表                                                             */
/************************************************************************/
class AnalogUnitPointTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<AnalogUnitPointTable> AnalogUnitPointTablePtr;

/************************************************************************/
/* 遥信点表                                                             */
/************************************************************************/
class DiscreteUnitPointTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<DiscreteUnitPointTable> DiscreteUnitPointTablePtr;

/************************************************************************/
/* 遥控点表                                                             */
/************************************************************************/
class ControlUnitPointTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);
};
typedef QSharedPointer<ControlUnitPointTable> ControlUnitPointTablePtr;

/************************************************************************/
/* 设备厂家表                                                             */
/************************************************************************/
class DevManufacturerTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

};
typedef QSharedPointer<DevManufacturerTable> DevManufacturerTablePtr;

/************************************************************************/
/* 设备类型表                                                             */
/************************************************************************/
class DevDeviceTypeTable : public BaseTable
{
public:
	virtual QString getTableName() const;
	virtual QString getTableDisplayName() const;

	virtual void getRandomValuesImpl(const RdbFieldStructList& fields, StringVector& values);

};
typedef QSharedPointer<DevDeviceTypeTable> DevDeviceTypeTablePtr;

#endif