/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：transferfile.ice
* 
*****************************************************/ 
 
#ifndef _TRANSFERFILE_ICE
#define _TRANSFERFILE_ICE
 
[["java:package:com.gridnt.transferfile"]]
/**
* 
* 传输文件
* 
*/
module TransferFileModule
{ 
    ["java:type:java.util.ArrayList<byte>"]   sequence<byte> Bytes;

	struct FileContent
	{
		long 	pos;
		Bytes 	content;
	}; 

    interface TransferFileManager
    {
       // 上传文件
       void uploadFile(string title, Bytes content);

       // 下载文件
       FileContent downloadFile(string title, long pos);
    };
};
 
#endif 