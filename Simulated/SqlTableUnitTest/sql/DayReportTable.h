/********************************************************************
	created:	2017/11/08
	created:	8:11:2017   15:16
	filename: 	D:\KH-8100T\project\src\datasvr\opt\hisdb\DayReportTable.h
	file path:	D:\KH-8100T\project\src\datasvr\opt\hisdb
	file base:	DayReportTable
	file ext:	h
	author:		Masf
	
	purpose:	日报处理类
*********************************************************************/

#include "KHSqlTable.h"

struct  HS_DayData
{
	std::string   m_date;
	std::string   m_resourceID;
	std::string	  m_name;
	double        m_values[25];
	double        m_maxValue;
	double        m_minValue;
	double        m_avgValue;
	std::string   m_maxTime;
	std::string   m_minTime;
};

class CDayReportTable : public CKHSqlTable
{
public:
	CDayReportTable();

	enum DayReportTableColumn
	{
		DayReport_Column_Id,
		DayReport_Column_Tbdate,
		DayReport_Column_ResourceId,
		DayReport_Column_Val0,
		DayReport_Column_Val1,
		DayReport_Column_Val2,
		DayReport_Column_Val3,
		DayReport_Column_Val4,
		DayReport_Column_Val5,
		DayReport_Column_Val6,
		DayReport_Column_Val7,
		DayReport_Column_Val8,
		DayReport_Column_Val9,
		DayReport_Column_Val10,
		DayReport_Column_Val11,
		DayReport_Column_Val12,
		DayReport_Column_Val13,
		DayReport_Column_Val14,
		DayReport_Column_Val15,
		DayReport_Column_Val16,
		DayReport_Column_Val17,
		DayReport_Column_Val18,
		DayReport_Column_Val19,
		DayReport_Column_Val20,
		DayReport_Column_Val21,
		DayReport_Column_Val22,
		DayReport_Column_Val23,
		DayReport_Column_Val24,
		DayReport_Column_MaxVal,
		DayReport_Column_MinVal,
		DayReport_Column_AvgVal,
		DayReport_Column_MaxTime,
		DayReport_Column_MinTime,
		DayReport_Column_Count
	};

public:
	// 初始化数据表的列名
	virtual void initTableField();

	// 获取数据表名
	virtual QString	getTableName();

	// 插入日报数据
	bool	insertDayReportData(const HS_DayData& dayData);

	// 单元测试
	void	unitTest();
};