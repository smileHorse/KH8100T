/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：realdbsync.ice
* 
*****************************************************/ 
 
#ifndef _REALDBSYNC_ICE
#define _REALDBSYNC_ICE
 
[["java:package:com.gridnt.realdbsync"]]
/**
* 
* 实时库同步
* 
*/
module RealdbSyncSpace
{ 
    ["java:type:java.util.ArrayList<byte>"]   sequence<byte> Bytes;
    /**
    *
    *文件内容
    *
    **/
    struct FileContent
    {
        //读取文件位置
        long pos;
        
        //内容
        Bytes content;

        //文件状态
        bool status;
    };


    interface RealdbSync
    {
       // 请求实时库
       bool requestRealdb();

       // 请求实时库内容
       FileContent requestRealdbContent(string fileName, long pos);
    };
};
 
#endif 