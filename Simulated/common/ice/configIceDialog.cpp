
#include "configIceDialog.h"
#include "configXmlHandler.h"

ConfigIceDialog::ConfigIceDialog(const QIcon& icon, QWidget* parent /*= 0*/ )
	: QDialog(parent), m_icon(icon)
{
	createDialogs();
	createLayout();
}

void ConfigIceDialog::createDialogs()
{
	ConfigIceInfo info;
	loadConfigInfo(info);

	iceIpLineEdit = new QLineEdit(info.iceIp);
	icePortSpinBox = new QSpinBox;
	icePortSpinBox->setRange(10000, 30000);
	icePortSpinBox->setValue(info.icePort);

	iceOffsideIpLineEdit = new QLineEdit(info.iceOffsideIp);
	iceOffsidePortSpinBox = new QSpinBox;
	iceOffsidePortSpinBox->setRange(10000, 30000);
	iceOffsidePortSpinBox->setValue(info.iceOffsidePort);

	iceStormIpLineEdit = new QLineEdit(info.iceStormIp);
	iceStormPortSpinBox = new QSpinBox;
	iceStormPortSpinBox->setRange(10000, 30000);
	iceStormPortSpinBox->setValue(info.iceStormPort);

	iceIpLabel = new QLabel(QStringLiteral("Ice服务IP:"));
	iceIpLabel->setBuddy(iceIpLineEdit);
	icePortLabel = new QLabel(QStringLiteral("Ice服务端口:"));
	icePortLabel->setBuddy(icePortSpinBox);
	iceOffsideIpLabel = new QLabel(QStringLiteral("对侧Ice服务IP:"));
	iceOffsideIpLabel->setBuddy(iceIpLineEdit);
	iceOffsidePortLabel = new QLabel(QStringLiteral("对侧Ice服务端口:"));
	iceOffsidePortLabel->setBuddy(icePortSpinBox);
	iceStormIpLabel = new QLabel(QStringLiteral("IceStorm服务IP:"));
	iceStormIpLabel->setBuddy(iceStormIpLineEdit);
	iceStormPortLabel = new QLabel(QStringLiteral("IceStorm服务端口:"));
	iceStormPortLabel->setBuddy(iceStormPortSpinBox);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	setWindowIcon(m_icon);
}

void ConfigIceDialog::createLayout()
{
	QGridLayout* layout = new QGridLayout;
	layout->addWidget(iceIpLabel, 0, 0);
	layout->addWidget(iceIpLineEdit, 0, 1);
	layout->addWidget(icePortLabel, 1, 0);
	layout->addWidget(icePortSpinBox, 1, 1);
	layout->addWidget(iceOffsideIpLabel, 2, 0);
	layout->addWidget(iceOffsideIpLineEdit, 2, 1);
	layout->addWidget(iceOffsidePortLabel, 3, 0);
	layout->addWidget(iceOffsidePortSpinBox, 3, 1);
	layout->addWidget(iceStormIpLabel, 4, 0);
	layout->addWidget(iceStormIpLineEdit, 4, 1);
	layout->addWidget(iceStormPortLabel, 5, 0);
	layout->addWidget(iceStormPortSpinBox, 5, 1);
	layout->addWidget(buttonBox, 6, 0, 1, 2);
	setLayout(layout);
}

void ConfigIceDialog::loadConfigInfo(ConfigIceInfo& info)
{
	ConfigXmlHandler xmlHandler;
	QMap<QString, QVector<QString> > dataValues;
	xmlHandler.readXml(dataValues);
	if (dataValues.contains("self-ice"))
	{
		QMap<QString, QVector<QString>>::Iterator iter = dataValues.equal_range("self-ice").first;
		QVector<QString> values = iter.value();
		info.iceIp = values.at(0);
		info.icePort = values.at(1).toInt();
	}
	if (dataValues.contains("off-ice"))
	{
		QMap<QString, QVector<QString>>::Iterator iter = dataValues.equal_range("off-ice").first;
		QVector<QString> values = iter.value();
		info.iceOffsideIp = values.at(0);
		info.iceOffsidePort = values.at(1).toInt();
	}
	if (dataValues.contains("ice-storm"))
	{
		QMap<QString, QVector<QString>>::Iterator iter = dataValues.equal_range("ice-storm").first;
		QVector<QString> values = iter.value();
		info.iceStormIp = values.at(0);
		info.iceStormPort = values.at(1).toInt();
	}
}

void ConfigIceDialog::saveConfigInfo()
{
	ConfigIceInfo info = getConfigIceInfo();
	QMap<QString, QVector<QString> > dataValues;
	QVector<QString> values;
	values << info.iceIp << QString("%1").arg(info.icePort);
	QString value = QString("%1").arg(info.icePort);
	dataValues.insert("self-ice", values);

	values.clear();
	values << info.iceOffsideIp << QString("%1").arg(info.iceOffsidePort);
	dataValues.insert("off-ice", values);

	values.clear();
	values << info.iceStormIp << QString("%1").arg(info.iceStormPort);
	dataValues.insert("ice-storm", values);

	ConfigXmlHandler xmlHandler;
	xmlHandler.writeXml(dataValues);
}

ConfigIceInfo ConfigIceDialog::getConfigIceInfo() const
{
	ConfigIceInfo info;
	info.iceIp = iceIpLineEdit->text();
	info.icePort = icePortSpinBox->value();
	info.iceOffsideIp = iceOffsideIpLineEdit->text();
	info.iceOffsidePort = iceOffsidePortSpinBox->value();
	info.iceStormIp = iceStormIpLineEdit->text();
	info.iceStormPort = iceStormPortSpinBox->value();
	return info;
}
