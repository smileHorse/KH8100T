#include "AmsSrvI.h"
#include "AmlConst.h"
#include "AmsServerThread.h"
#include "OperationInfo.h"

using namespace Amssrv;
using namespace std;

CAmsAppI::CAmsAppI( AmsServerThread* threadPtr )
	: m_threadPtr(threadPtr)
{
}

std::string CAmsAppI::Register( const string& strType, const Strings& ipVect, ::Ice::Int num, 
	ServerNode& serverInfo, const ::Ice::Current& /* = ::Ice::Current */ )
{
	serverInfo.name = "主数据服务";
	serverInfo.type = TYPE_DATASRV;
	serverInfo.ip = "127.0.0.1";
	serverInfo.port = DATASRV_ADAPTER_PORT;

	OperationInfo info(QString().fromStdString(strType).toInt(), "请求注册", true);
	m_threadPtr->putMessage(info);

	return ROLE_MASTER;
}

void CAmsAppI::FinishRegister( const string& strServerName, const string& strRole, string& district, 
	string& section, string& districtName, string& sectionName, const ::Ice::Current& /* = ::Ice::Current */ )
{

}

std::string CAmsAppI::GetRole( const string& strServerName, const ::Ice::Current& /* = ::Ice::Current */ ) const
{
	return ROLE_MASTER;
}
