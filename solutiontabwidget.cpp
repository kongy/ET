#include "solutiontabwidget.hpp"
#include "ui_solutiontabwidget.h"

#include <QDebug>

SolutionTabWidget::SolutionTabWidget(AST::LogicStatement *begin, AST::LogicStatement *end,
									 QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolutionTabWidget)
{
	// TODO
	ui->setupUi(this);

	qDebug() << "Tab created with begin" << begin->print() << "end" << end->print();
}

SolutionTabWidget::~SolutionTabWidget()
{
	delete ui;
}
