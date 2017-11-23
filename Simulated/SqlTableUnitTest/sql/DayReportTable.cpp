/********************************************************************
	created:	2017/11/08
	created:	8:11:2017   15:16
	filename: 	D:\KH-8100T\project\src\datasvr\opt\hisdb\DayReportTable.cpp
	file path:	D:\KH-8100T\project\src\datasvr\opt\hisdb
	file base:	DayReportTable
	file ext:	h
	author:		Masf
	
	purpose:	日报处理类
*********************************************************************/

#include "DayReportTable.h"

#include <iostream>

using namespace std;

CDayReportTable::CDayReportTable()
{
	initTableField();
}

// 初始化数据表的列名
void CDayReportTable::initTableField()
{
	QVector<QString> values;
	values	<< "id" << "tbdate" << "resourceid" 
		<< "val0"  <<"val1" << "val2" << "val3" << "val4" << "val5" << "val6" << "val7" << "val8" << "val9" 
		<< "val10" << "val11" << "val12" << "val13" << "val14" << "val15" << "val16" << "val17" << "val18" << "val19" 
		<< "val20" << "val21" << "val22" << "val23" << "val24" 
		<< "maxval" << "minval" << "avgval" << "maxtime" << "mintime";

	setTableField(values);
}

// 获取数据表名
QString CDayReportTable::getTableName()
{
	return "t_rep_daytb";
}

// 插入日报数据
bool CDayReportTable::insertDayReportData( const HS_DayData& dayData )
{
	// 执行插入操作
	QString sql = makeInsertSql(DayReport_Column_Count);

	QVector<QVariant> values;
	values << generateUUID() << convertStdStrToQStr(dayData.m_date) << convertStdStrToQStr(dayData.m_resourceID);
	int size = sizeof(dayData.m_values) / sizeof(dayData.m_values[0]);
	for (int i = 0; i < size; ++i)
	{
		values << dayData.m_values[i];
	}
	values << dayData.m_maxValue << dayData.m_minValue << dayData.m_avgValue 
		   << convertStdStrToQStr(dayData.m_maxTime) << convertStdStrToQStr(dayData.m_minTime);

	if (!exec(sql, values))
	{
		return false;
	}
	return true;
}

// 单元测试
void CDayReportTable::unitTest()
{
	// 测试insert语句
	cout << "-- 开始测试insert语句 --" << endl;
	QString sql = makeInsertSql(DayReport_Column_Count);
	cout << sql.toStdString() << endl;

	cout << "-- 结束测试insert语句 --" << endl;

	// 测试delete语句
	cout << "-- 开始测试delete语句 --" << endl;
	sql = makeDeleteSql();
	cout << sql.toStdString() << endl;

	IntSeq conditions;
	conditions.push_back(3);
	conditions.push_back(5);
	sql = makeDeleteSql(conditions);
	cout << sql.toStdString() << endl;

	cout << "-- 结束测试delete语句 --" << endl;

	// 测试update语句
	cout << "-- 开始测试update语句 --" << endl;
	IntSeq sets;
	sets.push_back(2);
	sets.push_back(4);
	sql = makeUpdateSql(sets);
	cout << sql.toStdString() << endl;

	sql = makeUpdateSql(sets, conditions);
	cout << sql.toStdString() << endl;

	cout << "-- 结束测试update语句 --" << endl;

	// 测试select语句
	cout << "-- 开始测试select语句 --" << endl;
	sql = makeSelectSql();
	cout << sql.toStdString() << endl;

	sql = makeSelectSql(sets);
	cout << sql.toStdString() << endl;

	sql = makeSelectSql(sets, conditions);
	cout << sql.toStdString() << endl;

	sql = makeSelectSql(sets, conditions, true);
	cout << sql.toStdString() << endl;

	sql = makeSelectSql(sets, conditions, true, 2);
	cout << sql.toStdString() << endl;

	sql = makeSelectSql(sets, conditions, true, 2, FieldOrder_Asc);
	cout << sql.toStdString() << endl;

	sql = makeSelectSql(sets, conditions, true, 2, FieldOrder_Asc, true);
	cout << sql.toStdString() << endl;

	sql = makeSelectSql(sets, conditions, true, 2, FieldOrder_Asc, true, 1, 1);
	cout << sql.toStdString() << endl;


	cout << "-- 结束测试select语句 --" << endl;
}
