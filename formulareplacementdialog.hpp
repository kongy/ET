#ifndef FORMULAREPLACEMENTDIALOG_HPP
#define FORMULAREPLACEMENTDIALOG_HPP

#include "AST.hpp"
#include "equivalenceengine.hpp"
#include "logicset.hpp"

#include <QDialog>

namespace Ui {
class FormulaReplacementDialog;
}

class FormulaReplacementDialog : public QDialog
{
	Q_OBJECT

	Ui::FormulaReplacementDialog *ui;

public:
	explicit FormulaReplacementDialog(AST::LogicStatement* subformula, QWidget *parent = 0);
	~FormulaReplacementDialog();

signals:
	LogicSet *ruleSelected(LogicSet*);

private slots:
	void onClick();
};

#endif // FORMULAREPLACEMENTDIALOG_HPP
