#ifndef AST_HPP
#define AST_HPP

namespace AST {
class Variable;
class LogicStatement;
}

#include <QString>
#include "symbol.hpp"
#include <QVector>
#include "idtable.hpp"

namespace AST {

class LogicStatement {
public:
	/* Returns a QString representation of AST */
	virtual QString print(bool fullBracket) = 0;

	/* Checks whether an AST contains first order logic */
	virtual bool isFirstOrderLogic() = 0;

	/* Returns the symbol representing a concrete class */
	virtual Symbol getSymbol() = 0;

	/* Returns a boolean value of the AST when variables
	 * gets assigned their boolean values */
	virtual bool evaluate() = 0;

	/* Used to check whether start and end formula is
	 * logically equivalent */
	bool isEquivalent(LogicStatement *);

	/* Iterates through the AST collecting references of
	 * variables, the inner vector represents a list with
	 * all variables having the same name */
	virtual void collectVariables(QVector<QVector<Variable *> *> *) = 0;

	/* Free up memory used to check start formula is
	 * equivalent to end one */
	void list_destroy(QVector<QVector<Variable *> *> *);

	/* Checks whether two AST are identical in structure
	 * and name */
	virtual bool equals(LogicStatement *) = 0;

	/* Called by rule AST to match the actual user AST
	 * called matching_statement */
	virtual bool match(LogicStatement *matching_statement, IDTable *table) = 0;

	/* Called from the replace rule, creates identical copies
	 * on heap with variables cloned using id_table */
	virtual LogicStatement* replace(IDTable *table) = 0;

	/* A function that deep clones a logicstatement */
	virtual LogicStatement* clone() = 0;

	/* Overriding == so that it can be used in standard container library */
	virtual inline bool operator==(LogicStatement &other) = 0;

protected:
	static inline bool higherPrecedence(LogicStatement *outer, LogicStatement* inner);
	static inline bool equalPrecedence(LogicStatement *outer, LogicStatement* inner);
};

class Truth : public LogicStatement {
public:
	QString print(bool);
	bool isFirstOrderLogic();
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matching_statement, IDTable *);
	LogicStatement* replace(IDTable *);
	LogicStatement* clone();
	inline bool operator==(LogicStatement &other);
};

class Falsity : public LogicStatement {
public:
	QString print(bool);
	bool isFirstOrderLogic();
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matching_statement, IDTable *);
	LogicStatement* replace(IDTable *);
	LogicStatement* clone();
	inline bool operator==(LogicStatement &other);
};

class Variable : public LogicStatement {
	QString name;
	bool value;

public:
	Variable(QString *name);
	QString print(bool);
	QString getName();
	void setName(QString *name);
	bool isFirstOrderLogic();
	Symbol getSymbol();

	/* Used to give variable a boolean value */
	void setBooleanValue(bool);
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matching_statement, IDTable *table);
	LogicStatement* replace(IDTable *table);
	LogicStatement* clone();
	inline bool operator==(LogicStatement &other);
};

class UnaryOpStatement : public LogicStatement {
	LogicStatement *nestedStatement;

public:
	virtual QString print(bool fullBracket) = 0;
	virtual bool isFirstOrderLogic() = 0;
	void setStatement(LogicStatement *);
	LogicStatement *getStatement();
	virtual Symbol getSymbol() = 0;
	bool evaluate() = 0;
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matching_statement, IDTable *table);
	LogicStatement* replace(IDTable *table);
	virtual LogicStatement* clone() = 0;
	inline bool operator==(LogicStatement &other);
};

class NotStatement : public UnaryOpStatement {
public:
	NotStatement(LogicStatement *);
	bool isFirstOrderLogic();
	QString print(bool fullBracket);
	Symbol getSymbol();
	bool evaluate();
	LogicStatement* clone();
};

class BinaryOpStatement : public LogicStatement {
	LogicStatement *leftStatement;
	LogicStatement *rightStatement;

public:
	virtual QString symbol() = 0;
	bool isFirstOrderLogic();
	QString print(bool fullBracket);
	void setLeftStatement(LogicStatement *);
	void setRightStatement(LogicStatement *);
	LogicStatement * getLeftStatement();
	LogicStatement * getRightStatement();
	Symbol getSymbol() = 0;
	bool evaluate() = 0;
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matching_statement, IDTable *table);
	LogicStatement* replace(IDTable *table);
	virtual LogicStatement* clone() = 0;
	inline bool operator==(LogicStatement &other);
};

class AndStatement : public BinaryOpStatement {
public:
	AndStatement(LogicStatement *, LogicStatement *);
	QString symbol();
	Symbol getSymbol();
	bool evaluate();
	LogicStatement* clone();
};

class OrStatement : public BinaryOpStatement {
public:
	OrStatement(LogicStatement *, LogicStatement *);
	QString symbol();
	Symbol getSymbol();
	bool evaluate();
	LogicStatement* clone();
};

class IffStatement : public BinaryOpStatement {
public:
	IffStatement(LogicStatement *, LogicStatement *);
	QString symbol();
	Symbol getSymbol();
	bool evaluate();
	LogicStatement* clone();
};

class ImpliesStatement : public BinaryOpStatement {
public:
	ImpliesStatement(LogicStatement *, LogicStatement *);
	QString symbol();
	Symbol getSymbol();
	bool evaluate();
	LogicStatement* clone();
};

class FirstOrderStatement : public LogicStatement {
public:
	virtual QString print(bool fullBrackets) = 0;
	bool isFirstOrderLogic();
	Symbol getSymbol() = 0;
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matching_statement, IDTable *table);
	LogicStatement* clone();
	LogicStatement* replace(IDTable *table);
	inline bool operator==(LogicStatement &other);
};

class ForAllStatement : public FirstOrderStatement {
	LogicStatement *statement;
	Variable *identifier;

public:
	ForAllStatement(Variable *,LogicStatement *);
	void setIdentifier(Variable *);
	QString print(bool fullBracket);
	LogicStatement * getStatement();
	void setStatement(LogicStatement *);
	Symbol getSymbol();
};

class ThereExistsStatement : public FirstOrderStatement {
	LogicStatement *statement;
	Variable *identifier;

public:
	ThereExistsStatement(Variable *,LogicStatement *);
	QString print(bool fullBracket);
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
	QString print(bool fullBracket);
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matching_statement, IDTable *table);
	LogicStatement* clone();
	LogicStatement* replace(IDTable *table);
	inline bool operator==(LogicStatement &other);
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
	QString print(bool fullBracket);
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
	QString print(bool fullBracket);
	Symbol getSymbol();
};

LogicStatement *parse(QString expression);
}

#endif // AST_HPP
