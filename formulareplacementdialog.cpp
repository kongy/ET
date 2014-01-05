#include "embeddedpushbutton.hpp"
#include "main.hpp"
#include "formulareplacementdialog.hpp"
#include "ui_formulareplacementdialog.h"

#include <QDebug>
#include <QPushButton>

typedef EmbeddedPushButton<LogicSet*> RulePushButton;

FormulaReplacementDialog::FormulaReplacementDialog(AST::LogicStatement *subformula, QWidget *parent) :
    QDialog(parent),
	ui(new Ui::FormulaReplacementDialog),
	eqEngine(new EquivalenceEngine)
{
	ui->setupUi(this);

	QVector<LogicSet*> *sl = eqEngine->match(subformula);
	for(LogicSet *l : *sl) {
		RulePushButton *w = new RulePushButton(l->print(ET::fullBracket), l, this);
		ui->verticalLayout->addWidget(w);
		connect(w, SIGNAL(clicked()), this, SLOT(onClick()));
	}
}

FormulaReplacementDialog::~FormulaReplacementDialog()
{
	delete ui;
}

void FormulaReplacementDialog::onClick() {
	RulePushButton *source = static_cast<RulePushButton*>(sender());
	qDebug() << source->getExtra()->print(ET::fullBracket);
}
