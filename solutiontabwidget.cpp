#include "main.hpp"
#include "matchedruleselectiondialog.hpp"
#include "solutiontabwidget.hpp"
#include "subformulaselectiondialog.hpp"
#include "formulareplacementdialog.hpp"
#include "ui_solutiontabwidget.h"

#include <QDebug>

SolutionTabWidget::SolutionTabWidget(AST::LogicStatement *begin, AST::LogicStatement *end,
									 QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolutionTabWidget),
	model(new solutionModel(begin, end)) {
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

	bool proofFinished = model->forwardStack.top()->equals(model->backwardStack.top());

	QVectorIterator<AST::LogicStatement*> forwardStackIt(model->forwardStack);
	QVectorIterator<AST::LogicStatement*> backwardStackIt(model->backwardStack);
	backwardStackIt.toBack();

	if(!proofFinished) {
		while(forwardStackIt.hasNext()) {
			while(forwardStackIt.hasNext()) {
				ui->textEdit->insertPlainText(forwardStackIt.next()->print(ET::fullBracket).append("\n"));
			}
			ui->textEdit->insertPlainText("\n");
			while(backwardStackIt.hasPrevious()) {
				ui->textEdit->insertPlainText(backwardStackIt.previous()->print(ET::fullBracket).append("\n"));
			}
			connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(lineSelected()));
		}
	}
	else {
		while(forwardStackIt.hasNext()) {
			ui->textEdit->insertPlainText(forwardStackIt.next()->print(ET::fullBracket).append("\n"));
		}
		backwardStackIt.previous();
		while(backwardStackIt.hasPrevious()) {
			ui->textEdit->insertPlainText(backwardStackIt.previous()->print(ET::fullBracket).append("\n"));
		}
	}
}

void SolutionTabWidget::lineSelected() {
	QTextCursor c = ui->textEdit->textCursor();

	int lineno = c.blockNumber();
	if(lineno == model->forwardStack.size() - 1) {
		selectedStatement = model->forwardStack.top();
		isForward = true;
	}
	else if (lineno == model->forwardStack.size() + 1) {
		selectedStatement = model->backwardStack.top();
		isForward = false;
	}
	else {
		// Blank line, do nothing and return
		return;
	}
	SubformulaSelectionDialog* dialog = new SubformulaSelectionDialog(selectedStatement, this);
	connect(dialog, SIGNAL(subformulaSelected(AST::LogicStatement*)), this, SLOT(subformulaSelected(AST::LogicStatement*)));
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	dialog->show();
}

void SolutionTabWidget::subformulaSelected(AST::LogicStatement *subformula) {
	FormulaReplacementDialog *d = new FormulaReplacementDialog(subformula, this);
	connect(d, SIGNAL(ruleSelected(LogicSet*)), this, SLOT(ruleSelected(LogicSet*)));
	d->setAttribute(Qt::WA_DeleteOnClose);
	d->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	d->show();
}

void SolutionTabWidget::ruleSelected(LogicSet *ruleset) {
	QVector<Rule*> *m = ET::eqEng->getMatchedRules(selectedStatement, ruleset);
	if(m->size() == 1) {
		// Only one match, go ahead an apply it
		matchedRuleSelected(m->at(0));
	} else {
		MatchedRuleSelectionDialog *d = new MatchedRuleSelectionDialog(m, this);
		connect(d, SIGNAL(ruleSelected(Rule*)), this, SLOT(matchedRuleSelected(Rule*)));
		d->setAttribute(Qt::WA_DeleteOnClose);
		d->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
		d->show();
	}
}

void SolutionTabWidget::matchedRuleSelected(Rule *rule) {
	qDebug() << rule->print(ET::fullBracket);
}

void SolutionTabWidget::newFormulaGenerated(AST::LogicStatement *formula) {
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
