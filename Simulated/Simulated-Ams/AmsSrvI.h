#ifndef AMSSRV_I_H
#define AMSSRV_I_H

#include <Ice/ice.h>
#include <IceUtil/IceUtil.h>
#include <string>

#include <amssrv.h>

using namespace std;

class AmsServerThread;

class CAmsAppI : public Amssrv::CAmsApp
{
public:
	CAmsAppI(AmsServerThread* threadPtr);

	virtual string Register(const ::std::string& strType, const ::Amssrv::Strings& ipVect, ::Ice::Int num, 
			::Amssrv::ServerNode& serverInfo, const ::Ice::Current& /* = ::Ice::Current */);

	virtual void FinishRegister(const ::std::string& strServerName, const ::std::string& strRole, const ::std::string& district, 
		const ::std::string& section,	const ::Ice::Current& /* = ::Ice::Current */);

	virtual void Quit(const ::Amssrv::ServerNode&, const ::Ice::Current& /* = ::Ice::Current() */);
	
	virtual void shiftRole(const ::std::string&, const ::std::string&, const ::std::string&, ::Ice::Int, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current());

	virtual string GetRole(const ::std::string& strServerName, const ::Ice::Current& /* = ::Ice::Current */) const;

	virtual void HeartBeat(const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual ::Amssrv::ServerNodeSeq getServerNodeList(const ::Ice::Current& = ::Ice::Current());

	virtual void setServerNodeList(const ::Amssrv::ServerNodeSeq&, const ::Ice::Current& = ::Ice::Current());

public:
	void setDataSrvInfo(const string& ip, int port);

	void setRole(const string& role);

private:
	AmsServerThread*	m_threadPtr;

	::Amssrv::ServerNode	dataSrvInfo;	// 数据服务器节点信息

	string	m_role;		// 当前角色

	::Amssrv::ServerNodeSeq	serverNodes;	// 服务节点
};
#endif