#include "inputpanel.hpp"

InputPanel::InputPanel(QWidget *parent) : QWidget(parent)
{
	form.setupUi(this);

	signalMapper.setMapping(form.panelButton_and, form.panelButton_and);
	signalMapper.setMapping(form.panelButton_or, form.panelButton_or);
	signalMapper.setMapping(form.panelButton_not, form.panelButton_not);
	signalMapper.setMapping(form.panelButton_truth, form.panelButton_truth);
	signalMapper.setMapping(form.panelButton_falsity, form.panelButton_falsity);
	signalMapper.setMapping(form.panelButton_implies, form.panelButton_implies);
	signalMapper.setMapping(form.panelButton_equiv, form.panelButton_equiv);
	signalMapper.setMapping(form.panelButton_forall, form.panelButton_forall);
	signalMapper.setMapping(form.panelButton_exist, form.panelButton_exist);
	signalMapper.setMapping(form.panelButton_equal, form.panelButton_equal);

	connect(form.panelButton_and, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_or, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_not, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_truth, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_falsity, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_implies, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_equiv, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_forall, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_exist, SIGNAL(clicked()), &signalMapper, SLOT(map()));
	connect(form.panelButton_equal, SIGNAL(clicked()), &signalMapper, SLOT(map()));

	connect(&signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(buttonClicked(QWidget*)));
}

void InputPanel::buttonClicked(QWidget *w)
{
	QString str = qvariant_cast<QString>(w->property("buttonValue"));
	emit stringGenerated(str);
}
