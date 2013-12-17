#include "solutiontabwidget.hpp"
#include "ui_solutiontabwidget.h"

SolutionTabWidget::SolutionTabWidget(AST::LogicStatement *begin, AST::LogicStatement *end,
									 QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolutionTabWidget),
	model(new solutionModel(begin, end)){
	ui->setupUi(this);

	redraw();
}

SolutionTabWidget::~SolutionTabWidget() {
	delete ui;
}

void SolutionTabWidget::redraw() const {
	ui->textEdit->clear();
	QVectorIterator<AST::LogicStatement*> forwardStackIt(model->forwardStack);
	while(forwardStackIt.hasNext()) {
		ui->textEdit->insertPlainText(forwardStackIt.next()->print().append("\n"));
	}

	ui->textEdit->insertPlainText("\n");

	QVectorIterator<AST::LogicStatement*> backwardStackIt(model->backwardStack);
	backwardStackIt.toBack();
	while(backwardStackIt.hasPrevious()) {
		ui->textEdit->insertPlainText(backwardStackIt.previous()->print().append("\n"));
	}
}

void SolutionTabWidget::undo() {
	// TODO
}

void SolutionTabWidget::redo() {
	// TODO
}
