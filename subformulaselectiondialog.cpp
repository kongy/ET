#include "embeddedpushbutton.hpp"
#include "main.hpp"
#include "subformulaselectiondialog.hpp"
#include "ui_subformulaselectiondialog.h"

#include <QDebug>
#include <QLabel>
#include <QPair>

typedef EmbeddedPushButton<AST::LogicStatement *> SubformulaButton;

SubformulaSelectionDialog::SubformulaSelectionDialog(
    AST::LogicStatement *formula, QWidget *parent)
    : QDialog(parent), ui(new Ui::SubformulaSelectionDialog),
      fullFormula(formula)
{
	ui->setupUi(this);

	qDebug() << parent;

	for (QPair<QString, AST::LogicStatement *> i :
	     formula->getStringMapping(ET::fullBracket)) {
		QWidget *sfw;
		if (i.second != nullptr) {
			sfw = new SubformulaButton(i.first, i.second, this);
			sfw->setToolTip(i.second->print(ET::fullBracket));
			connect(sfw, SIGNAL(clicked()), this, SLOT(onClick()));
		} else {
			sfw = new QLabel(i.first, this);
		}
		ui->subformulaeLayout->addWidget(sfw);
	}
}

SubformulaSelectionDialog::~SubformulaSelectionDialog()
{
	delete ui;
}

void SubformulaSelectionDialog::onClick()
{
	SubformulaButton *sBtn = static_cast<SubformulaButton *>(sender());
	emit subformulaSelected(sBtn->getExtra());
	close();
}
