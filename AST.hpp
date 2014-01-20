#ifndef AST_HPP
#define AST_HPP

#include "symbol.hpp"
#include <QString>
#include <QVector>
#include <QPair>
#include <QXmlStreamWriter>
#include "equivalenceutility.hpp"

class LogicSet;

class EquivalenceUtility;
class IDTable;

namespace AST
{
class Variable;
}

namespace AST
{

class LogicStatement
{
	/* Determines whether the rule can only be parsed forward, also indicate the
	 * rule is Leibniz rule */
	bool isLeibniz = false;

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
	virtual bool match(LogicStatement *matchingStatement,
					   EquivalenceUtility *matchingUtility) = 0;

	/* Called from the replace rule, creates identical copies on heap with
	 * variables cloned using id_table,
	 * at this point all nessasary variables lives in idTable and no futher
	 * information needed */
	virtual LogicStatement *replace(IDTable *idTable) = 0;

	/* A function that deep clones a logicstatement */
	virtual LogicStatement *clone() = 0;

	/* Overriding == so that it can be used in standard container library */
	virtual bool operator==(LogicStatement &other) = 0;

	/* Main parent destructor */
	virtual ~LogicStatement();

	/* Mapping from broken parts of print to its corresponding LogicStatement */
	virtual QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket) = 0;

	/* Returns true if the variable parameter is bounded, i.e. ForAll
	 * boundedVariable,
	 * ThereExists boundedVariable, or boundedVariable does not occur at all in
	 * the
	 * logicstatement */
	virtual bool variableBounded(Variable *boundedVariable) = 0;

	/* Adds variables that occur free to freeVariables, i.e. equals to
	 * freeVariable but not appear in quantifiers */
	virtual void collectFreeVariable(Variable *freeVariable,
									 QVector<Variable *> *collection) = 0;

	/* Forms a set of variables that are completely bounded, not free in any
	 * part of rootStatement */
	virtual void rejectionBoundVariables(LogicStatement *rootStatement,
										 LogicSet *rejectionSet) = 0;

	/* Used in the context of rule, infact for the Leibniz rule only */
	void setRuleType(bool isLeibnizRule);

	/* Used by rule, to determine whether a rule is only used forward for
	 * matching */
	bool isLeibnizRule();

	/* Returns a set representation of variables that lives in this AST but not
	 * in other AST */
	LogicSet *getExtraVariables(LogicStatement *other);

	/* Returns a set representation of all variables exists in the current AST
	 */
	LogicSet *getAllVariables();

	/* Returns true iff var does not appear in the statement at all */
	virtual bool notOccur(Variable *var) = 0;

	/* Every symbol must match and where a variable is logged to be occur free
	 * in one logicstatement, it occurs free
	 * at the same position in the other logicstatement with either it been the
	 * same variable or replaced by a
	 * different free variable that has been logged, returns the number of free
	 * variables replaced, or -1 if
	 * not leibniz equivalent */
	virtual int
	numberOfLeibnizReplacedVariable(LogicStatement *other,
									EquivalenceUtility *matchingUtility) = 0;

	/* Called by the main parent, note oldChildFormula must be exact reference
	 * of the parent, not copy, also please
	 * check mainparent != oldChildFormula before calling this function because
	 * in that case: the parent has been replaced
	 * and no further processing required i.e. no need to call this function */
	virtual void replaceChildStatement(LogicStatement *oldChildFormula,
									   LogicStatement *newChildFormula) = 0;

	/* Called to parse user defined rules */
	virtual void generateRule(QXmlStreamWriter *out) = 0;

	virtual QString XmlSymbol() = 0;

	virtual bool validFirstOrderStatement() = 0;
protected:
	static inline int comparePrecedence(LogicStatement *outer,
										LogicStatement *inner);
};

class Truth : public LogicStatement
{
public:
	QString print(bool);
	bool isFirstOrderLogic();
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *);
	LogicStatement *replace(IDTable *);
	LogicStatement *clone();
	bool operator==(LogicStatement &);
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool);
	bool variableBounded(Variable *);
	void collectFreeVariable(Variable *, QVector<Variable *> *);
	void rejectionBoundVariables(LogicStatement *, LogicSet *);
	bool notOccur(Variable *);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *);
	void replaceChildStatement(LogicStatement *, LogicStatement *);
	void generateRule(QXmlStreamWriter *out);
	QString XmlSymbol();
	bool validFirstOrderStatement();
};

