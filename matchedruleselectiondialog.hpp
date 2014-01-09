#ifndef MATCHEDRULESELECTIONDIALOG_HPP
#define MATCHEDRULESELECTIONDIALOG_HPP

#include "logicset.hpp"

#include <QDialog>

namespace Ui {
class MatchedRuleSelectionDialog;
}

class MatchedRuleSelectionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MatchedRuleSelectionDialog(QVector<Rule *> *rules, QWidget *parent = 0);
	~MatchedRuleSelectionDialog();

signals:
	void ruleSelected(Rule *r);

private:
	Ui::MatchedRuleSelectionDialog *ui;

private slots:
	void onClick();
};

#endif // MATCHEDRULESELECTIONDIALOG_HPP
