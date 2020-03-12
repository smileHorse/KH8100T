#ifndef FASTDBCOLLECTION_H
#define FASTDBCOLLECTION_H

#include <string>
#include <set>

#include "zthread/Mutex.h"
#include "zthread/Guard.h"

using namespace ZThread;
using namespace std;

class FastdbCollection {
public:
	void insertSubstation(const string& id);
	string getRandSubstation();

	void insertLine(const string& id);
	string getRandLine();

	void insertBreaker(const string& id);
	string getRandBreaker();

	void insertRemoteUnit(const string& id);
	string getRandRemoteUnit();

	void insertAnalog(const string& id);
	string getRandAnalog();

	void insertDiscrete(const string& id);
	string getRandDiscrete();

private:
	Mutex lock;
	set<string> m_substations;
	set<string> m_lines;
	set<string> m_breakers;
	set<string> m_remoteUnits;
	set<string> m_analogs;
	set<string> m_discretes;
};

#endif