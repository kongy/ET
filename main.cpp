#include "main.hpp"
#include "mainwindow.hpp"

#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

bool ET::fullBracket;
EquivalenceEngine *ET::eqEng;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationName("Equivalence Tutor");
	a.setApplicationVersion(APP_VERSION);

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
	                  QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);

	MainWindow w;
	w.show();

	return a.exec();
}
