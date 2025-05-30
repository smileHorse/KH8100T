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
// Generated from file `manualdata.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <manualdata.h>
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

const ::std::string __ManualData__ManualDataManagerRequest__send_name = "send";

const ::std::string __ManualData__ManualDataManagerRequest__cancalManual_name = "cancalManual";

const ::std::string __ManualData__ManualDataManagerRequest__sendFileList_name = "sendFileList";

const ::std::string __ManualData__ManualDataManagerResponse__receive_name = "receive";

const ::std::string __ManualData__ManualDataManagerResponse__receiveFileList_name = "receiveFileList";

const ::std::string __ManualData__ManualDataManagerResponse__receiveSegment_name = "receiveSegment";

}

namespace Ice
{
}
::IceProxy::Ice::Object* ::IceProxy::ManualData::upCast(::IceProxy::ManualData::ManualDataManagerRequest* p) { return p; }

void
::IceProxy::ManualData::__read(::IceInternal::BasicStream* __is, ::IceInternal::ProxyHandle< ::IceProxy::ManualData::ManualDataManagerRequest>& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::ManualData::ManualDataManagerRequest;
        v->__copyFrom(proxy);
    }
}

void
IceProxy::ManualData::ManualDataManagerRequest::send(const ::std::string& __p_requestId, const ::ManualData::manualMsgSeq& __p_dataSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __ManualData__ManualDataManagerRequest__send_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_dataSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::ManualData::ManualDataManagerRequest::begin_send(const ::std::string& __p_requestId, const ::ManualData::manualMsgSeq& __p_dataSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __ManualData__ManualDataManagerRequest__send_name, __del, __cookie);
    try
    {
        __result->prepare(__ManualData__ManualDataManagerRequest__send_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_dataSeq);
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
IceProxy::ManualData::ManualDataManagerRequest::end_send(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __ManualData__ManualDataManagerRequest__send_name);
}

void
IceProxy::ManualData::ManualDataManagerRequest::cancalManual(const ::std::string& __p_requestId, const ::ManualData::stringSeq& __p_deviceSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __ManualData__ManualDataManagerRequest__cancalManual_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_deviceSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::ManualData::ManualDataManagerRequest::begin_cancalManual(const ::std::string& __p_requestId, const ::ManualData::stringSeq& __p_deviceSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __ManualData__ManualDataManagerRequest__cancalManual_name, __del, __cookie);
    try
    {
        __result->prepare(__ManualData__ManualDataManagerRequest__cancalManual_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_deviceSeq);
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
IceProxy::ManualData::ManualDataManagerRequest::end_cancalManual(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __ManualData__ManualDataManagerRequest__cancalManual_name);
}

void
IceProxy::ManualData::ManualDataManagerRequest::sendFileList(const ::std::string& __p_requestId, const ::std::string& __p_deviceId, const ::ManualData::stringSeq& __p_fileSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __ManualData__ManualDataManagerRequest__sendFileList_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_deviceId);
        __os->write(__p_fileSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::ManualData::ManualDataManagerRequest::begin_sendFileList(const ::std::string& __p_requestId, const ::std::string& __p_deviceId, const ::ManualData::stringSeq& __p_fileSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __ManualData__ManualDataManagerRequest__sendFileList_name, __del, __cookie);
    try
    {
        __result->prepare(__ManualData__ManualDataManagerRequest__sendFileList_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_deviceId);
        __os->write(__p_fileSeq);
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
IceProxy::ManualData::ManualDataManagerRequest::end_sendFileList(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __ManualData__ManualDataManagerRequest__sendFileList_name);
}

const ::std::string&
IceProxy::ManualData::ManualDataManagerRequest::ice_staticId()
{
    return ::ManualData::ManualDataManagerRequest::ice_staticId();
}

::IceProxy::Ice::Object*
IceProxy::ManualData::ManualDataManagerRequest::__newInstance() const
{
    return new ManualDataManagerRequest;
}
::IceProxy::Ice::Object* ::IceProxy::ManualData::upCast(::IceProxy::ManualData::ManualDataManagerResponse* p) { return p; }

void
::IceProxy::ManualData::__read(::IceInternal::BasicStream* __is, ::IceInternal::ProxyHandle< ::IceProxy::ManualData::ManualDataManagerResponse>& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::ManualData::ManualDataManagerResponse;
        v->__copyFrom(proxy);
    }
}

void
IceProxy::ManualData::ManualDataManagerResponse::receive(const ::ManualData::receiveMsgSeq& __p_recSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __ManualData__ManualDataManagerResponse__receive_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_recSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::ManualData::ManualDataManagerResponse::begin_receive(const ::ManualData::receiveMsgSeq& __p_recSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __ManualData__ManualDataManagerResponse__receive_name, __del, __cookie);
    try
    {
        __result->prepare(__ManualData__ManualDataManagerResponse__receive_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_recSeq);
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
IceProxy::ManualData::ManualDataManagerResponse::end_receive(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __ManualData__ManualDataManagerResponse__receive_name);
}

void
IceProxy::ManualData::ManualDataManagerResponse::receiveFileList(const ::std::string& __p_requestId, const ::std::string& __p_deviceId, const ::ManualData::ManualFile& __p_fileSeq, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __ManualData__ManualDataManagerResponse__receiveFileList_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_deviceId);
        __os->write(__p_fileSeq);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::ManualData::ManualDataManagerResponse::begin_receiveFileList(const ::std::string& __p_requestId, const ::std::string& __p_deviceId, const ::ManualData::ManualFile& __p_fileSeq, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __ManualData__ManualDataManagerResponse__receiveFileList_name, __del, __cookie);
    try
    {
        __result->prepare(__ManualData__ManualDataManagerResponse__receiveFileList_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_deviceId);
        __os->write(__p_fileSeq);
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
IceProxy::ManualData::ManualDataManagerResponse::end_receiveFileList(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __ManualData__ManualDataManagerResponse__receiveFileList_name);
}

void
IceProxy::ManualData::ManualDataManagerResponse::receiveSegment(const ::std::string& __p_requestId, const ::std::string& __p_deviceId, const ::ManualData::ManualSegment& __p_fileSegment, const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __ManualData__ManualDataManagerResponse__receiveSegment_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_deviceId);
        __os->write(__p_fileSegment);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::ManualData::ManualDataManagerResponse::begin_receiveSegment(const ::std::string& __p_requestId, const ::std::string& __p_deviceId, const ::ManualData::ManualSegment& __p_fileSegment, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __ManualData__ManualDataManagerResponse__receiveSegment_name, __del, __cookie);
    try
    {
        __result->prepare(__ManualData__ManualDataManagerResponse__receiveSegment_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_requestId);
        __os->write(__p_deviceId);
        __os->write(__p_fileSegment);
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
IceProxy::ManualData::ManualDataManagerResponse::end_receiveSegment(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __ManualData__ManualDataManagerResponse__receiveSegment_name);
}

const ::std::string&
IceProxy::ManualData::ManualDataManagerResponse::ice_staticId()
{
    return ::ManualData::ManualDataManagerResponse::ice_staticId();
}

::IceProxy::Ice::Object*
IceProxy::ManualData::ManualDataManagerResponse::__newInstance() const
{
    return new ManualDataManagerResponse;
}

::Ice::Object* ManualData::upCast(::ManualData::ManualDataManagerRequest* p) { return p; }

namespace
{
const ::std::string __ManualData__ManualDataManagerRequest_ids[2] =
{
    "::Ice::Object",
    "::ManualData::ManualDataManagerRequest"
};

}

bool
ManualData::ManualDataManagerRequest::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__ManualData__ManualDataManagerRequest_ids, __ManualData__ManualDataManagerRequest_ids + 2, _s);
}

::std::vector< ::std::string>
ManualData::ManualDataManagerRequest::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__ManualData__ManualDataManagerRequest_ids[0], &__ManualData__ManualDataManagerRequest_ids[2]);
}

