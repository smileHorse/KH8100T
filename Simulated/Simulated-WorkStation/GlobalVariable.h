/*!
 * \file GlobalVariable.h
 * \date 2019/12/16 10:27
 *
 * \author Masf
 *
 * TODO: 全局变量
 *
 * \note
*/

#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H

#include <QtCore/QString>

class CGlobalVariable
{
public:
	static CGlobalVariable& instance() 
	{
		static CGlobalVariable globalVariable;
		return globalVariable;
	}

private:
	CGlobalVariable() {}

public:
	QString getRdbRequestTopic() const { return m_rdbRequestTopic; }
	void setRdbRequestTopic(const QString& topic) { m_rdbRequestTopic = topic; }

	QString getRdbRespondTopic() const { return m_rdbRespondTopic; }
	void setRdbRespondTopic(const QString& topic) { m_rdbRespondTopic = topic; }

private:
	QString m_rdbRequestTopic;
	QString m_rdbRespondTopic;
};
#endif