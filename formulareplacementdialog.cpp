#include "formulareplacementdialog.hpp"
#include "ui_formulareplacementdialog.h"

#include <QDebug>
#include <QPushButton>

FormulaReplacementDialog::FormulaReplacementDialog(AST::LogicStatement *subformula, QWidget *parent) :
    QDialog(parent),
	ui(new Ui::FormulaReplacementDialog),
	eqEngine(new EquivalenceEngine)
{
	ui->setupUi(this);

	QVector<LogicSet*> *sl = eqEngine->match(subformula);
	for(LogicSet *l : *sl) {
		for(Rule *r : *(l->getSet())) {
			ui->verticalLayout->addWidget(new QPushButton(r->print(false), this));
		}
	}
}

FormulaReplacementDialog::~FormulaReplacementDialog()
{
	delete ui;
}