const ::std::string&
ManualData::ManualDataManagerRequest::ice_id(const ::Ice::Current&) const
{
    return __ManualData__ManualDataManagerRequest_ids[1];
}

const ::std::string&
ManualData::ManualDataManagerRequest::ice_staticId()
{
#ifdef ICE_HAS_THREAD_SAFE_LOCAL_STATIC
    static const ::std::string typeId = "::ManualData::ManualDataManagerRequest";
    return typeId;
#else
    return __ManualData__ManualDataManagerRequest_ids[1];
#endif
}

::Ice::DispatchStatus
ManualData::ManualDataManagerRequest::___send(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string __p_requestId;
    ::ManualData::manualMsgSeq __p_dataSeq;
    __is->read(__p_requestId);
    __is->read(__p_dataSeq);
    __inS.endReadParams();
    send(__p_requestId, __p_dataSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
ManualData::ManualDataManagerRequest::___cancalManual(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string __p_requestId;
    ::ManualData::stringSeq __p_deviceSeq;
    __is->read(__p_requestId);
    __is->read(__p_deviceSeq);
    __inS.endReadParams();
    cancalManual(__p_requestId, __p_deviceSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
ManualData::ManualDataManagerRequest::___sendFileList(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string __p_requestId;
    ::std::string __p_deviceId;
    ::ManualData::stringSeq __p_fileSeq;
    __is->read(__p_requestId);
    __is->read(__p_deviceId);
    __is->read(__p_fileSeq);
    __inS.endReadParams();
    sendFileList(__p_requestId, __p_deviceId, __p_fileSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

namespace
{
const ::std::string __ManualData__ManualDataManagerRequest_all[] =
{
    "cancalManual",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "send",
    "sendFileList"
};

}

::Ice::DispatchStatus
ManualData::ManualDataManagerRequest::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< const ::std::string*, const ::std::string*> r = ::std::equal_range(__ManualData__ManualDataManagerRequest_all, __ManualData__ManualDataManagerRequest_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __ManualData__ManualDataManagerRequest_all)
    {
        case 0:
        {
            return ___cancalManual(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
        case 5:
        {
            return ___send(in, current);
        }
        case 6:
        {
            return ___sendFileList(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
ManualData::ManualDataManagerRequest::__writeImpl(::IceInternal::BasicStream* __os) const
{
    __os->startWriteSlice(ice_staticId(), -1, true);
    __os->endWriteSlice();
}

void
ManualData::ManualDataManagerRequest::__readImpl(::IceInternal::BasicStream* __is)
{
    __is->startReadSlice();
    __is->endReadSlice();
}

void 
ManualData::__patch(ManualDataManagerRequestPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::ManualData::ManualDataManagerRequestPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::ManualData::ManualDataManagerRequest::ice_staticId(), v);
    }
}

::Ice::Object* ManualData::upCast(::ManualData::ManualDataManagerResponse* p) { return p; }

namespace
{
const ::std::string __ManualData__ManualDataManagerResponse_ids[2] =
{
    "::Ice::Object",
    "::ManualData::ManualDataManagerResponse"
};

}

bool
ManualData::ManualDataManagerResponse::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__ManualData__ManualDataManagerResponse_ids, __ManualData__ManualDataManagerResponse_ids + 2, _s);
}

::std::vector< ::std::string>
ManualData::ManualDataManagerResponse::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__ManualData__ManualDataManagerResponse_ids[0], &__ManualData__ManualDataManagerResponse_ids[2]);
}

const ::std::string&
ManualData::ManualDataManagerResponse::ice_id(const ::Ice::Current&) const
{
    return __ManualData__ManualDataManagerResponse_ids[1];
}

const ::std::string&
ManualData::ManualDataManagerResponse::ice_staticId()
{
#ifdef ICE_HAS_THREAD_SAFE_LOCAL_STATIC
    static const ::std::string typeId = "::ManualData::ManualDataManagerResponse";
    return typeId;
#else
    return __ManualData__ManualDataManagerResponse_ids[1];
#endif
}

::Ice::DispatchStatus
ManualData::ManualDataManagerResponse::___receive(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::ManualData::receiveMsgSeq __p_recSeq;
    __is->read(__p_recSeq);
    __inS.endReadParams();
    receive(__p_recSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
ManualData::ManualDataManagerResponse::___receiveFileList(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string __p_requestId;
    ::std::string __p_deviceId;
    ::ManualData::ManualFile __p_fileSeq;
    __is->read(__p_requestId);
    __is->read(__p_deviceId);
    __is->read(__p_fileSeq);
    __inS.endReadParams();
    receiveFileList(__p_requestId, __p_deviceId, __p_fileSeq, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
ManualData::ManualDataManagerResponse::___receiveSegment(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string __p_requestId;
    ::std::string __p_deviceId;
    ::ManualData::ManualSegment __p_fileSegment;
    __is->read(__p_requestId);
    __is->read(__p_deviceId);
    __is->read(__p_fileSegment);
    __inS.endReadParams();
    receiveSegment(__p_requestId, __p_deviceId, __p_fileSegment, __current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

namespace
{
const ::std::string __ManualData__ManualDataManagerResponse_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "receive",
    "receiveFileList",
    "receiveSegment"
};

}

::Ice::DispatchStatus
ManualData::ManualDataManagerResponse::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< const ::std::string*, const ::std::string*> r = ::std::equal_range(__ManualData__ManualDataManagerResponse_all, __ManualData__ManualDataManagerResponse_all + 7, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __ManualData__ManualDataManagerResponse_all)
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
            return ___receive(in, current);
        }
        case 5:
        {
            return ___receiveFileList(in, current);
        }
        case 6:
        {
            return ___receiveSegment(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
ManualData::ManualDataManagerResponse::__writeImpl(::IceInternal::BasicStream* __os) const
{
    __os->startWriteSlice(ice_staticId(), -1, true);
    __os->endWriteSlice();
}

void
ManualData::ManualDataManagerResponse::__readImpl(::IceInternal::BasicStream* __is)
{
    __is->startReadSlice();
    __is->endReadSlice();
}

void 
ManualData::__patch(ManualDataManagerResponsePtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::ManualData::ManualDataManagerResponsePtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::ManualData::ManualDataManagerResponse::ice_staticId(), v);
    }
}
