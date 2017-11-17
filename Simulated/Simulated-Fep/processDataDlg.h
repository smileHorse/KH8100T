#ifndef PROCESSDATADLG_H
#define PROCESSDATADLG_H

#include "fepdata.h"

#include <QtWidgets/QDialog>
#include <QtCore/QtCore>
#include <vector>
using namespace std;

class QLabel;
class QLineEdit;
class QRadioButton;
class QTableWidget;
class QPushButton;

enum DataType {
	AllDataType,        // 0-全数据
	ChangedAnalogType,  // 1-变化遥测
	AccumulatorType,    // 2-电度
	UnitStateType       // 3-终端状态
};

enum AllDataTypeType
{
	Analog,
	Discrete,
	Accumulator
};

enum GenerateValueMode
{
	Random,
	Specific,
	Timed,
	UnKnown
};

class SelfDataPacket : public QObject
{
	Q_OBJECT

public:
	SelfDataPacket(QObject* parent = 0)
		: QObject(parent)
	{}

	SelfDataPacket(const SelfDataPacket& packet)
	{
		this->id = packet.id;
		this->fepNode = packet.fepNode;
		this->type = packet.type;
		this->unitNo = packet.unitNo;
		this->units.assign(packet.units.begin(), packet.units.end());
		this->analogs.assign(packet.analogs.begin(), packet.analogs.end());
		this->discretes.assign(packet.discretes.begin(), packet.discretes.end());
		this->accmulators.assign(packet.accmulators.begin(), packet.accmulators.end());
	}

public:
	int			id;       // 数据包ID
	QString     fepNode;  // 所属前置机    
	DataType    type;     // 数据类型
	short       unitNo;   // 单元号
	::FepData::UnitSeq             units;           // 终端状态
	vector<double>	analogs;         // 遥测
	vector<int>		discretes;       // 遥信
	vector<double>	accmulators;     // 电度
};

class ProcessDataDialog : public QDialog
{
	Q_OBJECT

public:
	ProcessDataDialog(DataType type, AllDataTypeType alltype, QWidget* parent = 0);

private:
	void createWidgets();
	void createConnections();
	QString getWindowTitle() const;
	QString getAllDataTypeTitle() const;
	GenerateValueMode	getValueMode() const;
	void	getDataPacket(SelfDataPacket& packet);
	void generateYcValues();
	void generateYxValues();
	void generateDdValues();

signals:
	void start(SelfDataPacket);

private slots:
	void generateValue();
	void startProcessData();
	void cancelProcessData();
	void processRandomData();

private:
	DataType	dataType;
	AllDataTypeType	allDataTypeType;
	
	QLabel*		idLable;		// 数据包id
	QLineEdit*	idEdit;
	QLabel*		fepNodeLable;	// 前置机
	QLineEdit*	fepNodeEdit;
	QLabel*		typeLable;		// 数据类型
	QLineEdit*	typeEdit;
	QLabel*		unitNoLable;	// 单元编号
	QLineEdit*	unitNoEdit;

	QLabel*		valueModeLable;	// 产生值的方式
	QRadioButton*	specificRadio;
	QRadioButton*	randomRadio;
	QRadioButton*	timerRadio;

	QLabel*		sendCountLable;	// 发送次数
	QLineEdit*	sendCountEdit;

	QTableWidget*	dataTable;	// 数据表格

	QPushButton*	startBtn;
	QPushButton*	cancelBtn;
	QPushButton*	closeBtn;

	QSharedPointer<QTimer>	m_timer;
	int				m_processCount;
};

#endif