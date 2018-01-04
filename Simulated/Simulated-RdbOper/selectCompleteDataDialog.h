#ifndef SELECTCOMPLETEDATADIALOG_H
#define SELECTCOMPLETEDATADIALOG_H

#include <QtWidgets/QtWidgets>

#include "rdbdata.h"

using namespace RdbRealData;

class SelectCompleteDataDialog : public QDialog
{
	Q_OBJECT

public:
	SelectCompleteDataDialog(const RdbDataOptPrx& rdbDataOptPrx, QWidget* parent = 0);

private:
	void createWidgets();
	void createLayout();
	void createConnectes();
	void updateTableWidget(const RespondCompleteDataSeq& repSeq);

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
	QComboBox*		fieldNameComboBox;
	QLineEdit*		fieldValueLineEdit;

	QPushButton*	queryButton;
	QPushButton*	stopQueryButton;
	QTableWidget*	dataTableWidget;

	RdbDataOptPrx	m_rdbDataOptPrx;

	QStringList		m_currFieldNames;
};
#endif