#ifndef SOLUTIONTABWIDGET_HPP
#define SOLUTIONTABWIDGET_HPP

#include <QWidget>
#include <QVector>

#include "AST.hpp"
#include "solutionmodel.hpp"

namespace Ui {
class SolutionTabWidget;
}

class SolutionTabWidget : public QWidget
{
	Q_OBJECT

	Ui::SolutionTabWidget *ui;
	solutionModel* model;
	bool fullBracket;

	/*
	 *  Can only select one statement for processing at a time,
	 *  save states globally so that no need to pass them all the time.
	 */
	bool isForward;
	AST::LogicStatement *selectedStatement;

public:
	explicit SolutionTabWidget(AST::LogicStatement * begin, AST::LogicStatement *end,
							   bool fullBracket = false, QWidget *parent = 0);
	~SolutionTabWidget();

	void changeBracketStatus(bool fullBracket);
	void undo();
	void redo();

signals:
	void lineSelected(AST::LogicStatement* expr);

private:
	void redraw() const;

private slots:
	void lineSelected();
	void subformulaSelected(AST::LogicStatement* subformula);
	void newFormulaGenerated(AST::LogicStatement *formula);
};

#endif // SOLUTIONTABWIDGET_HPP
