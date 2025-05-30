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
// Generated from file `translogger.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <translogger.h>
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

const ::std::string __TransferRdbLogger__TransferRdbLoggerInfo__transferRdbLoggerBinary_name = "transferRdbLoggerBinary";

const ::std::string __TransferRdbLogger__TransferRdbLoggerInfo__transferFinished_name = "transferFinished";

const ::std::string __TransferRdbLogger__TransferRdbLoggerInfo__syncRdbFinished_name = "syncRdbFinished";

}
::IceProxy::Ice::Object* ::IceProxy::TransferRdbLogger::upCast(::IceProxy::TransferRdbLogger::TransferRdbLoggerInfo* p) { return p; }

void
::IceProxy::TransferRdbLogger::__read(::IceInternal::BasicStream* __is, ::IceInternal::ProxyHandle< ::IceProxy::TransferRdbLogger::TransferRdbLoggerInfo>& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::TransferRdbLogger::TransferRdbLoggerInfo;
        v->__copyFrom(proxy);
    }
}

void
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::transferRdbLoggerBinary(const ::std::string& __p_title, const ::TransferRdbLogger::Bytes& __p_datas, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __TransferRdbLogger__TransferRdbLoggerInfo__transferRdbLoggerBinary_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_title);
        __os->write(__p_datas);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::begin_transferRdbLoggerBinary(const ::std::string& __p_title, const ::TransferRdbLogger::Bytes& __p_datas, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __TransferRdbLogger__TransferRdbLoggerInfo__transferRdbLoggerBinary_name, __del, __cookie);
    try
    {
        __result->prepare(__TransferRdbLogger__TransferRdbLoggerInfo__transferRdbLoggerBinary_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_title);
        __os->write(__p_datas);
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
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::end_transferRdbLoggerBinary(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __TransferRdbLogger__TransferRdbLoggerInfo__transferRdbLoggerBinary_name);
}

void
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::transferFinished(const ::std::string& __p_title, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __TransferRdbLogger__TransferRdbLoggerInfo__transferFinished_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_title);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::begin_transferFinished(const ::std::string& __p_title, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __TransferRdbLogger__TransferRdbLoggerInfo__transferFinished_name, __del, __cookie);
    try
    {
        __result->prepare(__TransferRdbLogger__TransferRdbLoggerInfo__transferFinished_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_title);
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
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::end_transferFinished(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __TransferRdbLogger__TransferRdbLoggerInfo__transferFinished_name);
}

void
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::syncRdbFinished(const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __TransferRdbLogger__TransferRdbLoggerInfo__syncRdbFinished_name, ::Ice::Normal, __ctx);
    __og.writeEmptyParams();
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::begin_syncRdbFinished(const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __TransferRdbLogger__TransferRdbLoggerInfo__syncRdbFinished_name, __del, __cookie);
    try
    {
        __result->prepare(__TransferRdbLogger__TransferRdbLoggerInfo__syncRdbFinished_name, ::Ice::Normal, __ctx);
        __result->writeEmptyParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

void
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::end_syncRdbFinished(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __TransferRdbLogger__TransferRdbLoggerInfo__syncRdbFinished_name);
}

const ::std::string&
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::ice_staticId()
{
    return ::TransferRdbLogger::TransferRdbLoggerInfo::ice_staticId();
}

::IceProxy::Ice::Object*
IceProxy::TransferRdbLogger::TransferRdbLoggerInfo::__newInstance() const
{
    return new TransferRdbLoggerInfo;
}

::Ice::Object* TransferRdbLogger::upCast(::TransferRdbLogger::TransferRdbLoggerInfo* p) { return p; }

namespace
{
const ::std::string __TransferRdbLogger__TransferRdbLoggerInfo_ids[2] =
{
    "::Ice::Object",
    "::TransferRdbLogger::TransferRdbLoggerInfo"
};

}

bool
TransferRdbLogger::TransferRdbLoggerInfo::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__TransferRdbLogger__TransferRdbLoggerInfo_ids, __TransferRdbLogger__TransferRdbLoggerInfo_ids + 2, _s);
}

::std::vector< ::std::string>
TransferRdbLogger::TransferRdbLoggerInfo::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__TransferRdbLogger__TransferRdbLoggerInfo_ids[0], &__TransferRdbLogger__TransferRdbLoggerInfo_ids[2]);
}

const ::std::string&
TransferRdbLogger::TransferRdbLoggerInfo::ice_id(const ::Ice::Current&) const
{
    return __TransferRdbLogger__TransferRdbLoggerInfo_ids[1];
}

const ::std::string&
TransferRdbLogger::TransferRdbLoggerInfo::ice_staticId()
{
#ifdef ICE_HAS_THREAD_SAFE_LOCAL_STATIC
    static const ::std::string typeId = "::TransferRdbLogger::TransferRdbLoggerInfo";
    return typeId;
#else
    return __TransferRdbLogger__TransferRdbLoggerInfo_ids[1];
#endif
}

::Ice::DispatchStatus
TransferRdbLogger::TransferRdbLoggerInfo::___transferRdbLoggerBinary(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string __p_title;
    ::TransferRdbLogger::Bytes __p_datas;
    __is->read(__p_title);
    __is->read(__p_datas);
    __inS.endReadParams();
    transferRdbLoggerBinary(__p_title, __p_datas, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
TransferRdbLogger::TransferRdbLoggerInfo::___transferFinished(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string __p_title;
    __is->read(__p_title);
    __inS.endReadParams();
    transferFinished(__p_title, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
TransferRdbLogger::TransferRdbLoggerInfo::___syncRdbFinished(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.readEmptyParams();
    syncRdbFinished(__current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

namespace
{
const ::std::string __TransferRdbLogger__TransferRdbLoggerInfo_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "syncRdbFinished",
    "transferFinished",
    "transferRdbLoggerBinary"
};

}

::Ice::DispatchStatus
TransferRdbLogger::TransferRdbLoggerInfo::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< const ::std::string*, const ::std::string*> r = ::std::equal_range(__TransferRdbLogger__TransferRdbLoggerInfo_all, __TransferRdbLogger__TransferRdbLoggerInfo_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __TransferRdbLogger__TransferRdbLoggerInfo_all)
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
            return ___syncRdbFinished(in, current);
        }
        case 5:
        {
            return ___transferFinished(in, current);
        }
        case 6:
        {
            return ___transferRdbLoggerBinary(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
TransferRdbLogger::TransferRdbLoggerInfo::__writeImpl(::IceInternal::BasicStream* __os) const
{
    __os->startWriteSlice(ice_staticId(), -1, true);
    __os->endWriteSlice();
}

void
TransferRdbLogger::TransferRdbLoggerInfo::__readImpl(::IceInternal::BasicStream* __is)
{
    __is->startReadSlice();
    __is->endReadSlice();
}

void 
TransferRdbLogger::__patch(TransferRdbLoggerInfoPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::TransferRdbLogger::TransferRdbLoggerInfoPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::TransferRdbLogger::TransferRdbLoggerInfo::ice_staticId(), v);
    }
}
