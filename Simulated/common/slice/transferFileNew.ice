/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：transferFileNew.ice
* 
*****************************************************/ 
 
#ifndef _TRANSFERFILENEW_ICE
#define _TRANSFERFILENEW_ICE
 
[["java:package:com.gridnt.transferfile"]]
/**
* 
* 传输文件
* 
*/
module TransferFileSpace
{ 
    ["java:type:java.util.ArrayList<byte>"]   sequence<byte> Bytes;

    const string TransferFileTopic = "transfer_file_topic";
    interface TransferFile
    {
       // 传输文件
       void sendFile(string filePath, Bytes fileDatas);
    };
};
 
#endif 