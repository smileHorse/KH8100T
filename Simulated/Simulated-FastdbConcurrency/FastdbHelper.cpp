
#include "FastdbHelper.h"
#include "FastdbTableDefine.h"
#include "logger.h"

#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

#if _MSC_VER
#define snprintf _snprintf
#endif

string newGuid()
{
	GUID newId;
	CoCreateGuid(&newId);

	std::vector<unsigned char> _bytes;
	_bytes.reserve(16);
	{
		_bytes.push_back((unsigned char)((newId.Data1 >> 24) & 0xFF));
		_bytes.push_back((unsigned char)((newId.Data1 >> 16) & 0xFF));
		_bytes.push_back((unsigned char)((newId.Data1 >> 8) & 0xFF));
		_bytes.push_back((unsigned char)((newId.Data1) & 0xff));
		_bytes.push_back((unsigned char)((newId.Data2 >> 8) & 0xFF));
		_bytes.push_back((unsigned char)((newId.Data2) & 0xff));
		_bytes.push_back((unsigned char)((newId.Data3 >> 8) & 0xFF));
		_bytes.push_back((unsigned char)((newId.Data3) & 0xFF));
		_bytes.push_back((unsigned char)newId.Data4[0]);
		_bytes.push_back((unsigned char)newId.Data4[1]);
		_bytes.push_back((unsigned char)newId.Data4[2]);
		_bytes.push_back((unsigned char)newId.Data4[3]);
		_bytes.push_back((unsigned char)newId.Data4[4]);
		_bytes.push_back((unsigned char)newId.Data4[5]);
		_bytes.push_back((unsigned char)newId.Data4[6]);
		_bytes.push_back((unsigned char)newId.Data4[7]);
	};

	char one[10], two[6], three[6], four[6], five[14];

	snprintf(one, 10, "%02x%02x%02x%02x",
		_bytes[0], _bytes[1], _bytes[2], _bytes[3]);
	snprintf(two, 6, "%02x%02x",
		_bytes[4], _bytes[5]);
	snprintf(three, 6, "%02x%02x",
		_bytes[6], _bytes[7]);
	snprintf(four, 6, "%02x%02x",
		_bytes[8], _bytes[9]);
	snprintf(five, 14, "%02x%02x%02x%02x%02x%02x",
		_bytes[10], _bytes[11], _bytes[12], _bytes[13], _bytes[14], _bytes[15]);
	const std::string sep("-");
	std::string out(one);

	out += sep + two;
	out += sep + three;
	out += sep + four;
	out += sep + five;

	return out;
}

void FastdbHelper::insertFastdbData()
{
	int value = rand() % 5;
	try {
		switch(value) {
		case 0:
			insertSubstation();
			break;
		case 1:
			insertLine();
			break;
		case 2:
			insertBreaker();
			break;
		case 3:
			insertRemoteUnit();
			break;
		case 4:
			insertAnalog();
			break;
		case 5:
			insertDiscrete();
			break;
		}
	} catch(dbException& e) {
		RdbLog(CLogger::Log_ERROR, "插入过程发送异常: %s", e.what());
	} catch(...) {
		RdbLog(CLogger::Log_ERROR, "插入过程发送异常: 未知异常");
	}
}

void FastdbHelper::insertSubstation()
{
	Substation data;
	data.mRID = newGuid();
	data.name = newGuid();
	insert(data);

	m_fastdbCollection->insertSubstation(data.mRID);
	RdbLog(CLogger::Log_INFO, "修改线程%d 插入Substation完成...", id);
}

void FastdbHelper::insertLine()
{
	Line data;
	data.mRID = newGuid();
	data.name = newGuid();
	data.ec_type = "Substation";
	data.ec_rid = m_fastdbCollection->getRandSubstation();
	if (!data.ec_rid.empty())
	{
		insert(data);
		m_fastdbCollection->insertLine(data.mRID);
		RdbLog(CLogger::Log_INFO, "修改线程%d 插入Line完成...", id);
	}
}

