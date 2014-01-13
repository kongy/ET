#include "embeddedpushbutton.hpp"
#include "equivalenceengine.hpp"
#include "main.hpp"
#include "formulareplacementdialog.hpp"
#include "ui_formulareplacementdialog.h"

#include <QDebug>
#include <QPushButton>

typedef EmbeddedPushButton<LogicSet *> RulePushButton;

FormulaReplacementDialog::FormulaReplacementDialog(
    AST::LogicStatement *subformula, QWidget *parent)
    : QDialog(parent), ui(new Ui::FormulaReplacementDialog)
{
	ui->setupUi(this);

	QVector<LogicSet *> *sl = ET::eqEng->match(subformula);
	for (LogicSet *l : *sl) {
		RulePushButton *w =
		    new RulePushButton(l->print(ET::fullBracket), l, this);
		w->setToolTip(l->getComment());
		ui->verticalLayout->addWidget(w);
		connect(w, SIGNAL(clicked()), this, SLOT(onClick()));
	}
}

FormulaReplacementDialog::~FormulaReplacementDialog()
{
	delete ui;
}

void FormulaReplacementDialog::onClick()
{
	RulePushButton *source = static_cast<RulePushButton *>(sender());
	emit ruleSelected(source->getExtra());
	close();
}
