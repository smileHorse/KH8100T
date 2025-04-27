/*!
 * \file FunctionExecTime.h
 *
 * \author Masf
 * \date 四月 2019
 *
 * 统计函数执行时间
 */

#include "FunctionExecTime.h"

#include <iostream>
#include <QtWidgets/QtWidgets>
using namespace std;

CFunctionExecTime::CFunctionExecTime(const string& funName, bool show)
	: m_funName(funName), m_show(show)
{
	m_start = clock();
	m_end = clock();
}

CFunctionExecTime::~CFunctionExecTime()
{
	m_end = clock();
	long ellapsed = m_end - m_start;
	if (m_show || ellapsed >= 1000)
	{
		std::cout << QStringLiteral("查询数据耗时: %1ms").arg(ellapsed).toStdString() << endl;
	}
}

void CFunctionExecTime::outputExecTime(const string& funName, const string& section, bool show /*= false*/)
{
	clock_t end = clock();
	long ellapsed = end - m_end;
	m_end = end;
	if (show || ellapsed >= 1000)
	{
		std::cout << QStringLiteral("查询数据耗时: %1ms").arg(ellapsed).toStdString() << endl;
	}
}