class Falsity : public LogicStatement
{
public:
	QString print(bool);
	bool isFirstOrderLogic();
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement, EquivalenceUtility *);
	LogicStatement *replace(IDTable *);
	LogicStatement *clone();
	bool operator==(LogicStatement &);
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool);
	bool variableBounded(Variable *);
	void collectFreeVariable(Variable *, QVector<Variable *> *);
	void rejectionBoundVariables(LogicStatement *, LogicSet *);
	bool notOccur(Variable *);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *);
	void replaceChildStatement(LogicStatement *, LogicStatement *);
	void generateRule(QXmlStreamWriter *out);
	QString XmlSymbol();
	bool validFirstOrderStatement();
};

class Variable : public LogicStatement
{
	QString name;
	bool value;
	/* Used by rule to indicate variable that binds to logicstatement */
	Variable *freeVariable;
	/* Used by rule to indicate variable that binds to logicstatement */
	Variable *boundedVariable;
	/* Used by Leibneiz rule */
	Variable *notOccurVariable;
	/* Used by Leibneiz rule */
	Variable *mayOccurVariable;

public:
	Variable(QString *name);
	~Variable();
	void setName(QString *);
	QString print(bool);
	QString getName();
	bool isFirstOrderLogic();
	Symbol getSymbol();

	/* Used to give variable a boolean value */
	void setBooleanValue(bool);
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement,
			   EquivalenceUtility *matchingUtility);
	/* Called from cloned version of rule, delete this removes the cloned
	 * version which is no longer accessible
	 * because it gets replaced */
	LogicStatement *replace(IDTable *idTable);
	LogicStatement *clone();
	bool operator==(LogicStatement &);
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable,
							 QVector<Variable *> *collection);
	void setFreeVariable(QString name);
	void setBoundedVariable(QString name);
	Variable *getFreeVariable();
	Variable *getBoundedVariable();
	void rejectionBoundVariables(LogicStatement *root, LogicSet *rejectionSet);
	bool notOccur(Variable *var);
	void setMayOccurVariable(QString name);
	void setNotOccurVariable(QString name);
	Variable *getMayOccurVariable();
	Variable *getNotOccurVariable();
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *matchingUtility);
	void replaceChildStatement(LogicStatement *, LogicStatement *);
	void generateRule(QXmlStreamWriter *out);
	QString XmlSymbol();
	bool validFirstOrderStatement();
};

class UnaryOpStatement : public LogicStatement
{
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
	bool match(LogicStatement *matchingStatement,
			   EquivalenceUtility *matchingUtility);
	LogicStatement *replace(IDTable *idTable);
	virtual LogicStatement *clone() = 0;
	bool operator==(LogicStatement &);
	virtual ~UnaryOpStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable,
							 QVector<Variable *> *collection);
	void rejectionBoundVariables(LogicStatement *rootStatement,
								 LogicSet *rejectionSet);
	bool notOccur(Variable *var);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *matchingUtility);
	void replaceChildStatement(LogicStatement *oldChildFormula,
							   LogicStatement *newChildFormula);
	void generateRule(QXmlStreamWriter *out);
	virtual QString XmlSymbol() = 0;
	bool validFirstOrderStatement();

};

class NotStatement : public UnaryOpStatement
{
public:
	NotStatement(LogicStatement *);
	bool isFirstOrderLogic();
	Symbol getSymbol();
	QString symbol();
	bool evaluate();
	LogicStatement *clone();
	QString XmlSymbol();
};

class BinaryOpStatement : public LogicStatement
{
	LogicStatement *leftStatement;
	LogicStatement *rightStatement;

protected:
	void setLeftStatement(LogicStatement *);
	void setRightStatement(LogicStatement *);

public:
	virtual QString symbol() = 0;
	bool isFirstOrderLogic();
	QString print(bool);
	LogicStatement *getLeftStatement();
	LogicStatement *getRightStatement();
	Symbol getSymbol() = 0;
	bool evaluate() = 0;
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	virtual bool match(LogicStatement *matchingStatement,
					   EquivalenceUtility *matchingUtility);
	LogicStatement *replace(IDTable *idTable);
	virtual LogicStatement *clone() = 0;
	bool operator==(LogicStatement &);
	virtual ~BinaryOpStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable,
							 QVector<Variable *> *collection);
	void rejectionBoundVariables(LogicStatement *rootStatement,
								 LogicSet *rejectionSet);
	bool notOccur(Variable *var);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *matchingUtility);
	void replaceChildStatement(LogicStatement *oldChildFormula,
							   LogicStatement *newChildFormula);
	void generateRule(QXmlStreamWriter *out);
	virtual QString XmlSymbol() = 0;
	bool validFirstOrderStatement();
};

