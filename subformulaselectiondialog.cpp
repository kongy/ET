#include "subformulaselectiondialog.hpp"
#include "ui_subformulaselectiondialog.h"

#include <QDialogButtonBox>
#include <QDebug>

SubformulaSelectionDialog::SubformulaSelectionDialog(AST::LogicStatement *formula, bool isForward, QWidget *parent) :
    QDialog(parent),
	ui(new Ui::SubformulaSelectionDialog),
	fullFormula(formula),
	isForward(isForward)
{
	ui->setupUi(this);
	ui->lineEdit->setText(fullFormula->print());

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
		emit subformulaSelected(subformula, isForward);
		return;
	}
	case QDialogButtonBox::Cancel:
		// Do nothing
		break;
	default:
		qWarning()<<"Unimplemented button clicked\n";
	}
}