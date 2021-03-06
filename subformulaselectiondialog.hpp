#ifndef SUBFORMULASELECTIONDIALOG_HPP
#define SUBFORMULASELECTIONDIALOG_HPP

#include <QDialog>
#include <QAbstractButton>

#include "AST.hpp"

namespace Ui
{
class SubformulaSelectionDialog;
}

class SubformulaSelectionDialog : public QDialog
{
	Q_OBJECT

	Ui::SubformulaSelectionDialog *ui;
	AST::LogicStatement *fullFormula;

  public:
	explicit SubformulaSelectionDialog(AST::LogicStatement *formula,
	                                   QWidget *parent = nullptr);
	~SubformulaSelectionDialog();

signals:
	void subformulaSelected(AST::LogicStatement *);

  private
slots:
	void onClick();
};

#endif // SUBFORMULASELECTIONDIALOG_HPP
