#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "AST.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
private slots:
	void startNewSolutionDialog();
	void startHelpDialog();
	void startAboutDialog();
	void createSolutionTab(AST::LogicStatement *start, AST::LogicStatement *end, QString name);
	void undo();
	void redo();
	void closeTab(int index);
};

#endif // MAINWINDOW_HPP
