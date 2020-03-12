
#include "BatchRequestDialog.h"
#include "IceHelper.h"
#include "RequestDataDialog.h"

int BatchRequestDialog::s_requestId = 0;

BatchRequestDialog::BatchRequestDialog(QWidget* parent /*= 0*/)
	: QDialog(parent)
{
	initDialog();
	setDialogData();
	createConnections();
}

void BatchRequestDialog::initDialog()
{
	m_listWidget = new QListWidget;
	m_listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	m_label = new QLabel(QStringLiteral("刷新时间(秒): "));
	m_spinBox = new QSpinBox;
	m_okButton = new QPushButton("OK");
	m_cancelButton = new QPushButton("Close");

	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->addWidget(m_label);
	hLayout->addWidget(m_spinBox);

	QHBoxLayout* commandLayout = new QHBoxLayout;
	commandLayout->addWidget(m_okButton);
	commandLayout->addWidget(m_cancelButton);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_listWidget);
	layout->addLayout(hLayout);
	layout->addLayout(commandLayout);
	
	setLayout(layout);
}

void BatchRequestDialog::setDialogData()
{
	m_spinBox->setValue(3);

	// 从实时库中加载配电所数据
	RdbDataItemVct items;
	CIceHelper::instance()->selectSubstations("", "", items);
	for (RdbDataItemVct::iterator it = items.begin(); it != items.end(); ++it)
	{
		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(it->name));
		item->setData(Qt::UserRole, QString::fromStdString(it->mrid));
		m_listWidget->addItem(item);
	}
}

void BatchRequestDialog::createConnections()
{
	connect(m_okButton, SIGNAL(clicked()), this, SLOT(doOk()));
	connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void BatchRequestDialog::doOk()
{
	int refreshFreq = m_spinBox->value();

	// 获取列表中所选择的配电所
	QList<QListWidgetItem *> items = m_listWidget->selectedItems();
	Q_FOREACH(QListWidgetItem* item, items) {
		string mrid = item->data(Qt::UserRole).toString().toStdString();
		string name = item->text().toStdString();
		string iedId = "";

		RemoteUnitItemVct unitItems;
		CIceHelper::instance()->selectRemoteUnits(ECRID, mrid, unitItems);
		if (!unitItems.empty())
		{
			iedId = unitItems[0].iedId;
		}

		// 每个配电所作为一个请求主题，向数据服务器发送请求

		// 查看配电所下的所有开关、刀闸
		RdbRealData::RequestCompleteDataSeq dataSeq;
		dataSeq.id = 0;
		dataSeq.isStop = false;
		dataSeq.refreshFreq = refreshFreq;
		dataSeq.requestNode = name;
		dataSeq.requestId = s_requestId++;

		RdbDataItemVct breakerItems;
		CIceHelper::instance()->selectBreakers(ECRID, mrid, breakerItems);
		for (RdbDataItemVct::iterator iter = breakerItems.begin(); iter != breakerItems.end(); ++iter)
		{
			RdbRealData::RequestCompleteData requestData;
			requestData.tableName = BREAKER;
			requestData.fieldName = MRID;
			requestData.fieldValue = iter->mrid;
			dataSeq.seq.push_back(requestData);
		}

		RdbDataItemVct disconnectorItems;
		CIceHelper::instance()->selectDisconnectors(ECRID, mrid, disconnectorItems);
		for (RdbDataItemVct::iterator iter = disconnectorItems.begin(); 
			iter != disconnectorItems.end(); ++iter)
		{
			RdbRealData::RequestCompleteData requestData;
			requestData.tableName = DISCONNECTOR;
			requestData.fieldName = MRID;
			requestData.fieldValue = iter->mrid;
			dataSeq.seq.push_back(requestData);
		}
		dataSeq.dataCount = dataSeq.seq.size();
		CRequestManager::addBatchCompleteDataSeq(dataSeq);

		// 查看配电所下的所有遥测、遥信
		RdbRealData::RequestCompleteDataSeq analogSeq;
		analogSeq.id = 0;
		analogSeq.isStop = false;
		analogSeq.refreshFreq = refreshFreq;
		analogSeq.requestNode = name;
		analogSeq.requestId = s_requestId++;

		RdbDataItemVct analogItems;
		CIceHelper::instance()->selectAnalogs(FTUUNITID, iedId, analogItems);
		for (RdbDataItemVct::iterator iter = analogItems.begin(); 
			iter != analogItems.end(); ++iter)
		{
			RdbRealData::RequestCompleteData requestData;
			requestData.tableName = ANALOG;
			requestData.fieldName = MRID;
			requestData.fieldValue = iter->mrid;

			analogSeq.seq.push_back(requestData);
		}
		analogSeq.dataCount = analogSeq.seq.size();
		CRequestManager::addBatchCompleteDataSeq(analogSeq);

		RdbRealData::RequestCompleteDataSeq discreteSeq;
		discreteSeq.id = 0;
		discreteSeq.isStop = false;
		discreteSeq.refreshFreq = refreshFreq;
		discreteSeq.requestNode = name;
		discreteSeq.requestId = s_requestId++;

		RdbDataItemVct discreteItems;
		CIceHelper::instance()->selectDiscretes(FTUUNITID, iedId, discreteItems);
		for (RdbDataItemVct::iterator iter = discreteItems.begin(); 
			iter != discreteItems.end(); ++iter)
		{
			RdbRealData::RequestCompleteData requestData;
			requestData.tableName = DISCRETE;
			requestData.fieldName = MRID;
			requestData.fieldValue = iter->mrid;

			discreteSeq.seq.push_back(requestData);
		}
		discreteSeq.dataCount = discreteSeq.seq.size();
		CRequestManager::addBatchCompleteDataSeq(discreteSeq);
	}

	QDialog::accept();
}
