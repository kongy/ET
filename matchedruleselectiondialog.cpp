#include "logicset.hpp"
#include "main.hpp"
#include "matchedruleselectiondialog.hpp"
#include "ui_matchedruleselectiondialog.h"

#include <embeddedpushbutton.hpp>
#include <QPushButton>

typedef EmbeddedPushButton<Rule *> RulePushButton;

MatchedRuleSelectionDialog::MatchedRuleSelectionDialog(QVector<Rule *> *rules, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatchedRuleSelectionDialog)
{
	ui->setupUi(this);

	for(Rule *r : *rules) {
		QPushButton *w = new RulePushButton(r->print(ET::fullBracket), r, this);
		ui->verticalLayout->addWidget(w);
		connect(w, SIGNAL(clicked()), this, SLOT(onClick()));
	}
}

MatchedRuleSelectionDialog::~MatchedRuleSelectionDialog()
{
	delete ui;
}

void MatchedRuleSelectionDialog::onClick() {
	RulePushButton *source = static_cast<RulePushButton*>(sender());
	emit ruleSelected(source->getExtra());
	close();
}
