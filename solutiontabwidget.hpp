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

public:
	explicit SolutionTabWidget(AST::LogicStatement * begin, AST::LogicStatement *end,
							   QWidget *parent = 0);
	~SolutionTabWidget();

	void undo();
	void redo();

signals:
	void lineSelected(AST::LogicStatement* expr);

private:
	void redraw() const;

private slots:
	void lineSelected();
	void subformulaSelected(AST::LogicStatement* formula, bool isForward);
};

#endif // SOLUTIONTABWIDGET_HPP
