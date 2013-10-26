#include "newsolutiondialog.hpp"
#include "ui_newsolutiondialog.h"

#include <QDebug>

NewSolutionDialog::NewSolutionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewSolutionDialog) {
	ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onClick(QAbstractButton*)));
	connect(ui->inputPanel, SIGNAL(stringGenerated(QString)), this, SLOT(insertString(QString)));
}

NewSolutionDialog::~NewSolutionDialog() {
	delete ui;
}

void NewSolutionDialog::onClick(QAbstractButton *btn) {
	switch(ui->buttonBox->standardButton(btn)) {
	case QDialogButtonBox::Ok:
		// TODO: parse input and generate AST
		emit accepted(nullptr, nullptr, ui->nameLineEdit->text());
		break;
	case QDialogButtonBox::Cancel:
		// TODO
		break;
	case QDialogButtonBox::Reset:
		ui->startFormulaLineEdit->clear();
		ui->endFormulaLineEdit->clear();
		break;
	default:
		qWarning()<<"Unimplemented button clicked\n";
	}
}

void NewSolutionDialog::insertString(QString str) {
	QWidget *w = QWidget::focusWidget();
	if (w == ui->startFormulaLineEdit) {
		ui->startFormulaLineEdit->insert(str);
	}
	else if (w == ui->endFormulaLineEdit) {
		ui->endFormulaLineEdit->insert(str);
	}
}
