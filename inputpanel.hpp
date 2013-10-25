#ifndef INPUTPANEL_HPP
#define INPUTPANEL_HPP

#include <QWidget>
#include <QSignalMapper>

#include "ui_inputpanel.h"


class InputPanel : public QWidget
{
	Q_OBJECT

public:
	InputPanel(QWidget *parent = 0);

signals:
	void stringGenerated(QString str);

private slots:
	void buttonClicked(QWidget *w);

private:
	Ui::InputPanelForm form;
	QSignalMapper signalMapper;
};

#endif // INPUTPANEL_HPP
