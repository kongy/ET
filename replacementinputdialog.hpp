#ifndef REPLACEMENTINPUTDIALOG_HPP
#define REPLACEMENTINPUTDIALOG_HPP

#include "AST.hpp"

#include <QDialog>

namespace Ui {
class ReplacementInputDialog;
}

class ReplacementInputDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ReplacementInputDialog(QString &reqMsg, QString &errMsg, QWidget *parent = 0);
	~ReplacementInputDialog();

	static QString getString(QString &reqMsg, QString &errMsg, QWidget *parent = 0);

private:
	Ui::ReplacementInputDialog *ui;
	QString textValue();
};

#endif // REPLACEMENTINPUTDIALOG_HPP
