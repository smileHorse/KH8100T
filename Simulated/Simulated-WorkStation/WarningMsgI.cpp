
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
		unsigned size = bytes.size();
		char* szBuf = new char[size];
		memset(szBuf, 0, size);
		for (int i = 0; i < size; ++i)
		{
			szBuf[i] = bytes[i];
		}
		fout.write(szBuf, sizeof(char) * size);
	}
}

void SendWarningMsgI::sendFaFileBinary(const ::std::string& folder, const ::std::string& title, 
	const ::WarningMsg::Bytes& bytes, const ::Ice::Current& /* = ::Ice::Current() */)
{
	string folderPath = "./data/" + folder;
	QString qFolderPath = QString::fromStdString(folderPath);
	QDir dir("./data/");
	if (dir.exists())
	{
		dir.mkdir(QString::fromStdString(folder));
	}
	string filePath = folderPath + "/" + title + ".xml";
	ofstream fout(title, std::ios::binary | std::ios::out);

	if (!bytes.empty())
	{
		unsigned size = bytes.size();
		char* szBuf = new char[size];
		memset(szBuf, 0, size);
		for (int i = 0; i < size; ++i)
		{
			szBuf[i] = bytes[i];
		}
		fout.write(szBuf, sizeof(char) * size);
	}
}

void SendWarningMsgI::sendRfwFileBinary(const ::std::string&, const ::std::string&, const ::WarningMsg::Bytes&, bool, const ::Ice::Current& /* = ::Ice::Current() */)
{

}

void SendWarningMsgI::sendCurveFileBinary(const ::std::string&, const ::std::string&, const ::WarningMsg::Bytes&, bool, const ::Ice::Current& /* = ::Ice::Current() */)
{

}

void SendWarningMsgI::sendReportFileBinary(const ::std::string&, const ::std::string&, const ::WarningMsg::Bytes&, bool, const ::Ice::Current& /*= ::Ice::Current()*/)
{

}
