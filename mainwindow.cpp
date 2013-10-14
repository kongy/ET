#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "newsolutiondialog.hpp"
#include "welcomepagewidget.hpp"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);

	// File menu items
	connect(ui->actionNewSolution, SIGNAL(triggered()), this, SLOT(startNewSolutionDialog()));
	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	// Edit menu items
	connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
	connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(redo()));

	// Help menu items
	connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(startHelpDialog()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(startAboutDialog()));

	// Add welcome page widget to tab widget
	ui->tabWidget->addTab(new WelcomePageWidget, "Welcome");
}

MainWindow::~MainWindow() {
	delete ui;
}

/** Start the new solution dialog */
void MainWindow::startNewSolutionDialog() {
	NewSolutionDialog *dialog = new NewSolutionDialog();
	dialog->show();
}

/** Opens help dialog */
void MainWindow::startHelpDialog() {
	// TODO
}

/** Opens about dialog */
void MainWindow::startAboutDialog() {
	QMessageBox::about(this, "Equivalence Tutor",
					   "<h2>Equivalence Tutor</h2><br>Based on Qt " QT_VERSION_STR);
}

/** Undo the last operation */
void MainWindow::undo() {
	// TODO
}

/** Redo the last operation */
void MainWindow::redo() {
	// TODO
}
