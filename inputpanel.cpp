#include "inputpanel.hpp"

InputPanel::InputPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::InputPanelForm)
{
	ui->setupUi(this);

	signalMapper.setMapping(ui->panelButton_and, ui->panelButton_and);
	signalMapper.setMapping(ui->panelButton_or, ui->panelButton_or);
	signalMapper.setMapping(ui->panelButton_not, ui->panelButton_not);
	signalMapper.setMapping(ui->panelButton_truth, ui->panelButton_truth);
	signalMapper.setMapping(ui->panelButton_falsity, ui->panelButton_falsity);
	signalMapper.setMapping(ui->panelButton_implies, ui->panelButton_implies);
	signalMapper.setMapping(ui->panelButton_equiv, ui->panelButton_equiv);
	signalMapper.setMapping(ui->panelButton_forall, ui->panelButton_forall);
	signalMapper.setMapping(ui->panelButton_exist, ui->panelButton_exist);
	signalMapper.setMapping(ui->panelButton_equal, ui->panelButton_equal);

	connect(ui->panelButton_and, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(ui->panelButton_or, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(ui->panelButton_not, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(ui->panelButton_truth, SIGNAL(clicked()), &signalMapper,
	        SLOT(map()));
	connect(ui->panelButton_falsity, SIGNAL(clicked()), &signalMapper,
	        SLOT(map()));
	connect(ui->panelButton_implies, SIGNAL(clicked()), &signalMapper,
	        SLOT(map()));
	connect(ui->panelButton_equiv, SIGNAL(clicked()), &signalMapper,
	        SLOT(map()));
	connect(ui->panelButton_forall, SIGNAL(clicked()), &signalMapper,
	        SLOT(map()));
	connect(ui->panelButton_exist, SIGNAL(clicked()), &signalMapper,
	        SLOT(map()));
	connect(ui->panelButton_equal, SIGNAL(clicked()), &signalMapper,
	        SLOT(map()));

	connect(&signalMapper, SIGNAL(mapped(QWidget *)), this,
	        SLOT(buttonClicked(QWidget *)));
}

InputPanel::~InputPanel()
{
	delete ui;
}

void InputPanel::buttonClicked(QWidget *w)
{
	QString str = qvariant_cast<QString>(w->property("buttonValue"));
	emit stringGenerated(str);
}
