#-------------------------------------------------
#
# ET - Equivalence Tutor
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ET
TEMPLATE = app

VERSION = Alpha
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

greaterThan(QT_MAJOR_VERSION, 4) {
        CONFIG += c++11
} else {
        # Workaround for Qt4
        QMAKE_CXXFLAGS += -std=c++11
}

SOURCES += main.cpp\
    mainwindow.cpp \
    newsolutiondialog.cpp \
    welcomepagewidget.cpp \
    AST.cpp \
    solutiontabwidget.cpp \
    inputpanel.cpp \
    idtable.cpp \
    equivalenceengine.cpp \
    logicset.cpp \
    solutionmodel.cpp \
    subformulaselectiondialog.cpp \
    formulareplacementdialog.cpp \
    equivalenceutility.cpp \
    matchedruleselectiondialog.cpp \
    ruleengine.cpp \
    replacementinputdialog.cpp

HEADERS  += mainwindow.hpp \
    newsolutiondialog.hpp \
    welcomepagewidget.hpp \
    AST.hpp \
    symbol.hpp \
    solutiontabwidget.hpp \
    inputpanel.hpp \
    idtable.hpp \
    equivalenceengine.hpp \
    logicset.hpp \
    solutionmodel.hpp \
    subformulaselectiondialog.hpp \
    formulareplacementdialog.hpp \
    main.hpp \
    embeddedpushbutton.hpp \
    equivalenceutility.hpp \
    matchedruleselectiondialog.hpp \
    utility.hpp \
    ruleengine.hpp \
    replacementinputdialog.hpp

FORMS    += mainwindow.ui \
    newsolutiondialog.ui \
    welcomepagewidget.ui \
    solutiontabwidget.ui \
    inputpanel.ui \
    subformulaselectiondialog.ui \
    matchedruleselectiondialog.ui \
    replacementinputdialog.ui \
    formulareplacementdialog.ui

LIBS += -ll -ly

FLEXSOURCES = lexer.l
BISONSOURCES = parser.y
EQUIVALENCESOURCES += equivalences.txt \
                      equivalences.xml

OTHER_FILES +=  \
    $$FLEXSOURCES \
    $$BISONSOURCES \
    $$EQUIVALENCESOURCES

flexsource.input = FLEXSOURCES
flexsource.output = ${QMAKE_FILE_BASE}.cpp
flexsource.commands = flex --header-file=${QMAKE_FILE_BASE}.hpp -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
flexsource.variable_out = SOURCES
flexsource.name = Flex Sources ${QMAKE_FILE_IN}
flexsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += flexsource

flexheader.input = FLEXSOURCES
flexheader.output = ${QMAKE_FILE_BASE}.hpp
flexheader.commands = @true
flexheader.variable_out = HEADERS
flexheader.name = Flex Headers ${QMAKE_FILE_IN}
flexheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += flexheader

bisonsource.input = BISONSOURCES
bisonsource.output = ${QMAKE_FILE_BASE}.cpp
bisonsource.commands = bison -d --defines=${QMAKE_FILE_BASE}.hpp -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
bisonsource.variable_out = SOURCES
bisonsource.name = Bison Sources ${QMAKE_FILE_IN}
bisonsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += bisonsource

bisonheader.input = BISONSOURCES
bisonheader.output = ${QMAKE_FILE_BASE}.hpp
bisonheader.commands = @true
bisonheader.variable_out = HEADERS
bisonheader.name = Bison Headers ${QMAKE_FILE_IN}
bisonheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += bisonheader

RESOURCES += \
    ET.qrc
