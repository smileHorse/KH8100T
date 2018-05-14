
#include "rdbTableFactory.h"
#include "selectEquipTreeDialog.h"

#define TEXT_SEPERATOR	"%"

SelectEquipTreeDialog::SelectEquipTreeDialog(const RdbDataOptPrx& rdbDataOptPrx,  QWidget* parent /*= 0*/)
	: QDialog(parent), m_rdbDataOptPrx(rdbDataOptPrx)
{
	createWidgets();
	createLayout();
	createConnectes();
}

void SelectEquipTreeDialog::createWidgets()
{
	setWindowTitle(QStringLiteral("查询全部数据"));
	setWindowIcon(QIcon(":/select.png"));
	resize(900, 700);

	idSpinBox = new QSpinBox();
	idSpinBox->setRange(1, 100);

	requestIdSpinBox = new QSpinBox;
	requestIdSpinBox->setRange(1, 100);

	requestNodeLineEdit = new QLineEdit;
	requestNodeLineEdit->setText("simulated-request");

	isStopCheckBox = new QCheckBox;
	isStopCheckBox->setChecked(true);

	refreshFreqSpinBox = new QSpinBox;
	refreshFreqSpinBox->setRange(0, 100);

	tableNameComboBox = new QComboBox;
	QStringList tableNames = RdbTableFactory::getTableNames();
	tableNameComboBox->addItems(tableNames);

	fieldNameComboBox = new QComboBox;
	tableNameChanged(tableNameComboBox->currentText());

	fieldValueLineEdit = new QLineEdit;

	queryButton = new QPushButton(QIcon(":/select.png"), QStringLiteral("查询"));
	stopQueryButton = new QPushButton(QIcon(":/stop.png"), QStringLiteral("停止查询"));

	dataTableWidget = new QTableWidget;
	dataTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	dataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	dataTableWidget->setSortingEnabled(true);
	dataTableWidget->setAlternatingRowColors(true);
}

void SelectEquipTreeDialog::createLayout()
{
	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(QStringLiteral("&Id:"), idSpinBox);
	formLayout->addRow(QStringLiteral("&RequestId:"), requestIdSpinBox);
	formLayout->addRow(QStringLiteral("RequestNode:"), requestNodeLineEdit);
	formLayout->addRow(QStringLiteral("IsStop:"), isStopCheckBox);
	formLayout->addRow(QStringLiteral("RefreshFreq"), refreshFreqSpinBox);
	formLayout->addRow(QStringLiteral("TableName:"), tableNameComboBox);
	formLayout->addRow(QStringLiteral("FieldName:"), fieldNameComboBox);
	formLayout->addRow(QStringLiteral("FieldValue:"), fieldValueLineEdit);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(queryButton);
	buttonLayout->addWidget(stopQueryButton);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addWidget(dataTableWidget);

	setLayout(mainLayout);
}

void SelectEquipTreeDialog::createConnectes()
{
	connect(tableNameComboBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(tableNameChanged(const QString&)));
	connect(queryButton, SIGNAL(clicked()), this, SLOT(queryData()));
	connect(stopQueryButton, SIGNAL(clicked()), this, SLOT(stopQueryData()));
}

void SelectEquipTreeDialog::updateTableWidget(const EquipTreeSequence& repSeq)
{
	dataTableWidget->clear();
	int rowCount = 0;
	while(rowCount = dataTableWidget->rowCount())
	{
		dataTableWidget->removeRow(rowCount - 1);
	}
	int columnCount = 0;
	while(columnCount = dataTableWidget->columnCount())
	{
		dataTableWidget->removeColumn(columnCount - 1);
	}

	QStringList headerLabels;
	headerLabels << m_currFieldNames;
	dataTableWidget->setColumnCount(headerLabels.size());
	dataTableWidget->setHorizontalHeaderLabels(headerLabels);

	for (size_t i = 0; i < repSeq.size(); ++i)
	{
		dataTableWidget->insertRow(i);

		QTableWidgetItem* item = new QTableWidgetItem(QString().fromStdString(repSeq.at(i).parentRid));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		dataTableWidget->setItem(i, 0, item);

		item = new QTableWidgetItem(QString().fromStdString(repSeq.at(i).parentName));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		dataTableWidget->setItem(i, 1, item);

		item = new QTableWidgetItem(QString().fromStdString(repSeq.at(i).parentType));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		dataTableWidget->setItem(i, 2, item);

		item = new QTableWidgetItem(QString().fromStdString(repSeq.at(i).equipRid));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		dataTableWidget->setItem(i, 3, item);

		item = new QTableWidgetItem(QString().fromStdString(repSeq.at(i).equipName));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		dataTableWidget->setItem(i, 4, item);

		item = new QTableWidgetItem(QString().fromStdString(repSeq.at(i).equipType));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		dataTableWidget->setItem(i, 5, item);
	}
	dataTableWidget->resizeColumnsToContents();
}

void SelectEquipTreeDialog::tableNameChanged( const QString& tableName )
{
	fieldNameComboBox->clear();
	fieldNameComboBox->addItem("");

	m_currFieldNames.clear();
	m_currFieldNames << "parentRid" << "parentName" << "parentType" 
		<< "equipRid" << "equipName" << "equipType";
	if (!m_currFieldNames.isEmpty())
	{
		fieldNameComboBox->addItems(m_currFieldNames);
		fieldNameComboBox->setCurrentIndex(0);
	}
}

void SelectEquipTreeDialog::queryData()
{
	if (!m_rdbDataOptPrx)
	{
		QMessageBox::warning(this, QStringLiteral("查询全部数据"), QStringLiteral("查询失败: Ice代理为空"));
		return;
	}

	try
	{
		// 准备请求条件
		RequestCompleteDataSeq reqSeq;
		reqSeq.id = idSpinBox->value();
		reqSeq.requestId = requestIdSpinBox->value();
		reqSeq.requestNode = requestNodeLineEdit->text().toStdString();
		reqSeq.isStop = isStopCheckBox->isChecked();
		reqSeq.refreshFreq = refreshFreqSpinBox->value();

		RequestCompleteData reqData;
		reqData.tableName = tableNameComboBox->currentText().toStdString();
		reqData.fieldName = fieldNameComboBox->currentText().toStdString();
		reqData.fieldValue = fieldValueLineEdit->text().toStdString();

		reqSeq.seq.push_back(reqData);
		reqSeq.dataCount = reqSeq.seq.size();

		EquipTreeSequence repSeq;
		m_rdbDataOptPrx->GetEquipTree(reqData.tableName, reqData.fieldValue, repSeq);

		updateTableWidget(repSeq);
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("查询全部数据"), 
			QStringLiteral("查询数据失败: %1").arg(ex.what()));
	}
}

void SelectEquipTreeDialog::stopQueryData()
{

}
