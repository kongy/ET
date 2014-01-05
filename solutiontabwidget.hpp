#ifndef SOLUTIONTABWIDGET_HPP
#define SOLUTIONTABWIDGET_HPP

#include <QWidget>
#include <QVector>

#include "AST.hpp"
#include "solutionmodel.hpp"
#include "logicset.hpp"

namespace Ui {
class SolutionTabWidget;
}

class SolutionTabWidget : public QWidget
{
	Q_OBJECT

	Ui::SolutionTabWidget *ui;
	solutionModel* model;

	/*
	 *  Can only select one statement for processing at a time,
	 *  save states globally so that no need to pass them all the time.
	 */
	bool isForward;
	AST::LogicStatement *selectedStatement;

public:
	explicit SolutionTabWidget(AST::LogicStatement * begin, AST::LogicStatement *end, QWidget *parent = 0);
	~SolutionTabWidget();

	void redraw() const;
	void undo();
	void redo();

signals:
	void lineSelected(AST::LogicStatement* expr);

private slots:
	void lineSelected();
	void subformulaSelected(AST::LogicStatement* subformula);
	void ruleSelected(LogicSet *ruleset);
	void newFormulaGenerated(AST::LogicStatement *formula);
};

#endif // SOLUTIONTABWIDGET_HPP
