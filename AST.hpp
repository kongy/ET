#ifndef AST_HPP
#define AST_HPP

#include <QString>
#include "symbol.hpp"

namespace AST {

class LogicStatement {
public:
	virtual QString print() = 0;
	virtual bool isFirstOrderLogic() = 0;
    virtual Symbol getSymbol() = 0;
};

class Truth : public LogicStatement {
public:
	QString print();
	bool isFirstOrderLogic();
    Symbol getSymbol();
};

class Falsity : public LogicStatement {
public:
	QString print();
	bool isFirstOrderLogic();
    Symbol getSymbol();
};

class Variable : public LogicStatement {
	QString *name;

public:
	Variable(QString *name);
	QString print();
	QString getName();
	void setName(QString *name);
	bool isFirstOrderLogic();
    Symbol getSymbol();
};

class UnaryOpStatement : public LogicStatement {
	LogicStatement *nestedStatement;

public:
	virtual QString print() = 0;
	virtual bool isFirstOrderLogic() = 0;
	void setStatement(LogicStatement *);
	LogicStatement *getStatement();
    virtual Symbol getSymbol() = 0;
};

class NotStatement : public UnaryOpStatement {
public:
	NotStatement(LogicStatement *);
	bool isFirstOrderLogic();
	QString print();
    Symbol getSymbol();
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
    Symbol getSymbol() = 0;
};

class AndStatement : public BinaryOpStatement {
public:
	AndStatement(LogicStatement *, LogicStatement *);
	QString symbol();
    Symbol getSymbol();
};

class OrStatement : public BinaryOpStatement {
public:
	OrStatement(LogicStatement *, LogicStatement *);
	QString symbol();
    Symbol getSymbol();
};

class IffStatement : public BinaryOpStatement {
public:
	IffStatement(LogicStatement *, LogicStatement *);
	QString symbol();
    Symbol getSymbol();
};

class ImpliesStatement : public BinaryOpStatement {
public:
	ImpliesStatement(LogicStatement *, LogicStatement *);
	QString symbol();
    Symbol getSymbol();
};

class FirstOrderStatement : public LogicStatement {
public:
	virtual QString print() = 0;
	bool isFirstOrderLogic();
    Symbol getSymbol() = 0;
};

class ForAllStatement : public FirstOrderStatement {
	LogicStatement *statement;
	Variable *identifier;

public:
	ForAllStatement(Variable *,LogicStatement *);
	void setIdentifier(Variable *);
	QString print();
	LogicStatement * getStatement();
	void setStatement(LogicStatement *);
    Symbol getSymbol();
};

class ThereExistsStatement : public FirstOrderStatement {
	LogicStatement *statement;
	Variable *identifier;

public:
	ThereExistsStatement(Variable *,LogicStatement *);
	QString print();
	LogicStatement * getStatement();
	void setStatement(LogicStatement *);
	void setIdentifier(Variable *);
    Symbol getSymbol();
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
	bool isFirstOrderLogic();
	QString print();
    Symbol getSymbol();
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
    Symbol getSymbol();
};

class EqualityStatement : public FirstOrderStatement {
	Variable *leftVariable;
	Variable *rightVariable;

public:
	EqualityStatement(Variable *, Variable *);
	Variable * getLeftVariable();
	Variable * getRightVariable();
	void setLeftVariable(Variable *);
	void setRightVariable(Variable *);
	QString print();
    Symbol getSymbol();
};

LogicStatement *parse(QString expression);
}

#endif // AST_HPP
