#ifndef SOLUTIONTABWIDGET_HPP
#define SOLUTIONTABWIDGET_HPP

#include <QWidget>

#include "AST.hpp"

namespace Ui {
class SolutionTabWidget;
}

class SolutionTabWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SolutionTabWidget(AST::LogicStatement * begin, AST::LogicStatement *end,
							   QWidget *parent = 0);
	~SolutionTabWidget();

private:
	Ui::SolutionTabWidget *ui;
};

#endif // SOLUTIONTABWIDGET_HPP
