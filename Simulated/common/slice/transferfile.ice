/* **************************************************
* Copyright(c) 2011-, JNKH. All rights reserved.
*
* File：transferFile.ice
* 
*****************************************************/ 
 
#ifndef _TRANSFERFILE_ICE
#define _TRANSFERFILE_ICE
 
[["java:package:com.gridnt.transferFile"]]
/**
* 
* 实时库同步
* 
*/
module TransferFileSpace
{ 
    ["java:type:java.util.ArrayList<Byte>"]   sequence<byte> Bytes;
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


    sequence<string>   FileDirSeq;
    interface TransferFile
    {
       // 请求实时库
       bool requestRealdb();

       /**
       *
       * 请求实时库内容
       * @parameter fileName: 请求的实时库名称
       * @parameter pos:上次读取文件位置
       * @return FileContent
       **/
       FileContent requestRealdbContent(string fileName, long pos);

      /**
       *
       * 请求历史曲线文件列表
       * @parameter sDate: 开始日期
       * @parameter eDate: 结束日期
       * @parameter fileList:文件列表
       * @return true：成功,false：失败
       **/
       bool requestHisCurveFileList(string sDate, string eDate, out FileDirSeq fileList);

       /**
       *
       * 请求历史曲线文件
       * @parameter date: 文件的上一层文件夹名称，为日期值，如2018-01-01
       * @parameter fileName: 告警文件名称, 为单元号，如13.dat
       * @parameter pos:上次读取文件位置
       * @return FileContent
       **/
       FileContent requestHisCurveFile(string date, string fileName, long pos);

      /**
       *
       * 请求告警文件列表
       * @parameter sDate: 开始日期
       * @parameter eDate: 结束日期
       * @parameter fileList:文件列表
       * @return true：成功,false：失败
       **/
       bool requestWarningFileList(string sDate, string eDate, out FileDirSeq fileList);

       /**
       *
       * 请求告警文件
       * @parameter fileName: 告警文件名称，如2018-1-1.msg
       * @parameter pos:上次读取文件位置
       * @return FileContent
       **/
       FileContent requestWarningFile(string fileName, long pos);

      /**
       *
       * 请求报表文件列表
       * @parameter folder: 文件的父文件夹名称，日报为rb,月波为yb,年报为nb
       * @parameter sDate: 开始日期
       * @parameter eDate: 结束日期
       * @parameter fileList:文件列表
       * @return true：成功,false：失败
       **/
       bool requestReportFileList(string folder, string sDate, string eDate, out FileDirSeq fileList);

       /**
       *
       * 请求报表文件
       * @parameter folder: 文件的父文件夹名称，日报类似rb/rb-01,月波为yb,年报为nb
       * @parameter fileName: 文件名称
       * @parameter pos:上次读取文件位置
       * @return FileContent
       **/
       FileContent requestReportFile(string folder, string fileName, long pos);


       /*************************  以下接口由数据服务器备机实现  *********************/
       /**
       *
       * 以二进制的形式发送文件内容
       * @parameter filePath: 文件全路径
       * @parameter datas: 文件内容
       * @parameter isFirst: 是否第一次发送 true: 是  false: 否
       **/
       void sendFileBinary(string filePath, Bytes datas, bool isFirst);

       /**
       *
       * 以二进制的形式发送实时库操作日志
       * @parameter title: 文件标题
       * @parameter datas: 文件内容
       * @parameter isFirst: 是否第一次发送 true: 是  false: 否
       **/
       void sendRdbLoggerBinary(string title, Bytes datas, bool isFirst);

       /**
       *
       * 实时库操作日志发送完成通知
       * @parameter title: 文件标题
       **/
       void sendRdbLoggerFinished(string title);

       /**
       *
       * 实时库操作日志同步完成通知
       **/
       void syncRdbFinished();
    };
};
 
#endif 