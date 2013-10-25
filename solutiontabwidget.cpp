#include "solutiontabwidget.hpp"
#include "ui_solutiontabwidget.h"

SolutionTabWidget::SolutionTabWidget(AST::LogicStatement *begin, AST::LogicStatement *end,
									 QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolutionTabWidget)
{
	// TODO
	ui->setupUi(this);
}

SolutionTabWidget::~SolutionTabWidget()
{
	delete ui;
}
