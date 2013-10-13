#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

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
	void undo();
	void redo();
};

#endif // MAINWINDOW_HPP
