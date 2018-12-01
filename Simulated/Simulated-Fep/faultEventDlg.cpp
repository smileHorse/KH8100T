
#include "faultEventDlg.h"
#include "fepserverthread.h"

#include <QtWidgets/QtWidgets>

CFaultEventDlg::CFaultEventDlg(FepServerThread*	fepServerThreadPtr, QWidget* parent /*= 0*/)
	: QDialog(parent)
{
	m_fepServerThreadPtr = fepServerThreadPtr;

	createWidgets();
	createConnections();
}

void CFaultEventDlg::createWidgets()
{
	unitNoLabel = new QLabel(QStringLiteral("unitNo:"));
	unitNoSpinBox = createSpinBox(-1, 10000);
	unitNoLabel->setBuddy(unitNoSpinBox);

	moduleNoLabel = new QLabel(QStringLiteral("moduleNo:"));
	moduleNoSpinBox = createSpinBox(-1, 10000);
	moduleNoLabel->setBuddy(unitNoSpinBox);

	moduleTypeLabel = new QLabel(QStringLiteral("moduleType:"));
	moduleTypeSpinBox = createSpinBox(-1, 10000);
	moduleTypeLabel->setBuddy(moduleTypeSpinBox);

	infoNoLabel = new QLabel(QStringLiteral("infoNo:"));
	infoNoSpinBox = createSpinBox(-1, 10000);
	infoNoLabel->setBuddy(infoNoSpinBox);

	stateLabel = new QLabel(QStringLiteral("state:"));
	stateSpinBox = createSpinBox(-1, 10000);
	stateLabel->setBuddy(stateSpinBox);

	// 网格布局
	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->addWidget(unitNoLabel, 0, 0);
	gridLayout->addWidget(unitNoSpinBox, 0, 1);
	gridLayout->addWidget(moduleNoLabel, 1, 0);
	gridLayout->addWidget(moduleNoSpinBox, 1, 1);
	gridLayout->addWidget(moduleTypeLabel, 2, 0);
	gridLayout->addWidget(moduleTypeSpinBox, 2, 1);
	gridLayout->addWidget(infoNoLabel, 3, 0);
	gridLayout->addWidget(infoNoSpinBox, 3, 1);
	gridLayout->addWidget(stateLabel, 4, 0);
	gridLayout->addWidget(stateSpinBox, 4, 1);

	// 操作按钮布局
	okButton = new QPushButton(QStringLiteral("确定"));
	closeButton = new QPushButton(QStringLiteral("关闭"));

	connect(okButton, SIGNAL(clicked()), this, SLOT(saveConfigData()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	QHBoxLayout* btnLayout = new QHBoxLayout;
	btnLayout->addStretch(1);
	btnLayout->addWidget(okButton);
	btnLayout->addWidget(closeButton);
	btnLayout->addStretch(1);

	// 垂直布局
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(gridLayout);
	mainLayout->addLayout(btnLayout);

	setLayout(mainLayout);

	resize(800, 600);
	setWindowTitle(QStringLiteral("保护事项设置"));
}

QSpinBox* CFaultEventDlg::createSpinBox(int min, int max)
{
	QSpinBox* spinBox = new QSpinBox();
	spinBox->setMinimum(min);
	spinBox->setMaximum(max);
	return spinBox;
}

void CFaultEventDlg::createConnections()
{
	connect(this, SIGNAL(sendFaultEvent(int, int, int, int, int)), m_fepServerThreadPtr, 
		SLOT(processProTypeEvent(int, int, int, int, int)));
}

void CFaultEventDlg::saveConfigData()
{
	int unitNo = unitNoSpinBox->value();
	int moduleNo = moduleNoSpinBox->value();
	int moduleType = moduleTypeSpinBox->value();
	int infoNo = infoNoSpinBox->value();
	int state = stateSpinBox->value();

	emit sendFaultEvent(unitNo, moduleNo, moduleType, infoNo, state);
}