class AndStatement : public BinaryOpStatement
{
public:
	AndStatement(LogicStatement *, LogicStatement *);
	QString symbol();
	Symbol getSymbol();
	bool evaluate();
	LogicStatement *clone();
	QString XmlSymbol();
};

class OrStatement : public BinaryOpStatement
{
public:
	OrStatement(LogicStatement *, LogicStatement *);
	QString symbol();
	Symbol getSymbol();
	bool evaluate();
	LogicStatement *clone();
	QString XmlSymbol();
};

class IffStatement : public BinaryOpStatement
{
public:
	IffStatement(LogicStatement *, LogicStatement *);
	QString symbol();
	Symbol getSymbol();
	bool evaluate();
	LogicStatement *clone();
	QString XmlSymbol();
};

class ImpliesStatement : public BinaryOpStatement
{
public:
	ImpliesStatement(LogicStatement *, LogicStatement *);
	QString symbol();
	Symbol getSymbol();
	bool evaluate();
	LogicStatement *clone();
	/* Added just for Leibniz Rule */
	bool match(LogicStatement *matchingStatement,
			   EquivalenceUtility *matchingUtility);
	QString XmlSymbol();
};

class FirstOrderStatement : public LogicStatement
{
public:
	virtual QString print(bool) = 0;
	bool isFirstOrderLogic();
	virtual Symbol getSymbol() = 0;
	bool evaluate();
	virtual void collectVariables(QVector<QVector<Variable *> *> *) = 0;
	virtual bool equals(LogicStatement *) = 0;
	virtual bool match(LogicStatement *, EquivalenceUtility *) = 0;
	virtual LogicStatement *clone() = 0;
	virtual LogicStatement *replace(IDTable *) = 0;
	virtual bool operator==(LogicStatement &) = 0;
	virtual ~FirstOrderStatement();
	virtual QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool) = 0;
	virtual bool variableBounded(Variable *) = 0;
	virtual void collectFreeVariable(Variable *, QVector<Variable *> *) = 0;
	virtual bool notOccur(Variable *) = 0;
	virtual int numberOfLeibnizReplacedVariable(LogicStatement *,
												EquivalenceUtility *) = 0;
	virtual void replaceChildStatement(LogicStatement *, LogicStatement *) = 0;
	virtual void generateRule(QXmlStreamWriter *) = 0;
	virtual QString XmlSymbol() = 0;
	virtual bool validFirstOrderStatement() = 0;
};

class ForAllStatement : public FirstOrderStatement
{
	LogicStatement *statement;
	Variable *identifier;

protected:
	void setStatement(LogicStatement *);
	void setIdentifier(Variable *);

public:
	ForAllStatement(Variable *, LogicStatement *);
	QString print(bool);
	LogicStatement *getStatement();
	Variable *getQuantifier();
	Symbol getSymbol();
	~ForAllStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable,
							 QVector<Variable *> *collection);
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	LogicStatement *clone();
	bool operator==(LogicStatement &);
	void rejectionBoundVariables(LogicStatement *rootStatement,
								 LogicSet *rejectionSet);
	bool match(LogicStatement *matchingStatement,
			   EquivalenceUtility *matchingUtility);
	LogicStatement *replace(IDTable *idTable);
	bool notOccur(Variable *var);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *matchingUtility);
	void replaceChildStatement(LogicStatement *oldChildFormula,
							   LogicStatement *newChildFormula);
	QString XmlSymbol();
	void generateRule(QXmlStreamWriter *out);
	bool validFirstOrderStatement();
};

