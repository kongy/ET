#ifndef FORMULAREPLACEMENTDIALOG_HPP
#define FORMULAREPLACEMENTDIALOG_HPP

#include "AST.hpp"
#include "equivalenceengine.hpp"

#include <QDialog>

namespace Ui {
class FormulaReplacementDialog;
}

class FormulaReplacementDialog : public QDialog
{
	Q_OBJECT

	Ui::FormulaReplacementDialog *ui;
	EquivalenceEngine *eqEngine;

public:
	explicit FormulaReplacementDialog(AST::LogicStatement* subformula, QWidget *parent = 0);
	~FormulaReplacementDialog();
};

#endif // FORMULAREPLACEMENTDIALOG_HPP
