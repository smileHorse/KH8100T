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
// Generated from file `rdbdata.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

package com.gridnt.icestorm.RdbWarningData;

public final class RdbAlarmDataHolder extends Ice.ObjectHolderBase<RdbAlarmData>
{
    public
    RdbAlarmDataHolder()
    {
    }

    public
    RdbAlarmDataHolder(RdbAlarmData value)
    {
        this.value = value;
    }

    public void
    patch(Ice.Object v)
    {
        if(v == null || v instanceof RdbAlarmData)
        {
            value = (RdbAlarmData)v;
        }
        else
        {
            IceInternal.Ex.throwUOE(type(), v);
        }
    }

    public String
    type()
    {
        return _RdbAlarmDataDisp.ice_staticId();
    }
}