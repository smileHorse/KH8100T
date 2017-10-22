#ifndef FASTDBTABLEDEFINE_H
#define FASTDBTABLEDEFINE_H

#include "fastdb.h"

#include <string>
#include <sstream>

using namespace std;

class Record
{
public:
	string	id;
	int		iValue;
	double	dValue;

	string output()
	{
		ostringstream stream;
		stream << "id: " << id << "\tiValue: " << iValue << "\tdValue: " << dValue;
		return stream.str();
	}

	TYPE_DESCRIPTOR((KEY(id, INDEXED),
		FIELD(iValue),
		FIELD(dValue)));
};
#endif