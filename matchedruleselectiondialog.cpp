#include "embeddedpushbutton.hpp"
#include "logicset.hpp"
#include "main.hpp"
#include "matchedruleselectiondialog.hpp"
#include "ui_matchedruleselectiondialog.h"

#include <QDebug>
#include <QMessageBox>

typedef EmbeddedPushButton<Rule *> RulePushButton;

MatchedRuleSelectionDialog::MatchedRuleSelectionDialog(QVector<Rule *> *rules,
													   LogicSet *ls,
													   QWidget *parent)
	: QDialog(parent), ui(new Ui::MatchedRuleSelectionDialog), ls(ls),
	  from(nullptr), to(nullptr)
{
	ui->setupUi(this);

	for (Rule *r : *rules) {
		QPushButton *w = new RulePushButton(r->print(ET::fullBracket), r, this);
		w->setCheckable(true);
		w->setAutoExclusive(true);
		connect(w, SIGNAL(clicked()), this, SLOT(onFromChosen()));
		ui->fromVLayout->addWidget(w);
	}
	for (Rule *r : *ls->getSet()) {
		QPushButton *w = new RulePushButton(r->print(ET::fullBracket), r, this);
		w->setCheckable(true);
		w->setAutoExclusive(true);
		connect(w, SIGNAL(clicked()), this, SLOT(onToChosen()));
		ui->toVLayout->addWidget(w);
	}
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton *)), this,
			SLOT(onButtonBoxClicked(QAbstractButton *)));
}

MatchedRuleSelectionDialog::~MatchedRuleSelectionDialog()
{
	delete ui;
}

void MatchedRuleSelectionDialog::onFromChosen()
{
	RulePushButton *sBtn = static_cast<RulePushButton*>(sender());
	from = sBtn->getExtra();
}

void MatchedRuleSelectionDialog::onToChosen()
{
	RulePushButton *sBtn = static_cast<RulePushButton*>(sender());
	to = sBtn->getExtra();
}

void MatchedRuleSelectionDialog::onButtonBoxClicked(QAbstractButton *btn) {
	switch (ui->buttonBox->standardButton(btn)) {
	case QDialogButtonBox::Ok: {
		if(from == nullptr || to == nullptr) {
			QMessageBox msgBox(this);
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setText("Select rules first");
			msgBox.exec();
			return;
		} else if(from->equals(to)) {
			QMessageBox msgBox(this);
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.setText("From rule and to rule cannot be the same");
			msgBox.exec();
			return;
		} else {
			emit ruleSelected(from, to);
			close();
		}
		break;
	}
	default:
		qWarning() << "Unimplemented button clicked\n";
	}
}