void FastdbHelper::insertBreaker()
{
	Breaker data;
	data.mRID = newGuid();
	data.name = newGuid();
	data.ec_type = "Substation";
	data.ec_rid = m_fastdbCollection->getRandSubstation();
	if (!data.ec_rid.empty())
	{
		insert(data);
		m_fastdbCollection->insertBreaker(data.mRID);
		RdbLog(CLogger::Log_INFO, "修改线程%d 插入Breaker完成...", id);
	}
}

void FastdbHelper::insertRemoteUnit()
{
	RemoteUnit data;
	data.mRID = newGuid();
	data.IEDName = newGuid();
	data.ec_type = "Substation";
	data.ec_rid = m_fastdbCollection->getRandSubstation();
	if (!data.ec_rid.empty())
	{
		insert(data);
		m_fastdbCollection->insertRemoteUnit(data.mRID);
		RdbLog(CLogger::Log_INFO, "修改线程%d 插入RemoteUnit完成...", id);
	}
}

void FastdbHelper::insertAnalog()
{
	Analog data;
	data.mRID = newGuid();
	data.name = newGuid();
	data.psr_type = "Line";
	data.psr_rid = m_fastdbCollection->getRandLine();
	if (!data.psr_rid.empty())
	{
		insert(data);
		RdbLog(CLogger::Log_INFO, "修改线程%d 插入Analog完成...", id);
		m_fastdbCollection->insertAnalog(data.mRID);
	}
}

void FastdbHelper::insertDiscrete()
{
	Discrete data;
	data.mRID = newGuid();
	data.name = newGuid();
	data.psr_type = "Breaker";
	data.psr_rid = m_fastdbCollection->getRandBreaker();
	if (!data.psr_rid.empty())
	{
		insert(data);
		RdbLog(CLogger::Log_INFO, "修改线程%d 插入Discrete完成...", id);
		m_fastdbCollection->insertDiscrete(data.mRID);
	}
}

void FastdbHelper::updateFastdbData()
{
	int value = rand() % 5;
	try {
		switch(value) {
		case 0:
			updateSubstation();
			break;
		case 1:
			updateLine();
			break;
		case 2:
			updateBreaker();
			break;
		case 3:
			updateRemoteUnit();
			break;
		case 4:
			updateAnalog();
			break;
		case 5:
			updateDiscrete();
			break;
		}
	} catch(dbException& e) {
		RdbLog(CLogger::Log_ERROR, "修改过程发送异常: %s", e.what());
	} catch(...) {
		RdbLog(CLogger::Log_ERROR, "修改过程发送异常: 未知异常");
	}
}

void FastdbHelper::updateSubstation()
{
	string mrid = m_fastdbCollection->getRandSubstation();
	if (mrid.empty())
	{
		return;
	}

	dbCursor<Substation> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID = ", mrid;
	if (cursor.select(query) > 0)
	{
		cursor->name = newGuid();
		cursor.update();
		RdbLog(CLogger::Log_INFO, "修改线程%d 修改Substation完成...", id);
	}
}

void FastdbHelper::updateLine()
{
	string mrid = m_fastdbCollection->getRandLine();
	if (mrid.empty())
	{
		return;
	}

	dbCursor<Line> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID = ", mrid;
	if (cursor.select(query) > 0)
	{
		cursor->name = newGuid();
		cursor.update();
		RdbLog(CLogger::Log_INFO, "修改线程%d 修改Line完成...", id);
	}
}

void FastdbHelper::updateBreaker()
{
	string mrid = m_fastdbCollection->getRandBreaker();
	if (mrid.empty())
	{
		return;
	}

	dbCursor<Breaker> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID = ", mrid;
	if (cursor.select(query) > 0)
	{
		cursor->name = newGuid();
		cursor.update();
		RdbLog(CLogger::Log_INFO, "修改线程%d 修改Breaker完成...", id);
	}
}

