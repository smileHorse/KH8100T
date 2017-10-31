/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：translogger.ice
* 
*****************************************************/ 
 
#ifndef _TRANSLOGGER_ICE
#define _TRANSLOGGER_ICE
 
[["java:package:com.gridnt.translogger"]]
/**
* 
* 传输实时库日志文件
* 
*/
module TransferRdbLogger
{ 
	sequence<byte> Bytes;
    
    const string TransferRdbLoggerTopic = "transaction_logger";  // 主题
    interface TransferRdbLoggerInfo
    {
        void transferRdbLogger(string title, string data);

        void transferRdbLoggerBinary(string title, Bytes datas);
    };
};
 
#endif 