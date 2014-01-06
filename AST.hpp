#ifndef AST_HPP
#define AST_HPP

#include "symbol.hpp"
#include <QString>
#include <QVector>
#include <QPair>
#include "equivalenceutility.hpp"

class LogicSet;
class EquivalenceUtility;

namespace AST {

class Variable;

class LogicStatement {
	bool forwardRule;
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
	virtual bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) = 0;

	/* Called from the replace rule, creates identical copies
	 * on heap with variables cloned using id_table */
	virtual LogicStatement* replace() = 0;

	/* A function that deep clones a logicstatement */
	virtual LogicStatement* clone() = 0;

	/* Overriding == so that it can be used in standard container library */
	virtual bool operator==(LogicStatement &other) = 0;

	/* Main parent destructor */
	virtual ~LogicStatement();

	/* Mapping from broken parts of print to its corresponding LogicStatement */
	virtual QVector<QPair<QString, LogicStatement *> > getStringMapping(bool fullBracket) = 0;

	/* Returns true if the variable parameter is bounded, i.e. ForAll boundedVariable,
	 * ThereExists boundedVariable, or boundedVariable does not occur at all in the
	 * logicstatement */
	virtual bool variableBounded(Variable *boundedVariable) = 0;

	/* Adds variables that occur free to freeVariables, i.e. equals to freeVariable but not appear in quantifiers */
	virtual void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection) = 0;

	/* Forms a set of variables that are completely bounded, not free in any part of rootStatement */
	virtual void candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet) = 0;

	/* Used in the context of rule, infact for the Leibniz rule only */
	void setRuleForward(bool forwardRule);

	/* Used by rule, to determine whether a rule is only used forward for matching */
	bool isForwardRule();
protected:
	static inline int comparePrecedence(LogicStatement *outer, LogicStatement* inner);
};

class Truth : public LogicStatement {
public:
	QString print(bool);
	bool isFirstOrderLogic();
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *);
	LogicStatement* replace();
	LogicStatement* clone();
	bool operator==(LogicStatement &);
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool);
	bool variableBounded(Variable *);
	void collectFreeVariable(Variable *, QVector<Variable *> *);
	void candidateBoundVariables(LogicStatement *, LogicSet *);
};

class Falsity : public LogicStatement {
public:
	QString print(bool);
	bool isFirstOrderLogic();
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *);
	LogicStatement* replace();
	LogicStatement* clone();
	bool operator==(LogicStatement &);
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool);
	bool variableBounded(Variable *);
	void collectFreeVariable(Variable *, QVector<Variable *> *);
	void candidateBoundVariables(LogicStatement *, LogicSet *);
};

class Variable : public LogicStatement {
	QString name;
	bool value;
	/* Used by rule to indicate variable that binds to logicstatement */
	Variable *freeVariable;
	/* Used by rule to indicate variable that binds to logicstatement */
	Variable *boundedVariable;
protected:
	void setName(QString *);
public:
	Variable(QString *name);
	~Variable();
	QString print(bool);
	QString getName();
	bool isFirstOrderLogic();
	Symbol getSymbol();

	/* Used to give variable a boolean value */
	void setBooleanValue(bool);
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility);
	/* Called from cloned version of rule, delete this removes the cloned version which is no longer accessible
	 * because it gets replaced */
	LogicStatement* replace();
	LogicStatement* clone();
	bool operator==(LogicStatement &);
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection);
	void setFreeVariable(QString name);
	void setBoundedVariable(QString name);
	Variable *getFreeVariable();
	Variable *getBoundedVariable();
	void candidateBoundVariables(LogicStatement *, LogicSet *);
};

class UnaryOpStatement : public LogicStatement {
	LogicStatement *nestedStatement;
protected:
	void setStatement(LogicStatement *);
public:
	QString print(bool);
	virtual bool isFirstOrderLogic() = 0;
	LogicStatement *getStatement();
	virtual Symbol getSymbol() = 0;
	virtual QString symbol() = 0;
	bool evaluate() = 0;
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility);
	LogicStatement* replace();
	virtual LogicStatement* clone() = 0;
	bool operator==(LogicStatement &);
	virtual ~UnaryOpStatement();
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection);
	void candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet);
};

class NotStatement : public UnaryOpStatement {
public:
	NotStatement(LogicStatement *);
	bool isFirstOrderLogic();
	Symbol getSymbol();
	QString symbol();
	bool evaluate();
	LogicStatement* clone();
};

