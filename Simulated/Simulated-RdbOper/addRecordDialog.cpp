
#include "addRecordDialog.h"
#include "rdbTableDefine.h"

AddRecordDialog::AddRecordDialog(const RdbDataOptPrx& rdbDataOptPrx, QWidget* parent /*= 0*/ )
	: QDialog(parent), m_rdbDataOptPrx(rdbDataOptPrx)
{
	createWidgets();
	createLayout();
	createConnectes();
}

void AddRecordDialog::createWidgets()
{
	setWindowTitle(QStringLiteral("添加记录"));
	setWindowIcon(QIcon(":/addRecord.png"));
	resize(600, 600);

	tableNameComboBox = new QComboBox;
	QStringList tableNames = RdbTableFactory::getTableNames();
	tableNameComboBox->addItems(tableNames);
	if (!tableNames.isEmpty())
	{
		tableNameComboBox->setCurrentIndex(0);
	}

	tableValueTableWidget = new QTableWidget;
	tableValueTableWidget->setAlternatingRowColors(true);
	tableValueTableWidget->resize(400, 800);
	updateTableWidget(tableNameComboBox->currentText());

	addButton = new QPushButton(QIcon(":/addRecord.png"), QStringLiteral("增加记录"));
	saveButton = new QPushButton(QIcon(":/save.png"), QStringLiteral("保存记录"));
}

void AddRecordDialog::createLayout()
{
	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(QStringLiteral("表名:"), tableNameComboBox);
	formLayout->addRow(QStringLiteral("表值:"), tableValueTableWidget);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(saveButton);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	setLayout(mainLayout);
}

void AddRecordDialog::createConnectes()
{
	connect(tableNameComboBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(updateTableWidget(const QString&)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addRecord()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveRecords()));
}

void AddRecordDialog::addRecord()
{
	int columnCount = tableValueTableWidget->columnCount();
	tableValueTableWidget->insertColumn(columnCount);
}

void AddRecordDialog::saveRecords()
{
	if (!m_rdbDataOptPrx)
	{
		QMessageBox::warning(this, QStringLiteral("插入数据"), QStringLiteral("插入失败: Ice代理为空"));
		return;
	}

	try
	{
		RespondCompleteDataSeq repSeq;
		
		int rowCount = tableValueTableWidget->rowCount();
		int columnCount = tableValueTableWidget->columnCount();
		for (int col = 0; col < columnCount; ++col)
		{
			RespondCompleteData data;
			data.tableName = tableNameComboBox->currentText().toStdString();
			for (int row = 0; row < rowCount; ++row)
			{
				QTableWidgetItem* item = tableValueTableWidget->item(row, col);
				if (item)
				{
					data.dataValues.push_back(item->text().toStdString());
				}
				else
				{
					data.dataValues.push_back("");
				}
			}

			repSeq.seq.push_back(data);
		}

		RespondCompleteDataSequence repSequence;
		bool ret = m_rdbDataOptPrx->InsertData(repSeq, repSequence);
		if (!ret)
		{
			QMessageBox::warning(this, QStringLiteral("插入数据"), 
				QStringLiteral("插入数据失败"));
		}
		else
		{
			QMessageBox::warning(this, QStringLiteral("插入数据"), 
				QStringLiteral("插入数据成功"));
		}
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("插入数据"), 
			QStringLiteral("插入数据失败: %1").arg(ex.what()));
	}
}

void AddRecordDialog::updateTableWidget( const QString& tableName )
{
	tableValueTableWidget->clear();
	int rowCount = 0;
	while(rowCount = tableValueTableWidget->rowCount())
	{
		tableValueTableWidget->removeRow(rowCount - 1);
	}
	int columnCount = 0;
	while(columnCount = tableValueTableWidget->columnCount())
	{
		tableValueTableWidget->removeColumn(columnCount - 1);
	}

	QStringList tableFields = RdbTableFactory::getTableFields(tableName);
	tableValueTableWidget->setRowCount(tableFields.size());
	tableValueTableWidget->setVerticalHeaderLabels(tableFields);
}
