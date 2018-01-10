
#include <IceUtil/IceUtil.h>

#include "logger.h"
#include "mRIDXmlHandler.h"
#include "randomInsertThread.h"
#include "rdbTableDefine.h"

RandomInsertThread::RandomInsertThread(QObject* parent /*= 0*/ )
	: QThread(parent), m_stop(false), m_count(0)
{
	readTableMRIDs();
}

void RandomInsertThread::setStop( bool stop )
{
	m_stop = stop;
}

void RandomInsertThread::setTableName( const QString& tableName )
{
	m_tableName = tableName;
}

void RandomInsertThread::setCount( int count )
{
	m_count = count;
}

void RandomInsertThread::setRdbDataOptPrx( const RdbDataOptPrx& rdbDataOptPrx )
{
	m_rdbDataOptPrx = rdbDataOptPrx;
}

void RandomInsertThread::run()
{
	int count = 0;
	int successCount = 0;
	int failCount = 0;
	qsrand(QTime::currentTime().msec());
	while(!m_stop && count < m_count)
	{
		RespondCompleteDataSeq repSeq;
		repSeq.id = 1;
		repSeq.requestId = 1;
		repSeq.requestNode = "randomInsert";

		RespondCompleteData data;
		data.tableName = m_tableName.toStdString();
		getRandomDataValues(m_tableName, data.dataValues);

		repSeq.seq.push_back(data);
		repSeq.dataCount = repSeq.seq.size();

		logData(m_tableName, data.dataValues);
		
		RespondCompleteDataSequence repSequence;
		bool result = m_rdbDataOptPrx->InsertData(repSeq, repSequence);
		if (result)
		{
			++successCount;
			RdbLog(CLogger::Log_INFO, "插入成功");
		}
		else
		{
			++failCount;
			RdbLog(CLogger::Log_INFO, "插入失败");
		}

		++count;
		emit updateInsertCount(count);
	}
	emit updateInsertCount(count);

	if (!m_stop)
	{
		// 插入完毕
		QString text = QStringLiteral("插入完成，共插入数据 %1 条: 成功 %2 条, 失败 %3 条")
						.arg(count).arg(successCount).arg(failCount);
		emit updateResultText(text);
	}
	else
	{
		// 插入未完成，提前终止
		QString text = QStringLiteral("插入人工终止，共插入数据 %1 条: 成功 %2 条, 失败 %3 条")
			.arg(count).arg(successCount).arg(failCount);
		emit updateResultText(text);
	}
	m_stop = false;
	saveTableMRIDs();
}

void RandomInsertThread::getRandomDataValues( const QString& tableName, RdbRealData::Strings& dataValues )
{
	// 根据表名获取表的字段信息
	RdbFieldStructList fields;
	RdbTableFactory::getTableFields(tableName, fields);

	// 为字段赋予随机值
	foreach(const RdbFieldStruct& field, fields)
	{
		QString fieldType = field.fieldType;
		QString fieldName = field.fieldName;
		if (fieldType == "string")
		{
			if (fieldName == "mRID")
			{
				dataValues.push_back(generateTableMRID(tableName));
			}
			else if (fieldName == "ec_type" || fieldName == "psrType")
			{
				dataValues.push_back(fieldType.toStdString());
			}
			else
			{
				QString text = QString("%1-%2").arg(tableName).arg(qrand());
				dataValues.push_back(text.toStdString());
			}
		}
		else if (fieldType == "int")
		{
			int value = qrand();
			dataValues.push_back(QString::number(value).toStdString());
		}
		else if (fieldType == "double")
		{
			double value = ((double)qrand() * 5.0) / 3.0;
			dataValues.push_back(QString::number(value).toStdString());
		}
		else if (fieldType == "bool")
		{
			bool value = (qrand() % 2 == 0);
			dataValues.push_back(QString::number(value).toStdString());
		}
		else
		{
			if (m_tableMRIDs.contains(fieldType))
			{
				QVector<string> mRIDs = m_tableMRIDs[fieldType];
				if (!mRIDs.isEmpty())
				{
					int index = qrand() % mRIDs.size();
					dataValues.push_back(mRIDs.at(index));
				}
				else
				{
					dataValues.push_back("");
				}
			}
			else
			{
				dataValues.push_back("");
			}
		}
	}
}

std::string RandomInsertThread::generateTableMRID( const QString& tableName )
{
	string mRID = IceUtil::generateUUID();
	if (m_tableMRIDs.contains(tableName))
	{
		m_tableMRIDs[tableName].push_back(mRID);
	}
	else
	{
		QVector<string> mRIDs;
		mRIDs.push_back(mRID);
		m_tableMRIDs.insert(tableName, mRIDs);
	}
	return mRID;
}

void RandomInsertThread::logData( const QString& tableName, RdbRealData::Strings& dataValues )
{
	string datas("");
	for (size_t i = 0; i < dataValues.size(); ++i)
	{
		datas += (i == 0 ? "" : ", ");
		datas += dataValues[i];
	}
	RdbLog(CLogger::Log_INFO, "实时库表 %s 插入数据 %s", tableName.toStdString().c_str(), datas.c_str());
}

void RandomInsertThread::readTableMRIDs()
{
	MRIDXmlHandler xmlHandler;
	xmlHandler.readXml(m_tableMRIDs);
}

void RandomInsertThread::saveTableMRIDs()
{
	MRIDXmlHandler xmlHandler;
	xmlHandler.writeXml(m_tableMRIDs);
}
