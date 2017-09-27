#ifndef HIDECOLUMNDIALOG_H
#define HIDECOLUMNDIALOG_H

#include <QtWidgets/QtWidgets>
#include <QtCore/QtCore>

#pragma execution_character_set("utf-8")

class HideColumnDialog : public QDialog
{
	Q_OBJECT

public:
	HideColumnDialog(QTableWidget* tableWidget, QWidget* parent = 0);

	QList<int>	getHideColumns();

private:
	void createWidget();
	void initColumnWidget();

private slots:
	void setHideColumns();

private:
	QTableWidget*	m_tableWidget;
	QList<int>		hideColumns;

	QListWidget*	columnWidget;
	QPushButton*	okBtn;
	QPushButton*	cancelBtn;
};
#endif