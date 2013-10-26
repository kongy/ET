#ifndef NEWSOLUTIONDIALOG_HPP
#define NEWSOLUTIONDIALOG_HPP

#include <QDialog>
#include <QAbstractButton>
#include <QString>

#include "AST.hpp"

namespace Ui {
class NewSolutionDialog;
}

class NewSolutionDialog : public QDialog {
	Q_OBJECT

public:
	explicit NewSolutionDialog(QWidget *parent = 0);
	~NewSolutionDialog();

signals:
	void accepted(AST::LogicStatement *start, AST::LogicStatement *end);

private:
	Ui::NewSolutionDialog *ui;

private slots:
	void onClick(QAbstractButton* btn);
	void insertString(QString str);
};

#endif // NEWSOLUTIONDIALOG_HPP
