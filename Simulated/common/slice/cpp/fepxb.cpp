// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.6.4
//
// <auto-generated>
//
// Generated from file `fepxb.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <fepxb.h>
#include <IceUtil/PushDisableWarnings.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/Outgoing.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/PopDisableWarnings.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 306
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 4
#       error Ice patch level mismatch!
#   endif
#endif

namespace
{

const ::std::string __FepXbData__CFepXb__sendSettingValue_name = "sendSettingValue";

const ::std::string __FepXbData__CFepXb__sendBrief_name = "sendBrief";

const ::std::string __FepXbData__CFepXb__sendSoe_name = "sendSoe";

const ::std::string __FepXbData__CFepXb__sendDiscrete_name = "sendDiscrete";

const ::std::string __FepXbData__CFepXb__sendUnitStatus_name = "sendUnitStatus";

const ::std::string __FepXbData__CFepXb__sendWave_name = "sendWave";

}

namespace Ice
{
}
::IceProxy::Ice::Object* ::IceProxy::FepXbData::upCast(::IceProxy::FepXbData::CFepXb* p) { return p; }

void
::IceProxy::FepXbData::__read(::IceInternal::BasicStream* __is, ::IceInternal::ProxyHandle< ::IceProxy::FepXbData::CFepXb>& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::FepXbData::CFepXb;
        v->__copyFrom(proxy);
    }
}

void
IceProxy::FepXbData::CFepXb::sendSettingValue(const ::FepXbData::SettingValueInfo& __p_settingVal, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __FepXbData__CFepXb__sendSettingValue_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_settingVal);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::FepXbData::CFepXb::begin_sendSettingValue(const ::FepXbData::SettingValueInfo& __p_settingVal, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __FepXbData__CFepXb__sendSettingValue_name, __del, __cookie);
    try
    {
        __result->prepare(__FepXbData__CFepXb__sendSettingValue_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_settingVal);
        __result->endWriteParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

void
IceProxy::FepXbData::CFepXb::end_sendSettingValue(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __FepXbData__CFepXb__sendSettingValue_name);
}

void
IceProxy::FepXbData::CFepXb::sendBrief(const ::FepXbData::TduBriefSeq& __p_briefSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __FepXbData__CFepXb__sendBrief_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_briefSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::FepXbData::CFepXb::begin_sendBrief(const ::FepXbData::TduBriefSeq& __p_briefSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __FepXbData__CFepXb__sendBrief_name, __del, __cookie);
    try
    {
        __result->prepare(__FepXbData__CFepXb__sendBrief_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_briefSeq);
        __result->endWriteParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

void
IceProxy::FepXbData::CFepXb::end_sendBrief(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __FepXbData__CFepXb__sendBrief_name);
}

void
IceProxy::FepXbData::CFepXb::sendSoe(const ::FepXbData::XbSOESeq& __p_soeSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __FepXbData__CFepXb__sendSoe_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_soeSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::FepXbData::CFepXb::begin_sendSoe(const ::FepXbData::XbSOESeq& __p_soeSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __FepXbData__CFepXb__sendSoe_name, __del, __cookie);
    try
    {
        __result->prepare(__FepXbData__CFepXb__sendSoe_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_soeSeq);
        __result->endWriteParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

void
IceProxy::FepXbData::CFepXb::end_sendSoe(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __FepXbData__CFepXb__sendSoe_name);
}

void
IceProxy::FepXbData::CFepXb::sendDiscrete(const ::FepXbData::XbDiscreteSeq& __p_discreteSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __FepXbData__CFepXb__sendDiscrete_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_discreteSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::FepXbData::CFepXb::begin_sendDiscrete(const ::FepXbData::XbDiscreteSeq& __p_discreteSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __FepXbData__CFepXb__sendDiscrete_name, __del, __cookie);
    try
    {
        __result->prepare(__FepXbData__CFepXb__sendDiscrete_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_discreteSeq);
        __result->endWriteParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

void
IceProxy::FepXbData::CFepXb::end_sendDiscrete(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __FepXbData__CFepXb__sendDiscrete_name);
}

void
IceProxy::FepXbData::CFepXb::sendUnitStatus(const ::FepXbData::XbUnitStatusSeq& __p_usSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __FepXbData__CFepXb__sendUnitStatus_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_usSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::FepXbData::CFepXb::begin_sendUnitStatus(const ::FepXbData::XbUnitStatusSeq& __p_usSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __FepXbData__CFepXb__sendUnitStatus_name, __del, __cookie);
    try
    {
        __result->prepare(__FepXbData__CFepXb__sendUnitStatus_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_usSeq);
        __result->endWriteParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

void
IceProxy::FepXbData::CFepXb::end_sendUnitStatus(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __FepXbData__CFepXb__sendUnitStatus_name);
}

void
IceProxy::FepXbData::CFepXb::sendWave(const ::FepXbData::XbWave& __p_wave, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __FepXbData__CFepXb__sendWave_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_wave);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::FepXbData::CFepXb::begin_sendWave(const ::FepXbData::XbWave& __p_wave, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __FepXbData__CFepXb__sendWave_name, __del, __cookie);
    try
    {
        __result->prepare(__FepXbData__CFepXb__sendWave_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_wave);
        __result->endWriteParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

void
IceProxy::FepXbData::CFepXb::end_sendWave(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __FepXbData__CFepXb__sendWave_name);
}

const ::std::string&
IceProxy::FepXbData::CFepXb::ice_staticId()
{
    return ::FepXbData::CFepXb::ice_staticId();
}

::IceProxy::Ice::Object*
IceProxy::FepXbData::CFepXb::__newInstance() const
{
    return new CFepXb;
}

::Ice::Object* FepXbData::upCast(::FepXbData::CFepXb* p) { return p; }

namespace
{
const ::std::string __FepXbData__CFepXb_ids[2] =
{
    "::FepXbData::CFepXb",
    "::Ice::Object"
};

}

bool
FepXbData::CFepXb::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__FepXbData__CFepXb_ids, __FepXbData__CFepXb_ids + 2, _s);
}

::std::vector< ::std::string>
FepXbData::CFepXb::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__FepXbData__CFepXb_ids[0], &__FepXbData__CFepXb_ids[2]);
}

