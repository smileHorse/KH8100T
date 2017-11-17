#ifndef YKDATAMANAGERI_H
#define YKDATAMANAGERI_H

#include "ykdata.h"

using namespace Yk;

class WorkStationServerThread;

class YkFepManagerI : public YkFepManager
{
public:
	YkFepManagerI( WorkStationServerThread* threadPtr );

	virtual void select(const ::Yk::YkCommand&, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual void execute(const ::Yk::YkCommand&, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual void cancel(const ::Yk::YkCommand&, const ::Ice::Current& /* = ::Ice::Current() */);

private:
	WorkStationServerThread*	m_threadPtr;
};

class YkAppManagerI : public YkAppManager
{
public:
	YkAppManagerI( WorkStationServerThread* threadPtr );

	virtual void selectEcho(const ::Yk::YkCommand&, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual void selectReturn(const ::Yk::YkCommand&, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual void executeEcho(const ::Yk::YkCommand&, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual void executeReturn(const ::Yk::YkCommand&, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual void cancelEcho(const ::Yk::YkCommand&, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */);

	virtual void cancelReturn(const ::Yk::YkCommand&, bool, const ::std::string&, const ::Ice::Current& /* = ::Ice::Current() */);

private:
	WorkStationServerThread*	m_threadPtr;
};
#endif