#ifndef AST_HPP
#define AST_HPP

#include <QString>

namespace AST {

using namespace std;

class LogicStatement {
public:
	virtual QString print() = 0;
	virtual bool isFirstOrderLogic() = 0;
};

class Truth : public LogicStatement {
public:
	QString print();
	bool isFirstOrderLogic();
};

class Falsity : public LogicStatement {
public:
	QString print();
	bool isFirstOrderLogic();
};

class Variable : public LogicStatement {
	QString *name;

public:
	Variable(QString *name);
	QString print();
	QString getName();
	void setName(QString *name);
	bool isFirstOrderLogic();
};

class UnaryOpStatement : public LogicStatement {
	LogicStatement *nestedStatement;

public:
	virtual QString print() = 0;
	virtual bool isFirstOrderLogic() = 0;
	void setStatement(LogicStatement *);
	LogicStatement *getStatement();
};

class NotStatement : public UnaryOpStatement {
public:
	NotStatement(LogicStatement *);
	bool isFirstOrderLogic();
	QString print();
};

class BinaryOpStatement : public LogicStatement {
	LogicStatement *leftStatement;
	LogicStatement *rightStatement;

public:
	virtual QString symbol() = 0;
	bool isFirstOrderLogic();
	QString print();
	void setLeftStatement(LogicStatement *);
	void setRightStatement(LogicStatement *);
	LogicStatement * getLeftStatement();
	LogicStatement * getRightStatement();
};

class AndStatement : public BinaryOpStatement {
public:
	AndStatement(LogicStatement *, LogicStatement *);
	QString symbol();
};

class OrStatement : public BinaryOpStatement {
public:
	OrStatement(LogicStatement *, LogicStatement *);
	QString symbol();
};

class IffStatement : public BinaryOpStatement {
public:
	IffStatement(LogicStatement *, LogicStatement *);
	QString symbol();
};

class ImpliesStatement : public BinaryOpStatement {
public:
	ImpliesStatement(LogicStatement *, LogicStatement *);
	QString symbol();
};

class FirstOrderStatement : public LogicStatement {
public:
	virtual QString print() = 0;
	bool isFirstOrderLogic();
};

class ForAllStatement : public FirstOrderStatement {
	LogicStatement *statement;

public:
	ForAllStatement(LogicStatement *);
	QString print();
	LogicStatement * getStatement();
	void setStatement(LogicStatement *);
};

class ThereExistsStatement : public FirstOrderStatement {
	LogicStatement *statement;

public:
	ThereExistsStatement(LogicStatement *);
	QString print();
	LogicStatement * getStatement();
	void setStatement(LogicStatement *);
};

class Parameters : public LogicStatement {
	Variable *parameter;
	Parameters *rest;

public:
	Parameters(Variable *, Parameters *);
	Variable * getParameter();
	void setParameter(Variable *);
	Parameters * getRemainingParameters();
	void setRemainingParameters(Parameters *);
	QString print();
};

class PredicateSymbolStatement : public FirstOrderStatement {
	Variable *predicateSymbol;
	Parameters *parameters;

public:
	PredicateSymbolStatement(Variable *, Parameters *);
	QString getPredicateSymbolName();
	Variable * getPredicateSymbol();
	void setPredicateSymbol(Variable *);
	Parameters * getParameters();
	void setParameters(Parameters *);
	QString print();
};

class EqualityStatement : FirstOrderStatement {
	Variable *leftVariable;
	Variable *rightVariable;

public:
	EqualityStatement(Variable *, Variable *);
	Variable * getLeftVariable();
	Variable * getRightVariable();
	void setLeftVariable(Variable *);
	void setRightVariable(Variable *);
	QString print();
};

}

#endif // AST_HPP