void FastdbHelper::updateRemoteUnit()
{
	string mrid = m_fastdbCollection->getRandRemoteUnit();
	if (mrid.empty())
	{
		return;
	}

	dbCursor<RemoteUnit> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID = ", mrid;
	if (cursor.select(query) > 0)
	{
		cursor->IEDName = newGuid();
		cursor.update();
		RdbLog(CLogger::Log_INFO, "修改线程%d 修改RemoteUnit完成...", id);
	}
}

void FastdbHelper::updateAnalog()
{
	string mrid = m_fastdbCollection->getRandAnalog();
	if (mrid.empty())
	{
		return;
	}

	dbCursor<Analog> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID = ", mrid;
	if (cursor.select(query) > 0)
	{
		cursor->name = newGuid();
		cursor.update();
		RdbLog(CLogger::Log_INFO, "修改线程%d 修改Analog完成...", id);
	}
}

void FastdbHelper::updateDiscrete()
{
	string mrid = m_fastdbCollection->getRandDiscrete();
	if (mrid.empty())
	{
		return;
	}

	dbCursor<Discrete> cursor(dbCursorForUpdate);
	dbQuery query;
	query = "mRID = ", mrid;
	if (cursor.select(query) > 0)
	{
		cursor->name = newGuid();
		cursor.update();
		RdbLog(CLogger::Log_INFO, "修改线程%d 修改Discrete完成...", id);
	}
}


void FastdbHelper::selectFastdbData()
{
	int value = rand() % 5;
	try {
		switch(value) {
		case 0:
			selectSubstation();
			break;
		case 1:
			selectLine();
			break;
		case 2:
			selectBreaker();
			break;
		case 3:
			selectRemoteUnit();
			break;
		case 4:
			selectAnalog();
			break;
		case 5:
			selectDiscrete();
			break;
		}
	} catch(dbException& e) {
		RdbLog(CLogger::Log_ERROR, "查询过程发送异常: %s", e.what());
	} catch(...) {
		RdbLog(CLogger::Log_ERROR, "查询过程发送异常: 未知异常");
	}
}

void FastdbHelper::selectSubstation()
{
	dbCursor<Substation> cursor;
	int count = cursor.select();
	if (!cursor.isEmpty())
	{
		do 
		{
		} while (cursor.next());
	}
	RdbLog(CLogger::Log_INFO, "读取线程%d： Substation数据条数%d", id, count);
}

void FastdbHelper::selectLine()
{
	dbCursor<Line> cursor;
	int count = cursor.select();
	if (!cursor.isEmpty())
	{
		do 
		{
		} while (cursor.next());
	}
	RdbLog(CLogger::Log_INFO, "读取线程%d： Line数据条数%d", id, count);
}

void FastdbHelper::selectBreaker()
{
	dbCursor<Breaker> cursor;
	int count = cursor.select();
	if (!cursor.isEmpty())
	{
		do 
		{
		} while (cursor.next());
	}
	RdbLog(CLogger::Log_INFO, "读取线程%d： Breaker数据条数%d", id, count);
}

void FastdbHelper::selectRemoteUnit()
{
	dbCursor<RemoteUnit> cursor;
	int count = cursor.select();
	if (!cursor.isEmpty())
	{
		do 
		{
		} while (cursor.next());
	}
	RdbLog(CLogger::Log_INFO, "读取线程%d： RemoteUnit数据条数%d", id, count);
}

void FastdbHelper::selectAnalog()
{
	dbCursor<Analog> cursor;
	int count = cursor.select();
	if (!cursor.isEmpty())
	{
		do 
		{
		} while (cursor.next());
	}
	RdbLog(CLogger::Log_INFO, "读取线程%d： Analog数据条数%d", id, count);
}

void FastdbHelper::selectDiscrete()
{
	dbCursor<Discrete> cursor;
	int count = cursor.select();
	if (!cursor.isEmpty())
	{
		do 
		{
		} while (cursor.next());
	}
	RdbLog(CLogger::Log_INFO, "读取线程%d： Discrete数据条数%d", id, count);
}