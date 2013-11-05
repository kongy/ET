#include "newsolutiondialog.hpp"
#include "ui_newsolutiondialog.h"

#include <QDebug>
#include <QMessageBox>

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
	{
		// Clear the warning
		ui->startFormulaLabel->setStyleSheet("");
		ui->endFormulaLabel->setStyleSheet("");


		AST::LogicStatement *begin = AST::parse(ui->startFormulaLineEdit->text());
		AST::LogicStatement *end = AST::parse(ui->endFormulaLineEdit->text());
		if(begin == nullptr) {
			QMessageBox msg;
			ui->startFormulaLabel->setStyleSheet("QLabel { color : red; }");
			msg.warning(this, "Parsing failure", "Failed to parse start formula");
			break;
		}
		if(end == nullptr) {
			QMessageBox msg;
			ui->endFormulaLabel->setStyleSheet("QLabel { color : red; }");
			msg.warning(this, "Parsing failure", "Failed to parse end formula");
			break;
		}
		if(!begin->isEquivalent(end)) {
			QMessageBox msg;
			ui->startFormulaLabel->setStyleSheet("QLabel { color : red; }");
			ui->endFormulaLabel->setStyleSheet("QLabel { color : red; }");
			msg.warning(this, "Not provable", "Start formula is not equivalent to end formula");
			break;
		}
		emit accepted(begin, end, ui->nameLineEdit->text());
		close();
		break;
	}
	case QDialogButtonBox::Cancel:
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

void NewSolutionDialog::accept() {
	// Do not close the dialog
	// Call close() manually if needed
}
