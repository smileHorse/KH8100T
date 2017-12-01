/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：translogger.ice
* 
*****************************************************/ 
 
#ifndef _PROCESSFILE_ICE
#define _PROCESSFILE_ICE
 
[["java:package:com.gridnt.processfile"]]
/**
* 
* 传输文件
* 
*/
module ProcessFile
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

    interface ProcessFileManager
    {
       // 请求文件
       FileContent requestFile(string title, long pos);

       // 发送文件
       bool sendFile(string title, Bytes datas);
    };
};
 
#endif 