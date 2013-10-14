#ifndef AST_HPP
#define AST_HPP

#include <string>

namespace AST {

using namespace std;

class LogicStatement {
public:
	virtual string print() = 0;
	virtual bool isFirstOrderLogic() = 0;
};

class Truth : public LogicStatement {
public:
	string print();
	bool isFirstOrderLogic();
};

class Falsity : public LogicStatement {
public:
	string print();
	bool isFirstOrderLogic();
};

class Variable : public LogicStatement {
	string *name;

public:
	Variable(string *name);
	string print();
	string getName();
	void setName(string *name);
	bool isFirstOrderLogic();
};

class UnaryOpStatement : public LogicStatement {
	LogicStatement *nestedStatement;

public:
	virtual string print() = 0;
	virtual bool isFirstOrderLogic() = 0;
	void setStatement(LogicStatement *);
	LogicStatement *getStatement();
};

class NotStatement : public UnaryOpStatement {
public:
	NotStatement(LogicStatement *);
	bool isFirstOrderLogic();
	string print();
};

class BinaryOpStatement : public LogicStatement {
	LogicStatement *leftStatement;
	LogicStatement *rightStatement;

public:
	virtual string symbol() = 0;
	bool isFirstOrderLogic();
	string print();
	void setLeftStatement(LogicStatement *);
	void setRightStatement(LogicStatement *);
	LogicStatement * getLeftStatement();
	LogicStatement * getRightStatement();
};

class AndStatement : public BinaryOpStatement {
public:
	AndStatement(LogicStatement *, LogicStatement *);
	string symbol();
};

class OrStatement : public BinaryOpStatement {
public:
	OrStatement(LogicStatement *, LogicStatement *);
	string symbol();
};

class IffStatement : public BinaryOpStatement {
public:
	IffStatement(LogicStatement *, LogicStatement *);
	string symbol();
};

class ImpliesStatement : public BinaryOpStatement {
public:
	ImpliesStatement(LogicStatement *, LogicStatement *);
	string symbol();
};

class FirstOrderStatement : public LogicStatement {
public:
	virtual string print() = 0;
	bool isFirstOrderLogic();
};

class ForAllStatement : public FirstOrderStatement {
	LogicStatement *statement;

public:
	ForAllStatement(LogicStatement *);
	string print();
	LogicStatement * getStatement();
	void setStatement(LogicStatement *);
};

class ThereExistsStatement : public FirstOrderStatement {
	LogicStatement *statement;

public:
	ThereExistsStatement(LogicStatement *);
	string print();
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
	string print();
};

class PredicateSymbolStatement : public FirstOrderStatement {
	Variable *predicateSymbol;
	Parameters *parameters;

public:
	PredicateSymbolStatement(Variable *, Parameters *);
	string getPredicateSymbolName();
	Variable * getPredicateSymbol();
	void setPredicateSymbol(Variable *);
	Parameters * getParameters();
	void setParameters(Parameters *);
	string print();
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
	string print();
};

}

#endif // AST_HPP
