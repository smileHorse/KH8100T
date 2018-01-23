#include "AmsSrvI.h"
#include "AmlConst.h"
#include "AmsServerThread.h"
#include "OperationInfo.h"

using namespace Amssrv;
using namespace std;

CAmsAppI::CAmsAppI( AmsServerThread* threadPtr )
	: m_threadPtr(threadPtr)
{
	// 初始化数据服务器节点信息
	dataSrvInfo.name = "主数据服务";
	dataSrvInfo.type = TYPE_DATASRV;
	dataSrvInfo.ip = "192.168.3.25";
	dataSrvInfo.port = DATASRV_ADAPTER_PORT;

	m_role = ROLE_MASTER;
}

std::string CAmsAppI::Register( const string& strType, const Strings& ipVect, ::Ice::Int num, 
	ServerNode& serverInfo, const ::Ice::Current& /* = ::Ice::Current */ )
{
	serverInfo = dataSrvInfo;

	OperationInfo info(QString().fromStdString(strType).toInt(), "请求注册", true);
	m_threadPtr->putMessage(info);

	return m_role;
}

void CAmsAppI::FinishRegister( const string& strServerName, const string& strRole, const string& district, 
	const string& section, const ::Ice::Current& /* = ::Ice::Current */ )
{

}

std::string CAmsAppI::GetRole( const string& strServerName, const ::Ice::Current& /* = ::Ice::Current */ ) const
{
	return ROLE_MASTER;
}

void CAmsAppI::Quit( const ::Amssrv::ServerNode&, const ::Ice::Current& /* = ::Ice::Current() */ )
{

}

void CAmsAppI::HeartBeat( const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	int i = 0;
}

void CAmsAppI::setDataSrvInfo( const string& ip, int port )
{
	dataSrvInfo.ip = ip;
	dataSrvInfo.port = port;
}

void CAmsAppI::setRole( const string& role )
{
	m_role = role;
}

ServerNodeSeq CAmsAppI::getServerNodeList( const ::Ice::Current& /*= ::Ice::Current()*/ )
{
	ServerNodeSeq seq;
	ServerNode node;
	
	node.type = TYPE_AMS;
	node.ip = "192.168.3.25";
	node.port = 10002;
	node.runRole = "master";
	seq.push_back(node);

	node.port = 10003;
	node.runRole = "slave";
	seq.push_back(node);

	node.type = TYPE_DATASRV;
	node.ip = "192.168.3.25";
	node.port = 10003;
	node.runRole = "master";
	seq.push_back(node);

	node.ip = "192.168.3.235";
	node.runRole = "slave";
	seq.push_back(node);

	node.type = TYPE_ICESTORM;
	node.ip = "192.168.3.25";
	node.port = 10000;
	node.runRole = "master";
	seq.push_back(node);

	return seq;
}

void CAmsAppI::setServerNodeList( const ::Amssrv::ServerNodeSeq&, const ::Ice::Current& /*= ::Ice::Current()*/ )
{

}

void CAmsAppI::shiftRole( const ::std::string&, const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& /*= ::Ice::Current()*/ )
{

}
