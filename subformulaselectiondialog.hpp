#ifndef SUBFORMULASELECTIONDIALOG_HPP
#define SUBFORMULASELECTIONDIALOG_HPP

#include <QDialog>
#include <QAbstractButton>

#include "AST.hpp"

namespace Ui {
class SubformulaSelectionDialog;
}

class SubformulaSelectionDialog : public QDialog
{
	Q_OBJECT

	Ui::SubformulaSelectionDialog *ui;
	AST::LogicStatement* fullFormula;
	bool isForward;

public:
	explicit SubformulaSelectionDialog(AST::LogicStatement* formula, bool isForward, QWidget *parent = 0);
	~SubformulaSelectionDialog();

signals:
	void subformulaSelected(AST::LogicStatement*, bool isForward);

private slots:
	void onClick(QAbstractButton* btn);
};

#endif // SUBFORMULASELECTIONDIALOG_HPP