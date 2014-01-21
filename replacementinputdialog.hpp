#ifndef REPLACEMENTINPUTDIALOG_HPP
#define REPLACEMENTINPUTDIALOG_HPP

#include "AST.hpp"

#include <QDialog>

namespace Ui
{
class ReplacementInputDialog;
}

class ReplacementInputDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit ReplacementInputDialog(QString &reqMsg, QString &errMsg,
	                                QWidget *parent = nullptr);
	~ReplacementInputDialog();

	static QString getString(QString &reqMsg, QString &errMsg,
	                         QWidget *parent = nullptr);

  private:
	Ui::ReplacementInputDialog *ui;
	QString textValue();

  private
slots:
	void insertString(QString str);
};

#endif // REPLACEMENTINPUTDIALOG_HPP
