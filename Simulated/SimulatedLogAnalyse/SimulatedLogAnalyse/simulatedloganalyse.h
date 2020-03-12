#ifndef SIMULATEDLOGANALYSE_H
#define SIMULATEDLOGANALYSE_H

#include <QtWidgets/QMainWindow>

class QAction;
class QTextEdit;
class CAnalyseFileThread;

class SimulatedLogAnalyse : public QMainWindow
{
	Q_OBJECT

public:
	SimulatedLogAnalyse(QWidget *parent = 0);
	~SimulatedLogAnalyse();

private:
	void initWidget();
	void createActions();

	void startAnalyseFile();
	void stopAnalyseFile();

	void outputInfo(const QString& info);

private slots:
	void doOpenFile();
	void doAnalyse();
	void doSetting();
	void doClear();
	void doAnalyseFinished();
	void doThreadOutput(const QString& info);

private:
	QAction* m_openFileAction;
	QAction* m_analyseAction;
	QAction* m_settingAction;
	QAction* m_clearAction;
	QAction* m_quitAction;

	QTextEdit*	m_textEdit;

	int m_timeSpan;
	QString m_fileName;
	bool m_isAnalysing;
	QSharedPointer<CAnalyseFileThread> m_analyseThread;
};

#endif // SIMULATEDLOGANALYSE_H
