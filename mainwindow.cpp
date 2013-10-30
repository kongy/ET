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
	connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeTab()));
	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	// Edit menu items
	connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
	connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(redo()));

	// Help menu items
	connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(startHelpDialog()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(startAboutDialog()));

	// Set up tabWidget
	WelcomePageWidget *welcome = new WelcomePageWidget(ui->tabWidget);
	ui->tabWidget->addTab(welcome, "Welcome");
	connect(welcome, SIGNAL(startNewSolution()), this, SLOT(startNewSolutionDialog()));
	connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

MainWindow::~MainWindow() {
	delete ui;
}

/** Start the new solution dialog */
void MainWindow::startNewSolutionDialog() {
	NewSolutionDialog *dialog = new NewSolutionDialog();
	connect(dialog, SIGNAL(accepted(AST::LogicStatement*,AST::LogicStatement*, QString)), this,
			SLOT(createSolutionTab(AST::LogicStatement*,AST::LogicStatement*, QString)));
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

/**
 * @brief Opens new solution tab with given formulae
 *        iff start and end are equivalent given they are not first-order
 * @param start Well-formed start formula AST
 * @param end Well-formed end formula AST
 * @param name Name of the new solution
 */
void MainWindow::createSolutionTab(AST::LogicStatement *start, AST::LogicStatement *end,
								   QString name) {
	// TODO: Check AST equivalence
	if(start == nullptr) qWarning()<<"Begin is NULL";
	if(end == nullptr) qWarning()<<"End is NULL";
	QWidget *tab = new SolutionTabWidget(start, end, ui->tabWidget);
	ui->tabWidget->addTab(tab, name);
	ui->tabWidget->setCurrentWidget(tab);
}

/** Undo the last operation */
void MainWindow::undo() {
	// TODO
}

/** Redo the last operation */
void MainWindow::redo() {
	// TODO
}

/** Close the requested tab
 *  \param index index of the tab in tabwidget
 */
void MainWindow::closeTab(int index) {
	QWidget* tabItem = ui->tabWidget->widget(index);
	ui->tabWidget->removeTab(index);

	delete(tabItem);
}

/** Close current tab */
void MainWindow::closeTab() {
	QWidget* tabItem = ui->tabWidget->currentWidget();
	ui->tabWidget->removeTab(ui->tabWidget->currentIndex());

	delete(tabItem);
}
