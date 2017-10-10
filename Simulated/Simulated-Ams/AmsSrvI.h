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

	virtual void FinishRegister(const ::std::string& strServerName, const ::std::string& strRole, ::std::string& district, 
		::std::string& section, ::std::string& districtName, ::std::string& sectionName, 
		const ::Ice::Current& /* = ::Ice::Current */);

	virtual string GetRole(const ::std::string& strServerName, const ::Ice::Current& /* = ::Ice::Current */) const;

private:
	AmsServerThread*	m_threadPtr;
};
#endif