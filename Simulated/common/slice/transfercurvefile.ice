/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：transfercurvefile.ice
* 
*****************************************************/ 
 
#ifndef _TRANSFERCURVEFILE_ICE
#define _TRANSFERCURVEFILE_ICE
 
[["java:package:com.gridnt.transfercurvefile"]]
/**
* 
* 传输文件
* 
*/
module TransferCurveFile
{     
    sequence<byte> Bytes;    
    const string TransferCurveFileTopic = "TransferCurveFileTopic";  // 主题
    interface CTransferCurveFile
    {
        // 发送文件
        void sendCurveFile(string title, string data);

        // 发送完成
        void sendFinished(string title);
    };
};
 
#endif 