class ThereExistsStatement : public FirstOrderStatement
{
	LogicStatement *statement;
	Variable *identifier;

protected:
	void setStatement(LogicStatement *);
	void setIdentifier(Variable *);

public:
	ThereExistsStatement(Variable *, LogicStatement *);
	QString print(bool);
	LogicStatement *getStatement();
	Variable *getQuantifier();
	Symbol getSymbol();
	~ThereExistsStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable,
							 QVector<Variable *> *collection);
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	LogicStatement *clone();
	bool operator==(LogicStatement &);
	void rejectionBoundVariables(LogicStatement *rootStatement,
								 LogicSet *rejectionSet);
	bool match(LogicStatement *matchingStatement,
			   EquivalenceUtility *matchingUtility);
	LogicStatement *replace(IDTable *idTable);
	bool notOccur(Variable *var);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *matchingUtility);
	void replaceChildStatement(LogicStatement *oldChildFormula,
							   LogicStatement *newChildFormula);
	QString XmlSymbol();
	void generateRule(QXmlStreamWriter *out);
	bool validFirstOrderStatement();
};

class Parameters : public LogicStatement
{
	Variable *parameter;
	Parameters *rest;

protected:
	void setParameter(Variable *);
	void setRemainingParameters(Parameters *);

public:
	Parameters(Variable *, Parameters *);
	Variable *getParameter();
	Parameters *getRemainingParameters();
	bool isFirstOrderLogic();
	QString print(bool);
	Symbol getSymbol();
	bool evaluate();
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	bool match(LogicStatement *matchingStatement,
			   EquivalenceUtility *matchingUtility);
	LogicStatement *clone();
	LogicStatement *replace(IDTable *idTable);
	bool operator==(LogicStatement &);
	~Parameters();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable,
							 QVector<Variable *> *collection);
	void rejectionBoundVariables(LogicStatement *root, LogicSet *rejectionSet);
	bool notOccur(Variable *var);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *matchingUtility);
	void replaceChildStatement(LogicStatement *, LogicStatement *);
	QString XmlSymbol();
	void generateRule(QXmlStreamWriter *out);
	bool validFirstOrderStatement();
};

class PredicateSymbolStatement : public FirstOrderStatement
{
	Variable *predicateSymbol;
	Parameters *parameters;
	void setParameters(Parameters *);
	void setPredicateSymbol(Variable *);

public:
	PredicateSymbolStatement(Variable *, Parameters *);
	QString getPredicateSymbolName();
	Variable *getPredicateSymbol();
	Parameters *getParameters();
	QString print(bool);
	Symbol getSymbol();
	~PredicateSymbolStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable,
							 QVector<Variable *> *collection);
	void collectVariables(QVector<QVector<Variable *> *> *);
	bool equals(LogicStatement *);
	LogicStatement *clone();
	bool operator==(LogicStatement &);
	void rejectionBoundVariables(LogicStatement *root, LogicSet *rejectionSet);
	bool match(LogicStatement *matchingStatement,
			   EquivalenceUtility *matchingUtility);
	LogicStatement *replace(IDTable *idTable);
	bool notOccur(Variable *var);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *matchingUtility);
	void replaceChildStatement(LogicStatement *, LogicStatement *);
	QString XmlSymbol();
	void generateRule(QXmlStreamWriter *out);
	bool validFirstOrderStatement();
};

class EqualityStatement : public FirstOrderStatement
{
	Variable *leftVariable;
	Variable *rightVariable;
	void setLeftVariable(Variable *);
	void setRightVariable(Variable *);

public:
	EqualityStatement(Variable *, Variable *);
	Variable *getLeftVariable();
	Variable *getRightVariable();
	QString print(bool fullBracket);
	Symbol getSymbol();
	~EqualityStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket);
	bool variableBounded(Variable *boundedVariable);
	void collectFreeVariable(Variable *freeVariable,
							 QVector<Variable *> *collection);
	void collectVariables(QVector<QVector<Variable *> *> *collection);
	bool equals(LogicStatement *);
	LogicStatement *clone();
	bool operator==(LogicStatement &);
	void rejectionBoundVariables(LogicStatement *root, LogicSet *rejectionSet);
	bool match(LogicStatement *matchingStatement,
			   EquivalenceUtility *matchingUtility);
	LogicStatement *replace(IDTable *idTable);
	bool notOccur(Variable *var);
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
										EquivalenceUtility *matchingUtility);
	void replaceChildStatement(LogicStatement *, LogicStatement *);
	QString XmlSymbol();
	void generateRule(QXmlStreamWriter *out);
	bool validFirstOrderStatement();
};

LogicStatement *parse(QString expression);
}

#endif // AST_HPP
