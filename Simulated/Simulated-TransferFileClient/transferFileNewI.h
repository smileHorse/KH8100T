#ifndef TRANSFERFILENEW_I_H
#define TRANSFERFILENEW_I_H

#include <QtCore/QtCore>

#include "transferFileNew.h"

using namespace TransferFileSpace;

class TransferFileClient;

class TransferFileI : public TransferFile
{
public:
	TransferFileI(TransferFileClient* frame);

	virtual void sendFile(const ::std::string& filePath, const ::TransferFileSpace::Bytes& fileDatas, 
		const ::Ice::Current& /* = ::Ice::Current() */);

private:
	void saveFile_C(const ::std::string& filePath, const ::TransferFileSpace::Bytes& fileDatas);
	void saveFile_CPlusPlus(const ::std::string& filePath, const ::TransferFileSpace::Bytes& fileDatas);
	void updateFrameTextEdit();

private:
	TransferFileClient*	_frame;
	QVector<QString>	_textes;
};
#endif