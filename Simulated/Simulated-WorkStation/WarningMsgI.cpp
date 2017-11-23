
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

	QFile file(QString().fromStdString(title));
	if (!file.open(QIODevice::ReadOnly))
	{
		m_threadPtr->outputOperationData(QStringLiteral("读取告警文件错误"));
		return;
	}

	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_5_0);
	while(!in.atEnd())
	{
		QString str;
		int type;
		in >> type;
		switch(type)
		{
		case CWarningDataManager::Table_Alarm_Limit:
			{
				CAlarmLimitTableData data;
				in >> data.id >> data.datet;
				in >> data.station >> data.equipName >> data.line >> data.dataName >> data.state >> data.limitVal
					>> data.currVal >> data.sms_flag;
				in >> data.warnSource >> data.warnLevel;
				data.transformToQString(str);
				break;
			}
		default:
			{
				str = QStringLiteral("未知");
				break;
			}
		}
		m_threadPtr->outputOperationData(str);
	}

	m_threadPtr->outputOperationData(QStringLiteral("结束读取告警文件"));
}
