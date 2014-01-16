#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QFile>
#include <QMainWindow>

#include "AST.hpp"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

  private:
	Ui::MainWindow *ui;
	void createSolutionTab(QFile *f);
	void saveSolutionTab(QFile *f);

  private
slots:
	void startNewSolutionDialog();
	void startHelpDialog();
	void startAboutDialog();
	void createSolutionTab(AST::LogicStatement *start, AST::LogicStatement *end,
	                       QString name);
	void undo();
	void redo();
	void bracketStatusChanged();
	void closeTab(int index);
	void closeTab();
	void openFromFile();
	void saveToFile();
	void saveRule();
};

#endif // MAINWINDOW_HPP
