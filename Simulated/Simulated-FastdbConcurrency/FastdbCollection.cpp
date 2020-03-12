
#include "FastdbCollection.h"

void FastdbCollection::insertSubstation(const string& id) {
	Guard<Mutex> g(lock);
	m_substations.insert(id);
}

string FastdbCollection::getRandSubstation() {
	Guard<Mutex> g(lock);
	if (m_substations.empty())
	{
		return "";
	}
	int index = rand() % (int)m_substations.size();
	if (index >= 0 && index < m_substations.size())
	{
		set<string>::iterator iter = m_substations.begin();
		std::advance(iter, index);
		return *iter;
	}
	return "";
}

void FastdbCollection::insertLine(const string& id) {
	Guard<Mutex> g(lock);
	m_lines.insert(id);
}

string FastdbCollection::getRandLine() {
	Guard<Mutex> g(lock);

	if (m_lines.empty())
	{
		return "";
	}
	int index = rand() % (int)m_lines.size();
	if (index >= 0 && index < m_lines.size())
	{
		set<string>::iterator iter = m_lines.begin();
		std::advance(iter, index);
		return *iter;
	}
	return "";
}

void FastdbCollection::insertBreaker(const string& id)
{
	Guard<Mutex> g(lock);
	m_breakers.insert(id);
}

std::string FastdbCollection::getRandBreaker()
{
	Guard<Mutex> g(lock);

	if (m_breakers.empty())
	{
		return "";
	}
	int index = rand() % (int)m_breakers.size();
	if (index >= 0 && index < m_breakers.size())
	{
		set<string>::iterator iter = m_breakers.begin();
		std::advance(iter, index);
		return *iter;
	}
	return "";
}

void FastdbCollection::insertRemoteUnit(const string& id)
{
	Guard<Mutex> g(lock);
	m_remoteUnits.insert(id);
}

std::string FastdbCollection::getRandRemoteUnit()
{
	Guard<Mutex> g(lock);

	if (m_remoteUnits.empty())
	{
		return "";
	}
	int index = rand() % (int)m_remoteUnits.size();
	if (index >= 0 && index < m_remoteUnits.size())
	{
		set<string>::iterator iter = m_remoteUnits.begin();
		std::advance(iter, index);
		return *iter;
	}
	return "";
}

void FastdbCollection::insertAnalog(const string& id)
{
	Guard<Mutex> g(lock);
	m_analogs.insert(id);
}

std::string FastdbCollection::getRandAnalog()
{
	Guard<Mutex> g(lock);

	if (m_analogs.empty())
	{
		return "";
	}
	int index = rand() % (int)m_analogs.size();
	if (index >= 0 && index < m_analogs.size())
	{
		set<string>::iterator iter = m_analogs.begin();
		std::advance(iter, index);
		return *iter;
	}
	return "";
}

void FastdbCollection::insertDiscrete(const string& id)
{
	Guard<Mutex> g(lock);
	m_discretes.insert(id);
}

std::string FastdbCollection::getRandDiscrete()
{
	Guard<Mutex> g(lock);

	if (m_discretes.empty())
	{
		return "";
	}
	int index = rand() % (int)m_discretes.size();
	if (index >= 0 && index < m_discretes.size())
	{
		set<string>::iterator iter = m_discretes.begin();
		std::advance(iter, index);
		return *iter;
	}
	return "";
}