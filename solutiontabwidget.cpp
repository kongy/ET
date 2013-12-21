#include "solutiontabwidget.hpp"
#include "subformulaselectiondialog.hpp"
#include "ui_solutiontabwidget.h"

#include <QDebug>

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
	disconnect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(lineSelected()));

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

	connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(lineSelected()));
}

void SolutionTabWidget::lineSelected() {
	QTextCursor c = ui->textEdit->textCursor();

	int lineno = c.blockNumber();
	AST::LogicStatement* selectedStatement = nullptr;
	bool isForward = lineno < model->forwardStack.size();
	bool isBackward = lineno > model->forwardStack.size() && lineno < model->forwardStack.size() + model->forwardStack.size() + 1;
	if(isForward) {
		selectedStatement = model->forwardStack.at(lineno);
	}
	else if (isBackward) {
		lineno -= model->forwardStack.size();
		selectedStatement = model->backwardStack.at(lineno - 1);
	}
	else {
		// Blank line, do nothing and return
		return;
	}
	SubformulaSelectionDialog* dialog = new SubformulaSelectionDialog(selectedStatement, isForward, this);
	connect(dialog, SIGNAL(subformulaSelected(AST::LogicStatement*,bool)), this, SLOT(subformulaSelected(AST::LogicStatement*,bool)));
	dialog->show();
}

void SolutionTabWidget::subformulaSelected(AST::LogicStatement *formula, bool isForward) {
	// TODO
	qDebug() << "Subformula" << formula->print() << "Selected";
	if(isForward) {
		model->forwardStack.append(formula);
	}
	else {
		model->backwardStack.append(formula);
	}
	redraw();
}

void SolutionTabWidget::undo() {
	// TODO
	qDebug() << "Undo requested";
}

void SolutionTabWidget::redo() {
	// TODO
	qDebug() << "Redo requested";
}
