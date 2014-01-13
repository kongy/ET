#ifndef MATCHEDRULESELECTIONDIALOG_HPP
#define MATCHEDRULESELECTIONDIALOG_HPP

#include "logicset.hpp"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class MatchedRuleSelectionDialog;
}

class MatchedRuleSelectionDialog : public QDialog
{
	Q_OBJECT

	Ui::MatchedRuleSelectionDialog *ui;
	LogicSet *ls;
	Rule *from, *to;

public:
	explicit MatchedRuleSelectionDialog(QVector<Rule *> *rules,  LogicSet *ls, QWidget *parent = 0);
	~MatchedRuleSelectionDialog();

signals:
	void ruleSelected(Rule *from, Rule *to);

private slots:
	void onFromChosen();
	void onToChosen();
};

#endif // MATCHEDRULESELECTIONDIALOG_HPP
