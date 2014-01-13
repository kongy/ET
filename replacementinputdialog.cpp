#include "replacementinputdialog.hpp"
#include "ui_replacementinputdialog.h"

ReplacementInputDialog::ReplacementInputDialog(QString &reqMsg, QString &errMsg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplacementInputDialog)
{
	ui->setupUi(this);

	ui->label->setText(reqMsg);
	ui->errorMsgLabel->setText(errMsg);
}

ReplacementInputDialog::~ReplacementInputDialog()
{
	delete ui;
}

QString ReplacementInputDialog::textValue() {
	return ui->lineEdit->text();
}

QString ReplacementInputDialog::getString(QString &reqMsg, QString &errMsg, QWidget *parent) {
	ReplacementInputDialog d(reqMsg, errMsg, parent);

	int ret = d.exec();
	if(ret) {
		return d.textValue();
	} else {
		return QString();
	}
}
