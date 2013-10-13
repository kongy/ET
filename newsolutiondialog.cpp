#include "newsolutiondialog.hpp"
#include "ui_newsolutiondialog.h"

NewSolutionDialog::NewSolutionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewSolutionDialog) {
	ui->setupUi(this);
}

NewSolutionDialog::~NewSolutionDialog() {
	delete ui;
}
