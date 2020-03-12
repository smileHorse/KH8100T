#ifndef BATCHREQUEST_DIALOG_H
#define BATCHREQUEST_DIALOG_H

#include <QtWidgets/QtWidgets>

class BatchRequestDialog : public QDialog {
	Q_OBJECT

public:
	BatchRequestDialog(QWidget* parent = 0);

private:
	void initDialog();
	void setDialogData();
	void createConnections();

private slots:
	void doOk();

private:
	QListWidget* m_listWidget;
	QLabel* m_label;
	QSpinBox* m_spinBox;
	QPushButton* m_okButton;
	QPushButton* m_cancelButton;

	static int s_requestId;
};
#endif