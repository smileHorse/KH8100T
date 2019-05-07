
#include "rdbTableFactory.h"
#include "selectSpecficDataDialog.h"

SelectSpecficDataDialog::SelectSpecficDataDialog(const RdbDataOptPrx& rdbDataOptPrx,  QWidget* parent /*= 0*/)
	: QDialog(parent), m_rdbDataOptPrx(rdbDataOptPrx)
{
	createWidgets();
	createLayout();
	createConnectes();
}

void SelectSpecficDataDialog::createWidgets()
{
	setWindowTitle(QStringLiteral("查询指定数据"));
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

	fieldSeqListWidget = new QListWidget;
	fieldSeqListWidget->setAlternatingRowColors(true);
	fieldSeqListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

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

void SelectSpecficDataDialog::createLayout()
{
	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(QStringLiteral("&Id:"), idSpinBox);
	formLayout->addRow(QStringLiteral("&RequestId:"), requestIdSpinBox);
	formLayout->addRow(QStringLiteral("RequestNode:"), requestNodeLineEdit);
	formLayout->addRow(QStringLiteral("IsStop:"), isStopCheckBox);
	formLayout->addRow(QStringLiteral("RefreshFreq"), refreshFreqSpinBox);
	formLayout->addRow(QStringLiteral("TableName:"), tableNameComboBox);
	formLayout->addRow(QStringLiteral("FieldSeq:"), fieldSeqListWidget);
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

void SelectSpecficDataDialog::createConnectes()
{
	connect(tableNameComboBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(tableNameChanged(const QString&)));
	connect(queryButton, SIGNAL(clicked()), this, SLOT(queryData()));
	connect(stopQueryButton, SIGNAL(clicked()), this, SLOT(stopQueryData()));
}

void SelectSpecficDataDialog::updateTableWidget(const RespondSpecficDataSeq& repSeq)
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
	headerLabels << "dataRid";
	if (!repSeq.seq.empty())
	{
		for (int i = 0; i < repSeq.seq.at(0).fieldSeq.size(); ++i)
		{
			headerLabels << QString().fromStdString(repSeq.seq.at(0).fieldSeq.at(i));
		}
	}
	dataTableWidget->setRowCount(headerLabels.size());
	dataTableWidget->setVerticalHeaderLabels(headerLabels);

	for (size_t i = 0; i < repSeq.seq.size(); ++i)
	{
		dataTableWidget->insertColumn(i);

		QTableWidgetItem* ridItem = new QTableWidgetItem(QString().fromStdString(repSeq.seq.at(0).dataRid));
		dataTableWidget->setItem(0, i, ridItem);
		for (int j = 0; j < repSeq.seq.at(i).dataValues.size(); ++j)
		{
			QTableWidgetItem* item = new QTableWidgetItem(QString().fromStdString(repSeq.seq.at(i).dataValues.at(j)));
			item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			dataTableWidget->setItem(j + 1, i, item);
		}
	}
	dataTableWidget->resizeColumnsToContents();
}

void SelectSpecficDataDialog::tableNameChanged( const QString& tableName )
{
	fieldSeqListWidget->clear();
	fieldNameComboBox->clear();

	fieldNameComboBox->addItem("");
	QStringList fieldNames = RdbTableFactory::getTableFields(tableName);
	if (!fieldNames.isEmpty())
	{
		fieldSeqListWidget->insertItems(0, fieldNames);

		fieldNameComboBox->addItems(fieldNames);
		fieldNameComboBox->setCurrentIndex(0);
	}
}

void SelectSpecficDataDialog::queryData()
{
	if (!m_rdbDataOptPrx)
	{
		QMessageBox::warning(this, QStringLiteral("查询指定数据"), QStringLiteral("查询失败: Ice代理为空"));
		return;
	}

	try
	{
		// 准备请求条件
		RequestSpecficDataSeq reqSeq;
		reqSeq.id = idSpinBox->value();
		reqSeq.requestId = requestIdSpinBox->value();
		reqSeq.requestNode = requestNodeLineEdit->text().toStdString();
		reqSeq.isStop = isStopCheckBox->isChecked();
		reqSeq.refreshFreq = refreshFreqSpinBox->value();

		RequestSpecficData reqData;
		reqData.tableName = tableNameComboBox->currentText().toStdString();
		QList<QListWidgetItem*> selectedItems = fieldSeqListWidget->selectedItems();
		foreach(QListWidgetItem* item, selectedItems)
		{
			reqData.fieldSeq.push_back(item->text().toStdString());
		}
		reqData.fieldName = fieldNameComboBox->currentText().toStdString();
		reqData.fieldValue = fieldValueLineEdit->text().toStdString();

		reqSeq.seq.push_back(reqData);
		reqSeq.dataCount = reqSeq.seq.size();

		RespondSpecficDataSeq repSeq;
		m_rdbDataOptPrx->SelectSpecificData(reqSeq, repSeq);

		updateTableWidget(repSeq);
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("查询指定数据"), 
			QStringLiteral("查询数据失败: %1").arg(ex.what()));
	}
}

void SelectSpecficDataDialog::stopQueryData()
{

}
