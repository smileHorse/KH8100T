
#include "WarningMsgI.h"
#include "WarningTableData.h"
#include "workstationserverthread.h"

#include <fstream>
#include <QtCore/QtCore>

using namespace std;

#define BUFFER_SIZE 2048 * 2

SendWarningMsgI::SendWarningMsgI( WorkStationServerThread* threadPtr )
	: m_threadPtr(threadPtr)
{

}

void SendWarningMsgI::sendWarningMsgBinary( const ::std::string& title, const ::WarningMsg::Bytes& bytes, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	ofstream fout(title, std::ios::binary | std::ios::app);

	if (!bytes.empty())
	{
		char szBuf[BUFFER_SIZE] = {0};  
		unsigned size = bytes.size();
		for (int i = 0; i < size; ++i)
		{
			szBuf[i] = bytes[i];
		}
		fout.write(szBuf, sizeof(char) * size);
	}
}

void SendWarningMsgI::sendFinished( const ::std::string& title, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	m_threadPtr->outputOperationData(QStringLiteral("接收告警文件 %1 完成").arg(QString().fromStdString(title)));
	m_threadPtr->outputOperationData(QStringLiteral("开始读取告警文件"));

	/*
	ifstream inf(title, std::ios::binary);

	while(!inf.eof())
	{
		int type;
		inf.read((char*)&type, sizeof(int));

		QString str;
		switch(type)
		{
		case CWarningDataManager::Table_Alarm_Limit:
			{
				CAlarmLimitTableData data;
				inf.read((char*)&data, sizeof(CAlarmLimitTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Soe:
			{
				CAlarmSoeTableData data;
				inf.read((char*)&data, sizeof(CAlarmSoeTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Yx:
			{
				CAlarmYxTableData data;
				inf.read((char*)&data, sizeof(CAlarmYxTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_SysEvent:
			{
				CAlarmSysEventTableData data;
				inf.read((char*)&data, sizeof(CAlarmSysEventTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Oper:
			{
				CAlarmOperTableData data;
				inf.read((char*)&data, sizeof(CAlarmOperTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Relay:
			{
				CAlarmRelayTableData data;
				inf.read((char*)&data, sizeof(CAlarmRelayTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Rtu:
			{
				CAlarmRtuTableData data;
				inf.read((char*)&data, sizeof(CAlarmRtuTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Graph:
			{
				CAlarmGraphTableData data;
				inf.read((char*)&data, sizeof(CAlarmGraphTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Fa:
			{
				CAlarmFaTableData data;
				inf.read((char*)&data, sizeof(CAlarmFaTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Fault:
			{
				CAlarmFaultTableData data;
				inf.read((char*)&data, sizeof(CAlarmFaultTableData));
				data.transformToQString(str);
				break;
			}
		case CWarningDataManager::Table_Alarm_Rfw:
			{
				CAlarmRfwTableData data;
				inf.read((char*)&data, sizeof(CAlarmRfwTableData));
				data.transformToQString(str);
				break;
			}
		default:
			{
				str = QStringLiteral("未知");
				break;
			}
		}
		m_threadPtr->outputOperationData("");
	}
	*/

	FILE* inf = NULL;
	if ((inf = fopen(title.c_str(), "r+b")) == NULL)
	{
		m_threadPtr->outputOperationData(QStringLiteral("打开告警文件出错..."));
		return;
	}

	while(!feof(inf))
	{
		int type;
		int nItems = fread(&type, sizeof(int), 1, inf);
		if (nItems <= 0)
		{
			break;
		}

		switch(type)
		{
		case CWarningDataManager::Table_Alarm_Limit:
			{
				QSharedPointer<CAlarmLimitTableData> data = QSharedPointer<CAlarmLimitTableData>::create();
				data->readFromBinaryFile(inf);

				QString str;
				data->transformToQString(str);
				m_threadPtr->outputOperationData(str);
				break;
			}
		default:
			break;
		}
	}

	m_threadPtr->outputOperationData(QStringLiteral("结束读取告警文件"));
}
