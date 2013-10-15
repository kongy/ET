#include "mainwindow.hpp"
#include "myinputpanelcontext.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	MyInputPanelContext *ic = new MyInputPanelContext;
	a.setInputContext(ic);

	MainWindow w;
	w.show();

	return a.exec();
}
