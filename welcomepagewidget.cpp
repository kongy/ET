#include "welcomepagewidget.hpp"
#include "ui_welcomepagewidget.h"

WelcomePageWidget::WelcomePageWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WelcomePageWidget)
{
	ui->setupUi(this);
}

WelcomePageWidget::~WelcomePageWidget()
{
	delete ui;
}
