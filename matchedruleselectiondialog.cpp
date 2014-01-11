#include "logicset.hpp"
#include "main.hpp"
#include "matchedruleselectiondialog.hpp"
#include "ui_matchedruleselectiondialog.h"

#include <embeddedpushbutton.hpp>
#include <QPushButton>

typedef EmbeddedPushButton<Rule *> RulePushButton;

MatchedRuleSelectionDialog::MatchedRuleSelectionDialog(QVector<Rule *> *rules, LogicSet *ls, QWidget *parent) :
    QDialog(parent),
	ui(new Ui::MatchedRuleSelectionDialog),
	ls(ls)
{
	ui->setupUi(this);

	for(Rule *r : *rules) {
		QPushButton *w = new RulePushButton(r->print(ET::fullBracket), r, this);
		ui->fromLayout->addWidget(w);
		connect(w, SIGNAL(clicked()), this, SLOT(onFromChosen()));
	}
}

MatchedRuleSelectionDialog::~MatchedRuleSelectionDialog()
{
	delete ui;
}

void MatchedRuleSelectionDialog::onFromChosen() {
	RulePushButton *source = static_cast<RulePushButton*>(sender());
	from = source->getExtra();
	for(Rule *r : *ls->diff(from)->getSet()) {
		QPushButton *w = new RulePushButton(r->print(ET::fullBracket), r, this);
		ui->toLayout->addWidget(w);
		connect(w, SIGNAL(clicked()), this, SLOT(onToChosen()));
	}
}

void MatchedRuleSelectionDialog::onToChosen() {
	RulePushButton *source = static_cast<RulePushButton*>(sender());
	to = source->getExtra();
	emit ruleSelected(from, to);
	close();
}
