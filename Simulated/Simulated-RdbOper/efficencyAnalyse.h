#ifndef EFFICIENCYANALYSE_H
#define EFFICIENCYANALYSE_H

#include <QtCore/QtCore>

struct SecondData
{
	int second;
	long	count;

	SecondData() : count(0)
	{}

	long sum() const
	{
		return (count > 2) ? (count / 2) : 1;
	}

	long average() const
	{
		return sum();
	}

	void clear()
	{
		second = 0;
		count = 0;
	}
};
typedef QList<SecondData> SecondDataList;

struct MinuteData
{
	int minute;
	SecondDataList secondDatas;

	long count() const
	{
		long count = 0;
		foreach(const SecondData& data, secondDatas)
		{
			count += data.sum();
		}
		return count;
	}

	long average() const
	{
		long sum = count();
		return secondDatas.isEmpty() ? 0 : (sum / secondDatas.size());
	}

	void clear() 
	{
		minute = 0;
		secondDatas.clear();
	}
};
typedef QList<MinuteData> MinuteDataList;

struct HourData
{
	int hour;
	MinuteDataList minuteDatas;

	long count() const
	{
		long sum = 0;
		foreach(const MinuteData& data, minuteDatas)
		{
			sum += data.count();
		}
		return sum;
	}

	long average() const
	{
		long sum = count();
		long secondCount = 0;
		foreach(const MinuteData& minuteData, minuteDatas) 
		{
			secondCount += minuteData.secondDatas.size();
		}
		return secondCount == 0 ? 0 : (sum / secondCount);
	}

	void clear()
	{
		hour = 0;
		minuteDatas.clear();
	}
};
typedef QList<HourData> HourDataList;

class EfficiencyAnalyseThread : public QThread
{
	Q_OBJECT

public:
	EfficiencyAnalyseThread(const QString& fileName, QObject* parent = 0);

	// 开始分析文件
	void startAnalyse();

protected:
	virtual void run();

private:
	// 转换字符串为QDateTime
	bool transferStringToDateTime(const QString& text, QDateTime& dateTime);

	// 产生分析信息
	void generateAnalyseInfo(QString& analyseInfo);

	// 计算记录行数
	long computeDataCount();

	// 计算运行时间
	void computeExecuteTime(int& executeSec, QString& executeTime);

	// 计算平均效率
	void computeExecuteEfficiency(QString& executeEfficiency);

	void outputAnalyseInfo(const QString& analyseInfo);

private:
	QString _fileName;
	HourDataList	_hourDatas;
	QDateTime		_startDateTime;
	QDateTime		_endDateTime;
};

#endif