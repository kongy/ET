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
	connect(ui->actionFull_Brackets, SIGNAL(changed()), this, SLOT(bracketStatusChanged()));

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
	NewSolutionDialog *dialog = new NewSolutionDialog(this);
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
	QString aboutMsgStr;
	aboutMsgStr.append("<h1>" + QApplication::applicationName() + "</h1>");
	aboutMsgStr.append("<br>Version: " + QApplication::applicationVersion() + "</br>");
	aboutMsgStr.append("<br>Based on Qt " + QString(QT_VERSION_STR) + "</br>");
	QMessageBox::about(this, "About", aboutMsgStr);
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
	bool fullBracket = ui->actionFull_Brackets->isChecked();
	QWidget *tab = new SolutionTabWidget(start, end, fullBracket, ui->tabWidget);
	ui->tabWidget->addTab(tab, name);
	ui->tabWidget->setCurrentWidget(tab);
}

/** Undo the last operation */
void MainWindow::undo() {
	QWidget* curWidget = ui->tabWidget->currentWidget();
	if(typeid(*curWidget) == typeid(SolutionTabWidget)) {
		static_cast<SolutionTabWidget*> (curWidget)->undo();
	}
}

/** Redo the last operation */
void MainWindow::redo() {
	QWidget* curWidget = ui->tabWidget->currentWidget();
	if(typeid(*curWidget) == typeid(SolutionTabWidget)) {
		static_cast<SolutionTabWidget*> (curWidget)->redo();
	}
}

/** Change bracket status of all tabs */
void MainWindow::bracketStatusChanged() {
	bool fullBracket = ui->actionFull_Brackets->isChecked();
	for(int i = 0; i < ui->tabWidget->count(); i++) {
		QWidget* widget = ui->tabWidget->widget(i);
		if(typeid(*widget) == typeid(SolutionTabWidget)) {
			static_cast<SolutionTabWidget*> (widget)->changeBracketStatus(fullBracket);
		}
	}
	return;
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
