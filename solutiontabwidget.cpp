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

SolutionTabWidget::SolutionTabWidget(QFile *f, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolutionTabWidget),
	model(new solutionModel(f)) {
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

	bool proofFinished = model->proofFinished();

	QVectorIterator<AST::LogicStatement*> forwardStackIt(model->forwardStack);
	QVectorIterator<AST::LogicStatement*> backwardStackIt(model->backwardStack);
	backwardStackIt.toBack();

	if(!proofFinished) {
		while(forwardStackIt.hasNext()) {
			ui->textEdit->insertPlainText(forwardStackIt.next()->print(ET::fullBracket).append("\n"));
		}
		ui->textEdit->insertPlainText("\n");
		while(backwardStackIt.hasPrevious()) {
			ui->textEdit->insertPlainText(backwardStackIt.previous()->print(ET::fullBracket).append("\n"));
		}
		connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(lineSelected()));
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
		selectedFormula = model->forwardStack.top();
		isForward = true;
	}
	else if (lineno == model->forwardStack.size() + 1) {
		selectedFormula = model->backwardStack.top();
		isForward = false;
	}
	else {
		// Blank line, do nothing and return
		return;
	}
	selectedFormula = selectedFormula->clone();
	SubformulaSelectionDialog* dialog = new SubformulaSelectionDialog(selectedFormula, this);
	connect(dialog, SIGNAL(subformulaSelected(AST::LogicStatement*)), this, SLOT(subformulaSelected(AST::LogicStatement*)));
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	dialog->show();
}

void SolutionTabWidget::subformulaSelected(AST::LogicStatement *subformula) {
	selectedSubformula = subformula;

	FormulaReplacementDialog *d = new FormulaReplacementDialog(subformula, this);
	connect(d, SIGNAL(ruleSelected(LogicSet*)), this, SLOT(ruleSelected(LogicSet*)));
	d->setAttribute(Qt::WA_DeleteOnClose);
	d->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	d->show();
}

void SolutionTabWidget::ruleSelected(LogicSet *ruleset) {
	QVector<Rule*> *m = ET::eqEng->getMatchedRules(selectedSubformula, ruleset);
	MatchedRuleSelectionDialog *d = new MatchedRuleSelectionDialog(m, ruleset, this);
	connect(d, SIGNAL(ruleSelected(Rule*,Rule*)), this, SLOT(matchedRuleSelected(Rule*,Rule*)));
	d->setAttribute(Qt::WA_DeleteOnClose);
	d->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	d->show();
}

void SolutionTabWidget::matchedRuleSelected(Rule *from, Rule *to) {
	newFormulaGenerated(ET::eqEng->replaceStatement(selectedSubformula, from, to, this));
}

void SolutionTabWidget::newFormulaGenerated(AST::LogicStatement *formula) {
	model->add(isForward, formula);
	redraw();
}

void SolutionTabWidget::undo() {
	model->undo();
	redraw();
}

void SolutionTabWidget::redo() {
	model->redo();
	redraw();
}

LogicStatement *SolutionTabWidget::getReplacement(const QString msg, const QString errorMessage) {
	//TODO: check if the errorMessage is Not equal to "";
	LogicStatement *userInput = nullptr;

	if (errorMessage != "") {
		//Display the error to user and request for the new LogicStatement with msg and return it
	} else {
		//Request for new LogicStatement with msg;
	}

	return userInput;
}
