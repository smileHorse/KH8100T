
#include "randomInsertDialog.h"
#include "rdbTableFactory.h"

RandomInsertDialog::RandomInsertDialog(const RdbDataOptPrx& rdbDataOptPrx,  QWidget* parent /*= 0*/ )
	: QDialog(parent),m_rdbDataOptPrx(rdbDataOptPrx)
{
	createWidgets();
	createLayout();
	createConnectes();
}

void RandomInsertDialog::createWidgets()
{
	setWindowTitle(QStringLiteral("插入随机数据"));
	setWindowIcon(QIcon(":/random.png"));
	resize(400, 400);

	tableNameComboBox = new QComboBox;
	QStringList tableNames = RdbTableFactory::getTableNames();
	tableNameComboBox->addItems(tableNames);

	countSpinBox = new QSpinBox;
	countSpinBox->setRange(1, 20000000);

	progressBar = new QProgressBar;
	progressBar->setMinimum(0);

	resultTextEdit = new QTextEdit;

	insertPushButton = new QPushButton(QStringLiteral("插入"));
	stopPushButton = new QPushButton(QStringLiteral("停止"));
	stopPushButton->setEnabled(false);
}

void RandomInsertDialog::createLayout()
{
	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(QStringLiteral("表名:"), tableNameComboBox);
	formLayout->addRow(QStringLiteral("数据个数:"), countSpinBox);
	formLayout->addRow(QStringLiteral("插入进度:"), progressBar);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(insertPushButton);
	buttonLayout->addWidget(stopPushButton);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(formLayout);
	layout->addLayout(buttonLayout);
	layout->addWidget(resultTextEdit);
	setLayout(layout);
}

void RandomInsertDialog::createConnectes()
{
	connect(insertPushButton, SIGNAL(clicked()), this, SLOT(insertData()));
	connect(stopPushButton, SIGNAL(clicked()), this, SLOT(stopInsertData()));
	connect(countSpinBox, SIGNAL(valueChanged(int)), progressBar, SLOT(setMaximum(int)));

	connect(&m_thread, SIGNAL(updateInsertCount(int)), this, SLOT(updateProgessBar(int)));
	connect(&m_thread, SIGNAL(updateResultText(const QString&)), this, SLOT(updateTextEdit(const QString&)));
	connect(&m_thread, SIGNAL(finished()), this, SLOT(threadFinish()));
}

void RandomInsertDialog::insertData()
{
	insertPushButton->setEnabled(false);
	stopPushButton->setEnabled(true);

	QString tableName = tableNameComboBox->currentText();
	int		count = countSpinBox->value();

	m_thread.setStop(false);
	m_thread.setTableName(tableName);
	m_thread.setCount(count);
	m_thread.setRdbDataOptPrx(m_rdbDataOptPrx);
	m_thread.start();
}

void RandomInsertDialog::stopInsertData()
{
	insertPushButton->setEnabled(true);
	stopPushButton->setEnabled(false);

	if (m_thread.isRunning())
	{
		m_thread.setStop(true);
	}
}

void RandomInsertDialog::updateProgessBar( int value )
{
	if (value > 0 && value < progressBar->maximum())
	{
		progressBar->setValue(value);
	}
}

void RandomInsertDialog::updateTextEdit( const QString& text )
{
	resultTextEdit->moveCursor(QTextCursor::End);
	resultTextEdit->insertPlainText(text);
	resultTextEdit->insertPlainText("\n");
	resultTextEdit->moveCursor(QTextCursor::End);
}

void RandomInsertDialog::threadFinish()
{
	insertPushButton->setEnabled(true);
	stopPushButton->setEnabled(false);
}
