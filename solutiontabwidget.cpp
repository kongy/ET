#include "solutiontabwidget.hpp"
#include "ui_solutiontabwidget.h"

SolutionTabWidget::SolutionTabWidget(AST::LogicStatement *begin, AST::LogicStatement *end,
									 QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolutionTabWidget)
{
	// TODO
	ui->setupUi(this);

	ui->textEdit->insertPlainText(begin->print() + "  (Begin)\n");
	ui->textEdit->insertPlainText("\n");
	ui->textEdit->insertPlainText(end->print() + "  (End)\n");
}

SolutionTabWidget::~SolutionTabWidget()
{
	delete ui;
}
