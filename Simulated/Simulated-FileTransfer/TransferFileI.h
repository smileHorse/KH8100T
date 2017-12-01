#ifndef TRANSFERFILE_I_H
#define TRANSFERFILE_I_H

#include "transferfile.h"

using namespace TransferFileModule;

#define FILE_BUFFER_SIZE	BUFSIZ * 10
#define UPLOAD_PATH			"file/upload/"
#define DOWNLOAD_PATH		"file/download/"

class TransferFileManagerI : public TransferFileManager
{
public:
	TransferFileManagerI();

	virtual void uploadFile(const ::std::string& fileName, const ::TransferFileModule::Bytes& content, 
			const ::Ice::Current& /* = ::Ice::Current() */);

	virtual FileContent downloadFile(const ::std::string& fileName, Ice::Long pos, const ::Ice::Current& /* = ::Ice::Current() */);

};
#endif