const ::std::string&
FepXbData::CFepXb::ice_id(const ::Ice::Current&) const
{
    return __FepXbData__CFepXb_ids[0];
}

const ::std::string&
FepXbData::CFepXb::ice_staticId()
{
#ifdef ICE_HAS_THREAD_SAFE_LOCAL_STATIC
    static const ::std::string typeId = "::FepXbData::CFepXb";
    return typeId;
#else
    return __FepXbData__CFepXb_ids[0];
#endif
}

::Ice::DispatchStatus
FepXbData::CFepXb::___sendSettingValue(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::FepXbData::SettingValueInfo __p_settingVal;
    __is->read(__p_settingVal);
    __inS.endReadParams();
    sendSettingValue(__p_settingVal, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
FepXbData::CFepXb::___sendBrief(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::FepXbData::TduBriefSeq __p_briefSeq;
    __is->read(__p_briefSeq);
    __inS.endReadParams();
    sendBrief(__p_briefSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
FepXbData::CFepXb::___sendSoe(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::FepXbData::XbSOESeq __p_soeSeq;
    __is->read(__p_soeSeq);
    __inS.endReadParams();
    sendSoe(__p_soeSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
FepXbData::CFepXb::___sendDiscrete(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::FepXbData::XbDiscreteSeq __p_discreteSeq;
    __is->read(__p_discreteSeq);
    __inS.endReadParams();
    sendDiscrete(__p_discreteSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
FepXbData::CFepXb::___sendUnitStatus(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::FepXbData::XbUnitStatusSeq __p_usSeq;
    __is->read(__p_usSeq);
    __inS.endReadParams();
    sendUnitStatus(__p_usSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
FepXbData::CFepXb::___sendWave(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::FepXbData::XbWave __p_wave;
    __is->read(__p_wave);
    __inS.endReadParams();
    sendWave(__p_wave, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

namespace
{
const ::std::string __FepXbData__CFepXb_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "sendBrief",
    "sendDiscrete",
    "sendSettingValue",
    "sendSoe",
    "sendUnitStatus",
    "sendWave"
};

}

::Ice::DispatchStatus
FepXbData::CFepXb::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< const ::std::string*, const ::std::string*> r = ::std::equal_range(__FepXbData__CFepXb_all, __FepXbData__CFepXb_all + 10, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __FepXbData__CFepXb_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___sendBrief(in, current);
        }
        case 5:
        {
            return ___sendDiscrete(in, current);
        }
        case 6:
        {
            return ___sendSettingValue(in, current);
        }
        case 7:
        {
            return ___sendSoe(in, current);
        }
        case 8:
        {
            return ___sendUnitStatus(in, current);
        }
        case 9:
        {
            return ___sendWave(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
FepXbData::CFepXb::__writeImpl(::IceInternal::BasicStream* __os) const
{
    __os->startWriteSlice(ice_staticId(), -1, true);
    __os->endWriteSlice();
}

void
FepXbData::CFepXb::__readImpl(::IceInternal::BasicStream* __is)
{
    __is->startReadSlice();
    __is->endReadSlice();
}

void 
FepXbData::__patch(CFepXbPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::FepXbData::CFepXbPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::FepXbData::CFepXb::ice_staticId(), v);
    }
}
