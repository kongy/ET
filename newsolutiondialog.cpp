#include "newsolutiondialog.hpp"
#include "ui_newsolutiondialog.h"

#include <QDebug>

NewSolutionDialog::NewSolutionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewSolutionDialog) {
	ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onClick(QAbstractButton*)));
}

NewSolutionDialog::~NewSolutionDialog() {
	delete ui;
}

void NewSolutionDialog::onClick(QAbstractButton *btn) {
	switch(ui->buttonBox->standardButton(btn)) {
	case QDialogButtonBox::Ok:
		// TODO: parse input and generate AST
		emit accepted(nullptr, nullptr);
		break;
	case QDialogButtonBox::Cancel:
		// TODO
		break;
	case QDialogButtonBox::Reset:
		ui->startFormulaLineEdit->clear();
		ui->endFormulaLineEdit->clear();
		break;
	default:
		qDebug()<<"Unimplemented button clicked\n";
	}
}
