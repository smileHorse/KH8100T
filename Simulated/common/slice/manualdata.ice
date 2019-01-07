/* **************************************************
* Copyright(c) 2011-, KHDQ, inc. All rights reserved.
*
* File：manualdata.ice
* 手动调用行波数据slice接口定义文件。
*
*****************************************************/ 

#ifndef _FEP_MANUALDATA_ICESTORM
#define _FEP_MANUALDATA_ICESTORM

[["java:package:com.kh.icestorm"]]

module ManualData
{
    struct ManualTime
    {
        string year;
        string month;
        string day;
        string hour;
    };

    struct ManualMsg
    {
        string deviceId;
        string bz;//0:当天 1:此前几天数据 2：介于时间段 3:所有
        string day;
        ManualTime beginTime;
        ManualTime endTime;
    };
    sequence<ManualMsg> manualMsgSeq;

    struct ReceiveMsg
    {
        string requestId;
        string deviceId;
        string bz;//结束标志，0：否 1：是
        string message;
    };
    sequence<ReceiveMsg> receiveMsgSeq;

    //取消调用数据队列
    ["java:type:java.util.ArrayList<String>"] sequence<string> stringSeq;

    // 文件信息
    struct ManualFile
    {
        string fileName;     // 文件名称
        string fileSize;     // 文件大小
        string fileTime;     // 文件时间
        string fileResult;   // 传输标识 0=继续，1=结束，2=失败
    };

    // 文件传输信息
    struct ManualSegment
    {
        string fileName;     // 文件名称
        string fileSegment;  // 已传输大小
		string fileCount;    // 帧计数
		string fileData;     // 文件传输数据
        string fileResult;   // 传输标识 0=继续，1=结束，2=失败
    };
    
    /**
    * 手动调取数据
    * ManualDataManager
    */
    const string ManualDataRequestTopic = "manualdata_operation_Request";
    const string ManualDataResponseTopic = "manualdata_operation_Response";
    interface ManualDataManagerRequest
    {
        // 发送召唤文件列表命令
        void send(string requestId, manualMsgSeq dataSeq);
        // 发送取消召唤文件命令
        void cancalManual(string requestId, stringSeq deviceSeq);
        // 发送下载文件列表命令
        void sendFileList(string requestId, string deviceId, stringSeq fileSeq);
    };

    interface ManualDataManagerResponse
    {
        // 返回文件下载结果
        void receive(receiveMsgSeq recSeq);
        // 返回文件列表
        void receiveFileList(string requestId, string deviceId, ManualFile fileSeq);
        // 返回文件传输信息
        void receiveSegment(string requestId, string deviceId, ManualSegment fileSegment);
    };
};

#endif  //  _FEP_MANUALDATA_ICESTORM