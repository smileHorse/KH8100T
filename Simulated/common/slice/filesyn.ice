/* **************************************************
* Copyright(c) 2011-, KHDQ, inc. All rights reserved.
*
* File£ºfilesyn.ice
* 
*
*****************************************************/ 



[["java:package:com.kh.icestorm"]]

module Filesyn
{

		
    struct FilesynCmdMsg
    {
    	string ip;
	string port;//
	string filepaths;
    };
    
    
    struct FilesynCmdReplyMsg
    {
    	string ip;
    	
    	string file;

	string flag;
   	};
	
    
    /**
    * 
    *
    */
    const string FilesynCmdRequestTopic = "filesyncmd_operation_Request";
    const string FilesynCmdResponseTopic = "filesyncmd_operation_Response";
    interface FilesynCmdManagerRequest
    {
    	void FilesynCmdRoute(FilesynCmdMsg reqSeq);	
    };
    
    interface FilesynCmdManagerResponse
    {
    	void TDU100GeneralCmdReply(FilesynCmdReplyMsg replySeq);
    };
};

