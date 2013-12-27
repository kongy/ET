#include "subformulaselectiondialog.hpp"
#include "ui_subformulaselectiondialog.h"

#include <QDialogButtonBox>
#include <QDebug>

SubformulaSelectionDialog::SubformulaSelectionDialog(AST::LogicStatement *formula, QWidget *parent) :
    QDialog(parent),
	ui(new Ui::SubformulaSelectionDialog),
	fullFormula(formula)
{
	ui->setupUi(this);
	ui->lineEdit->setText(fullFormula->print(false));

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onClick(QAbstractButton*)));
}

SubformulaSelectionDialog::~SubformulaSelectionDialog()
{
	delete ui;
}

void SubformulaSelectionDialog::onClick(QAbstractButton *btn) {
	switch(ui->buttonBox->standardButton(btn)) {
	case QDialogButtonBox::Ok:
	{
		AST::LogicStatement* subformula = AST::parse(ui->lineEdit->text());
		emit subformulaSelected(subformula);
		return;
	}
	case QDialogButtonBox::Cancel:
		// Do nothing
		break;
	default:
		qWarning()<<"Unimplemented button clicked\n";
	}
}
