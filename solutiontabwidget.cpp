#include "solutiontabwidget.hpp"
#include "subformulaselectiondialog.hpp"
#include "ui_solutiontabwidget.h"

#include <QDebug>

SolutionTabWidget::SolutionTabWidget(AST::LogicStatement *begin, AST::LogicStatement *end,
									 bool fullBracket, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolutionTabWidget),
	model(new solutionModel(begin, end)),
	fullBracket(fullBracket) {
	ui->setupUi(this);

	redraw();
}

SolutionTabWidget::~SolutionTabWidget() {
	delete model;
	delete ui;
}

void SolutionTabWidget::redraw() const {
	disconnect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(lineSelected()));
	ui->textEdit->clear();

	QVectorIterator<AST::LogicStatement*> forwardStackIt(model->forwardStack);
	while(forwardStackIt.hasNext()) {
		ui->textEdit->insertPlainText(forwardStackIt.next()->print(fullBracket).append("\n"));
	}

	ui->textEdit->insertPlainText("\n");

	QVectorIterator<AST::LogicStatement*> backwardStackIt(model->backwardStack);
	backwardStackIt.toBack();
	while(backwardStackIt.hasPrevious()) {
		ui->textEdit->insertPlainText(backwardStackIt.previous()->print(fullBracket).append("\n"));
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
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
}

void SolutionTabWidget::subformulaSelected(AST::LogicStatement *formula, bool isForward) {
	// TODO
	if(isForward) {
		model->forwardStack.append(formula);
	}
	else {
		model->backwardStack.append(formula);
	}
	redraw();
}

void SolutionTabWidget::changeBracketStatus(bool fullBracket) {
	this->fullBracket = fullBracket;
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
