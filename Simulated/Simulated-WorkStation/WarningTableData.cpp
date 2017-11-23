/********************************************************************
	created:	2017/11/14
	created:	14:11:2017   10:09
	filename: 	D:\KH-8100T\project\src\datasvr\opt\warningI\WarningTableData.cpp
	file path:	D:\KH-8100T\project\src\datasvr\opt\warningI
	file base:	WarningTableData
	file ext:	h
	author:		Masf
	
	purpose:	告警表数据类型
*********************************************************************/

#include "WarningTableData.h"

#include <IceUtil/IceUtil.h>


// 将长整型转换为日期类型
QString CAlarmTableDataImpl::convertDateTimeToQStr( long long timestamp )
{
	IceUtil::Time time = IceUtil::Time::milliSeconds(timestamp);
	string strDateTime = "";
	return QString().fromStdString(strDateTime);
}

// 将标准string转换为QString
QString CAlarmTableDataImpl::convertStdStrToQStr( const string& str )
{
	return QString().fromStdString(str); 
}

// 将标准string转换为double
double CAlarmTableDataImpl::convertStdStrToDouble( const string& str )
{
	return QString().fromStdString(str).toDouble();
}

// 将标准string转换为int
int CAlarmTableDataImpl::convertStdStrToInt( const string& str )
{
	return QString().fromStdString(str).toInt();
}

// 将int转换为QString
QString CAlarmTableDataImpl::convertIntToQStr( int value )
{
	return QString("%1").arg(value);
}

/************************************************************************/
/*告警表通用数据				                                        */
/************************************************************************/

CAlarmTableData::CAlarmTableData()
	: id(""), datet(""), warnSource(""), warnLevel("")
{}


void CAlarmTableData::transformToVector( QVector<QVariant>& values ) const
{
	values.clear();
	values << id << datet;
	transformToVectorImpl(values);
	values << warnSource << warnLevel;
}


void CAlarmTableData::transformToQString( QString& str ) const
{
	QVector<QVariant> values;
	transformToVector(values);
	QStringList fields;
	getDataField(fields);
	for (int i = 0; i < values.size(); ++i)
	{
		str += (i == 0 ? "" : ", ");
		str += fields.at(i) + ": " + values.at(i).toString();
	}
}


void CAlarmTableData::getDataField( QStringList& strList ) const
{
	strList.clear();
	strList << "id" << datet;
	getDataFieldImpl(strList);
	strList << "warnSource" << "warnLevel";
}

/************************************************************************/
/*遥测越限表	t_alarm_limittb                                         */
/************************************************************************/
CAlarmLimitTableData::CAlarmLimitTableData()
	: station(""), equipName(""), line(""), dataName(""), state(""),
	limitVal(0.0), currVal(0.0), sms_flag("")
{}

void CAlarmLimitTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << station << equipName << line << dataName << state << limitVal 
		   << currVal << sms_flag;
}


void CAlarmLimitTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "station" << "equipName" << "line" << "dataName" << "state" << "limitVal" 
		<< "currVal" << "sms_flag";
}

int CAlarmLimitTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Limit;
}

/************************************************************************/
/*SOE表	t_alarm_soetb                                         */
/************************************************************************/
CAlarmSoeTableData::CAlarmSoeTableData()
	: station(""), equipName(""), line(""), dataName(""), type(""), state(0), sms_flag("")
{}

void CAlarmSoeTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << station << equipName << line << dataName << type << state << sms_flag;
}


void CAlarmSoeTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "station" << "equipName" << "line" << "dataName" << "type" 
		<< "state" << "sms_flag";
}

int CAlarmSoeTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Soe;
}

/************************************************************************/
/*遥信变位表	t_alarm_yxtb	                                        */
/************************************************************************/
CAlarmYxTableData::CAlarmYxTableData()
	: station(""), equipName(""), line(""), dataName(""), type(""), state(0)
{}

void CAlarmYxTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << station << equipName << line << dataName << type << state;
}


void CAlarmYxTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "station" << "equipName" << "line" << "dataName" << "type" 
		<< "state";
}

int CAlarmYxTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Yx;
}

/************************************************************************/
/*操作事项表	t_alarm_opertb	                                        */
/************************************************************************/
CAlarmOperTableData::CAlarmOperTableData()
	: type(0), station(""), equipName(""), line(""), dataName(""), 
	host(""), oper(""), content("")
{}

void CAlarmOperTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << type << station << equipName << line << dataName << host << oper << content;
}


void CAlarmOperTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "type" << "station" << "equipName" << "line" << "dataName" << "host" 
		<< "oper" << "content";
}

int CAlarmOperTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Oper;
}

/************************************************************************/
/*系统事项表	t_alarm_syseventtb	                                        */
/************************************************************************/
CAlarmSysEventTableData::CAlarmSysEventTableData()
	: type(""), host(""), oper(""), content("")
{}

void CAlarmSysEventTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << type << host << oper << content;
}


void CAlarmSysEventTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "type" << "host" << "oper" << "content";
}

int CAlarmSysEventTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_SysEvent;
}

/************************************************************************/
/*保护事项表	t_alarm_relaytb	                                        */
/************************************************************************/
CAlarmRelayTableData::CAlarmRelayTableData()
	: deviceId(0), station(""), equipName(""), line(""), dataName(""), content(""),
	manufact(""), lineType(""), proTypeNo(0), proTypeName(""), isEvent(""), modulus(0.0),
	units("")
{}

void CAlarmRelayTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << deviceId << station << equipName << line << dataName << content 
		   << manufact << lineType << proTypeNo << proTypeName << isEvent 
		   << modulus << units;
}


void CAlarmRelayTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "deviceId" << "station" << "equipName" << "line" << "dataName" << "content" 
		<< "manufact" << "lineType" << "proTypeNo" << "proTypeName" << "isEvent" 
		<< "modulus" << "units";
};

int CAlarmRelayTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Relay;
}

/************************************************************************/
/*单元状态表	t_alarm_rtutb                                         */
/************************************************************************/
CAlarmRtuTableData::CAlarmRtuTableData()
	: deviceId(0), station(""), equipName(""), line(""), state("")
{}

void CAlarmRtuTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << deviceId << station << equipName << line << state;
}


void CAlarmRtuTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "deviceId" << "station" << "equipName" << "line" << "state";
}

int CAlarmRtuTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Rtu;
}

/************************************************************************/
/*图形操作表	t_alarm_graphtb                                         */
/************************************************************************/
CAlarmGraphTableData::CAlarmGraphTableData()
	: graphId(""), graphName(""), graphType(""), state("")
{}

void CAlarmGraphTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << graphId << graphName << graphType << state;
}


void CAlarmGraphTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "graphId" << "graphName" << "graphType" << "state";
}

int CAlarmGraphTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Graph;
}

/************************************************************************/
/*Fa表	t_alarm_fatb                                         */
/************************************************************************/
CAlarmFaTableData::CAlarmFaTableData()
	: faultId(""), content("")
{}

void CAlarmFaTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << faultId << content;
}


void CAlarmFaTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "faultId" << "content";
}

int CAlarmFaTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Fa;
}

/************************************************************************/
/*故障事项表	t_alarm_faulttb                                         */
/************************************************************************/
CAlarmFaultTableData::CAlarmFaultTableData()
	: line(""), station(""), equipName(""), faultType(""), selLineReliability(0), iValues(""), 
	factorJD(""), sms_flag("")
{}

void CAlarmFaultTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << line << station << equipName << faultType << selLineReliability << iValues 
		   << factorJD << sms_flag;
}


void CAlarmFaultTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "line" << "station" << "equipName" << "faultType" << "selLineReliability" << "iValues" 
			<< "factorJD" << "sms_flag";
}

int CAlarmFaultTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Fault;
}


/************************************************************************/
/*录波事项表	t_alarm_rfwtb                                         */
/************************************************************************/
CAlarmRfwTableData::CAlarmRfwTableData()
	: station(""), equipName(""), line(""), faultType(-1), type(-1), reason(-1)
{
}

void CAlarmRfwTableData::transformToVectorImpl( QVector<QVariant>& values ) const
{
	values << station << equipName << line << faultType << type << reason;
}


void CAlarmRfwTableData::getDataFieldImpl( QStringList& strList ) const
{
	strList << "station" << "equipName" << "line" << "faultType" << "type" << "reason";
}

int CAlarmRfwTableData::getAlarmTableDataType() const
{
	return CWarningDataManager::Table_Alarm_Rfw;
}
