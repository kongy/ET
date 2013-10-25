#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "newsolutiondialog.hpp"
#include "solutiontabwidget.hpp"
#include "welcomepagewidget.hpp"

#include <QDebug>
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
	ui->tabWidget->addTab(new WelcomePageWidget(ui->tabWidget), "Welcome");
}

MainWindow::~MainWindow() {
	delete ui;
}

/** Start the new solution dialog */
void MainWindow::startNewSolutionDialog() {
	NewSolutionDialog *dialog = new NewSolutionDialog();
	connect(dialog, SIGNAL(accepted(AST::LogicStatement*,AST::LogicStatement*)), this,
			SLOT(createSolutionTab(AST::LogicStatement*,AST::LogicStatement*)));
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

/** Opens new solution tab with given formulae
 *  iff start and end are equivalent given they are not first-order
 *  \param start Well-formed start formula AST
 *  \param end Well-formed end formula AST
 */
void MainWindow::createSolutionTab(AST::LogicStatement *start, AST::LogicStatement *end) {
	// TODO: Check AST equivalence
	qDebug()<<"createSolutionTab called with AST"<<start<<end;
	ui->tabWidget->addTab(new SolutionTabWidget(start, end, ui->tabWidget), "Solution");
}

/** Undo the last operation */
void MainWindow::undo() {
	// TODO
}

/** Redo the last operation */
void MainWindow::redo() {
	// TODO
}