class BinaryOpStatement : public LogicStatement {
	LogicStatement *leftStatement;
	LogicStatement *rightStatement;
protected:
	void setLeftStatement(LogicStatement *);
	void setRightStatement(LogicStatement *);
public:
	virtual QString symbol() = 0;
	bool isFirstOrderLogic();
	QString print(bool);
	LogicStatement * getLeftStatement();
	LogicStatement * getRightStatement();
	Symbol getSymbol() = 0;
	bool evaluate() = 0;
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility);
	LogicStatement* replace();
	virtual LogicStatement* clone() = 0;
	bool operator==(LogicStatement &);
	virtual ~BinaryOpStatement();
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection);
	void candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet);
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
	virtual QString print(bool) = 0;
	bool isFirstOrderLogic();
	Symbol getSymbol() = 0;
	bool evaluate();
	virtual void collectVariables(QVector<QVector<Variable *> *> *) = 0;
	virtual bool equals(LogicStatement *) = 0;
	virtual bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility) = 0;
	virtual LogicStatement* clone() = 0;
	LogicStatement* replace();
	virtual bool operator==(LogicStatement &) = 0;
	virtual ~FirstOrderStatement();
	virtual QVector<QPair<QString, LogicStatement *> > getStringMapping(bool) = 0;
	virtual bool variableBounded(Variable *) = 0;
	virtual void collectFreeVariable(Variable *, QVector<Variable *> *) = 0;
};

class ForAllStatement : public FirstOrderStatement {
	LogicStatement *statement;
	Variable *identifier;
protected:
	void setStatement(LogicStatement *);
	void setIdentifier(Variable *);
public:
	ForAllStatement(Variable *,LogicStatement *);
	QString print(bool);
	LogicStatement * getStatement();
	Variable *getQuantifier();
	Symbol getSymbol();
	~ForAllStatement();
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection);
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	LogicStatement* clone();
	bool operator==(LogicStatement &);
	void candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility);
};

class ThereExistsStatement : public FirstOrderStatement {
	LogicStatement *statement;
	Variable *identifier;
protected:
	void setStatement(LogicStatement *);
	void setIdentifier(Variable *);
public:
	ThereExistsStatement(Variable *,LogicStatement *);
	QString print(bool);
	LogicStatement * getStatement();
	Variable *getQuantifier();
	Symbol getSymbol();
	~ThereExistsStatement();
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection);
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	LogicStatement* clone();
	bool operator==(LogicStatement &);
	void candidateBoundVariables(LogicStatement *rootStatement, LogicSet *boundSet);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility);
};

class Parameters : public LogicStatement {
	Variable *parameter;
	Parameters *rest;
protected:
	void setParameter(Variable *);
	void setRemainingParameters(Parameters *);
public:
	Parameters(Variable *, Parameters *);
	Variable * getParameter();
	Parameters * getRemainingParameters();
	bool isFirstOrderLogic();
	QString print(bool);
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility);
	LogicStatement* clone();
	LogicStatement* replace();
	bool operator==(LogicStatement &);
	~Parameters();
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection);
	void candidateBoundVariables(LogicStatement *, LogicSet *);
};

class PredicateSymbolStatement : public FirstOrderStatement {
	Variable *predicateSymbol;
	Parameters *parameters;
	void setParameters(Parameters *);
	void setPredicateSymbol(Variable *);
public:
	PredicateSymbolStatement(Variable *, Parameters *);
	QString getPredicateSymbolName();
	Variable * getPredicateSymbol();
	Parameters * getParameters();
	QString print(bool);
	Symbol getSymbol();
	~PredicateSymbolStatement();
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection);
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	LogicStatement* clone();
	bool operator==(LogicStatement &);
	void candidateBoundVariables(LogicStatement *, LogicSet *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility);
};

class EqualityStatement : public FirstOrderStatement {
	Variable *leftVariable;
	Variable *rightVariable;
	void setLeftVariable(Variable *);
	void setRightVariable(Variable *);
public:
	EqualityStatement(Variable *, Variable *);
	Variable * getLeftVariable();
	Variable * getRightVariable();
	QString print(bool fullBracket);
	Symbol getSymbol();
	~EqualityStatement();
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable, QVector<Variable *> *collection);
	void collectVariables(QVector<QVector<Variable *> *> *collection);
	bool equals(LogicStatement *);
	LogicStatement* clone();
	bool operator==(LogicStatement &);
	void candidateBoundVariables(LogicStatement *, LogicSet *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *matchingUtility);
};

LogicStatement *parse(QString expression);
}

#endif // AST_HPP
