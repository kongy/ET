#include "main.hpp"
#include "matchedruleselectiondialog.hpp"
#include "solutiontabwidget.hpp"
#include "subformulaselectiondialog.hpp"
#include "replacementinputdialog.hpp"
#include "formulareplacementdialog.hpp"
#include "ui_solutiontabwidget.h"

#include <QClipboard>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>

class SolutionTabWidget::FormulaWidgetItem : public QListWidgetItem
{
  public:
	LogicStatement *lstat;
	FormulaWidgetItem(const QString &text, LogicStatement *lstat,
	                  QListWidget *view = 0)
	    : QListWidgetItem(text, view), lstat(lstat)
	{
	}
};

SolutionTabWidget::SolutionTabWidget(AST::LogicStatement *begin,
                                     AST::LogicStatement *end, QWidget *parent)
    : QWidget(parent), ui(new Ui::SolutionTabWidget),
      model(new solutionModel(begin, end))
{
	ui->setupUi(this);

	ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
	        this, SLOT(ShowContextMenu(const QPoint &)));

	redraw();
}

SolutionTabWidget::SolutionTabWidget(QFile *f, QWidget *parent)
    : QWidget(parent), ui(new Ui::SolutionTabWidget),
      model(new solutionModel(f))
{
	ui->setupUi(this);

	ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this,
	        SLOT(ShowContextMenu(QPoint)));

	redraw();
}

SolutionTabWidget::~SolutionTabWidget()
{
	delete model;
	delete ui;
}

void SolutionTabWidget::redraw()
{
	disconnect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this,
	           SLOT(lineSelected(QListWidgetItem *)));
	ui->listWidget->clear();

	bool proofFinished = model->proofFinished();

	QVectorIterator<AST::LogicStatement *> forwardStackIt(model->forwardStack);
	QVectorIterator<AST::LogicStatement *> backwardStackIt(
	    model->backwardStack);
	backwardStackIt.toBack();

	if (!proofFinished) {
		while (forwardStackIt.hasNext()) {
			LogicStatement *lstat = forwardStackIt.next();
			FormulaWidgetItem *w = new FormulaWidgetItem(
			    lstat->print(ET::fullBracket).append("\n"), lstat,
			    ui->listWidget);
			ui->listWidget->addItem(w);
		}
		ui->listWidget->addItem("\n");
		while (backwardStackIt.hasPrevious()) {
			LogicStatement *lstat = backwardStackIt.previous();
			FormulaWidgetItem *w = new FormulaWidgetItem(
			    lstat->print(ET::fullBracket).append("\n"), lstat,
			    ui->listWidget);
			ui->listWidget->addItem(w);
		}
		connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this,
		        SLOT(lineSelected(QListWidgetItem *)));
	} else {
		while (forwardStackIt.hasNext()) {
			LogicStatement *lstat = forwardStackIt.next();
			FormulaWidgetItem *w = new FormulaWidgetItem(
				lstat->print(ET::fullBracket).append("\n"), lstat,
				ui->listWidget);
			ui->listWidget->addItem(w);
		}
		backwardStackIt.previous();
		while (backwardStackIt.hasPrevious()) {
			LogicStatement *lstat = backwardStackIt.previous();
			FormulaWidgetItem *w = new FormulaWidgetItem(
				lstat->print(ET::fullBracket).append("\n"), lstat,
				ui->listWidget);
			ui->listWidget->addItem(w);
		}
		QMessageBox msgBox(this);
		msgBox.setText("Proof Finished");
		msgBox.exec();
	}
}

void SolutionTabWidget::saveToFile(QFile *f)
{
	model->saveToFile(f);
}

void SolutionTabWidget::lineSelected(QListWidgetItem *item)
{
	int lineno = ui->listWidget->row(item);
	if (lineno == model->forwardStack.size() - 1) {
		selectedFormula = model->forwardStack.top();
		isForward = true;
	} else if (lineno == model->forwardStack.size() + 1) {
		selectedFormula = model->backwardStack.top();
		isForward = false;
	} else {
		// Blank line, do nothing and return
		return;
	}
	selectedFormula = selectedFormula->clone();
	SubformulaSelectionDialog *dialog =
	    new SubformulaSelectionDialog(selectedFormula, this);
	connect(dialog, SIGNAL(subformulaSelected(AST::LogicStatement *)), this,
	        SLOT(subformulaSelected(AST::LogicStatement *)));
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	dialog->show();
}

void SolutionTabWidget::subformulaSelected(AST::LogicStatement *subformula)
{
	selectedSubformula = subformula;

	FormulaReplacementDialog *d =
	    new FormulaReplacementDialog(subformula, this);
	connect(d, SIGNAL(ruleSelected(LogicSet *)), this,
	        SLOT(ruleSelected(LogicSet *)));
	d->setAttribute(Qt::WA_DeleteOnClose);
	d->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	d->show();
}

void SolutionTabWidget::ruleSelected(LogicSet *ruleset)
{
	QVector<Rule *> *m =
	    ET::eqEng->getMatchedRules(selectedSubformula, ruleset);
	MatchedRuleSelectionDialog *d =
	    new MatchedRuleSelectionDialog(m, ruleset, this);
	connect(d, SIGNAL(ruleSelected(Rule *, Rule *)), this,
	        SLOT(matchedRuleSelected(Rule *, Rule *)));
	d->setAttribute(Qt::WA_DeleteOnClose);
	d->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
	d->show();
}

void SolutionTabWidget::matchedRuleSelected(Rule *from, Rule *to)
{
	LogicStatement *newSubFormula =
	    ET::eqEng->replaceStatement(selectedSubformula, from, to, this);

	if (selectedSubformula == selectedFormula) {
		newFormulaGenerated(newSubFormula);
	} else {
		selectedFormula->replaceChildStatement(selectedSubformula,
		                                       newSubFormula);
		newFormulaGenerated(selectedFormula);
	}
}

void SolutionTabWidget::newFormulaGenerated(AST::LogicStatement *formula)
{
	model->add(isForward, formula);
	redraw();
}

void SolutionTabWidget::undo()
{
	model->undo();
	redraw();
}

void SolutionTabWidget::redo()
{
	model->redo();
	redraw();
}

LogicStatement *SolutionTabWidget::getReplacement(QString &prefixMessage,
                                                  Variable *suffix,
                                                  QString &errorMessage)
{
	QString reqMsg = prefixMessage + suffix->print(ET::fullBracket);

	LogicStatement *input = nullptr;
	while (input == nullptr) {
		QString inputStr =
		    ReplacementInputDialog::getString(reqMsg, errorMessage, this);
		input = AST::parse(inputStr);
	}

	return input;
}

void SolutionTabWidget::ShowContextMenu(const QPoint &point)
{
	QListWidgetItem *item = ui->listWidget->itemAt(point);
	FormulaWidgetItem *fitem = dynamic_cast<FormulaWidgetItem *>(item);
	if (!fitem) {
		// No item under point
		return;
	}

	QMenu menu;
	QPoint globalPos = ui->listWidget->viewport()->mapToGlobal(point);

	menu.addAction("Copy");
	menu.addAction("Remove");

	QAction *selectedItem = menu.exec(globalPos);

	if (selectedItem) {
		if (selectedItem->text() == "Copy") {
			QApplication::clipboard()->setText(
			    fitem->lstat->print(ET::fullBracket));
		} else if (selectedItem->text() == "Remove") {
			model->remove(fitem->lstat);
			redraw();
		}
	}
}
