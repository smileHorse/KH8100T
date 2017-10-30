#ifndef TRANSFERLOGGERI_H
#define TRANSFERLOGGERI_H

#include "translogger.h"

using namespace TransferRdbLogger;

class TransferRdbLoggerInfoI : public  TransferRdbLoggerInfo
{
public:
	TransferRdbLoggerInfoI();

	virtual void transferRdbLogger(const ::std::string&, const ::std::string&, 
		const ::Ice::Current& /* = ::Ice::Current() */);
};
#endif