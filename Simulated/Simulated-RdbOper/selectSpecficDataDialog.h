#ifndef SELECTSPECFICDATADIALOG_H
#define SELECTSPECFICDATADIALOG_H

#include <QtWidgets/QtWidgets>

#include "rdbdata.h"

using namespace RdbRealData;

class SelectSpecficDataDialog : public QDialog
{
	Q_OBJECT

public:
	SelectSpecficDataDialog(const RdbDataOptPrx& rdbDataOptPrx, QWidget* parent = 0);

private:
	void createWidgets();
	void createLayout();
	void createConnectes();
	void updateTableWidget(const RespondSpecficDataSeq& repSeq);

private slots:
	void tableNameChanged(const QString& tableName);
	void queryData();
	void stopQueryData();

private:
	QSpinBox*		idSpinBox;
	QSpinBox*		requestIdSpinBox;
	QLineEdit*		requestNodeLineEdit;
	QCheckBox*		isStopCheckBox;
	QSpinBox*		refreshFreqSpinBox;
	QComboBox*		tableNameComboBox;
	QListWidget*	fieldSeqListWidget;
	QComboBox*		fieldNameComboBox;
	QLineEdit*		fieldValueLineEdit;

	QPushButton*	queryButton;
	QPushButton*	stopQueryButton;
	QTableWidget*	dataTableWidget;

	RdbDataOptPrx	m_rdbDataOptPrx;
};
#endif