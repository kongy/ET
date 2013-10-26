#include "welcomepagewidget.hpp"
#include "ui_welcomepagewidget.h"

WelcomePageWidget::WelcomePageWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WelcomePageWidget)
{
	ui->setupUi(this);

	connect(ui->startSolutionButton, SIGNAL(clicked()), this, SLOT(startNewSolutionSlot()));
}

WelcomePageWidget::~WelcomePageWidget()
{
	delete ui;
}

void WelcomePageWidget::startNewSolutionSlot() {
	emit startNewSolution();
}
