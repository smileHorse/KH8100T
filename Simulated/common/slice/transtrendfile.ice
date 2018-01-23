/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：transtrendfile.ice
* 
*****************************************************/ 
 
#ifndef _TRANSTRENDFILE_ICE
#define _TRANSTRENDFILE_ICE
 
[["java:package:com.gridnt.transtrendfile"]]
/**
* 
* 传输趋势数据文件
* 
*/
module TransferTrendFile
{ 
	// 趋势数据包(只有一个数据文件，一次传一个）
	sequence<byte> Bytes;	
	struct TrendPacket 
	{
        byte         id;       // 数据包ID
        string       fepNode;  // 所属前置机 
		string       filename;  // 趋势数据文件名
		int			 filelength;//趋势数据文件长度
		Bytes        datas;//趋势数据
    };
    
    
    /**
    * 命令上行（Unit->FEP->WKS），FEP为publisher
    * 需实现接口TrendAppManager。
    */
    const string TrendAppTopic = "trend_app_operation"; 
    interface TrendAppManager
    {
        void processTrendFile(TrendPacket aTrendPacket);  // 趋势文件数据 
    };
	
	/**
    * 命令下行（WKS->FEP），FEP为subscriber
    * TrendFepManager
    */
    const string TrendFepTopic = "trend_fep_operation";
    interface TrendFepManager
    {
        /**
        * @param: filename - 趋势数据文件名
        */
       void trendExecute( string filename );
    };
};
 
#endif //_TRANSTRENDFILE_ICE