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
	QString print(bool) override;
	bool isFirstOrderLogic() override;
	Symbol getSymbol() override;
	bool evaluate() override;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *) override;
	LogicStatement *replace(IDTable *) override;
	LogicStatement *clone() override;
	bool operator==(LogicStatement &) override;
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool) override;
	bool variableBounded(Variable *) override;
	void collectFreeVariable(Variable *, QVector<Variable *> *) override;
	void rejectionBoundVariables(LogicStatement *, LogicSet *) override;
	bool notOccur(Variable *) override;
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
	                                    EquivalenceUtility *) override;
	void replaceChildStatement(LogicStatement *, LogicStatement *) override;
	void generateRule(QXmlStreamWriter *out) override;
	QString XmlSymbol() override;
	bool validFirstOrderStatement() override;
};

class Falsity : public LogicStatement
{
  public:
	QString print(bool) override;
	bool isFirstOrderLogic() override;
	Symbol getSymbol() override;
	bool evaluate() override;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *) override;
	LogicStatement *replace(IDTable *) override;
	LogicStatement *clone() override;
	bool operator==(LogicStatement &) override;
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool) override;
	bool variableBounded(Variable *) override;
	void collectFreeVariable(Variable *, QVector<Variable *> *) override;
	void rejectionBoundVariables(LogicStatement *, LogicSet *) override;
	bool notOccur(Variable *) override;
	int numberOfLeibnizReplacedVariable(LogicStatement *other,
	                                    EquivalenceUtility *) override;
	void replaceChildStatement(LogicStatement *, LogicStatement *) override;
	void generateRule(QXmlStreamWriter *out) override;
	QString XmlSymbol() override;
	bool validFirstOrderStatement() override;
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
	QString print(bool) override;
	QString getName();
	bool isFirstOrderLogic() override;
	Symbol getSymbol() override;

	/* Used to give variable a boolean value */
	void setBooleanValue(bool);
	bool evaluate() override;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *matchingUtility) override;
	/* Called from cloned version of rule, delete this removes the cloned
	 * version which is no longer accessible
	 * because it gets replaced */
	LogicStatement *replace(IDTable *idTable) override;
	LogicStatement *clone() override;
	bool operator==(LogicStatement &) override;
	QVector<QPair<QString, LogicStatement *> > getStringMapping(bool) override;
	bool variableBounded(Variable *boundedVariable) override;
	void collectFreeVariable(Variable *freeVariable,
	                         QVector<Variable *> *collection) override;
	void setFreeVariable(QString name);
	void setBoundedVariable(QString name);
	Variable *getFreeVariable();
	Variable *getBoundedVariable();
	void rejectionBoundVariables(LogicStatement *root,
	                             LogicSet *rejectionSet) override;
	bool notOccur(Variable *var) override;
	void setMayOccurVariable(QString name);
	void setNotOccurVariable(QString name);
	Variable *getMayOccurVariable();
	Variable *getNotOccurVariable();
	int numberOfLeibnizReplacedVariable(
	    LogicStatement *other, EquivalenceUtility *matchingUtility) override;
	void replaceChildStatement(LogicStatement *, LogicStatement *) override;
	void generateRule(QXmlStreamWriter *out) override;
	QString XmlSymbol() override;
	bool validFirstOrderStatement() override;
};

class UnaryOpStatement : public LogicStatement
{
	LogicStatement *nestedStatement;

  protected:
	void setStatement(LogicStatement *);

  public:
	QString print(bool) override;
	virtual bool isFirstOrderLogic() = 0;
	LogicStatement *getStatement();
	virtual Symbol getSymbol() = 0;
	virtual QString symbol() = 0;
	bool evaluate() = 0;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *matchingUtility) override;
	LogicStatement *replace(IDTable *idTable) override;
	virtual LogicStatement *clone() = 0;
	bool operator==(LogicStatement &) override;
	virtual ~UnaryOpStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket) override;
	bool variableBounded(Variable *boundedVariable) override;
	void collectFreeVariable(Variable *freeVariable,
	                         QVector<Variable *> *collection) override;
	void rejectionBoundVariables(LogicStatement *rootStatement,
	                             LogicSet *rejectionSet) override;
	bool notOccur(Variable *var) override;
	int numberOfLeibnizReplacedVariable(
	    LogicStatement *other, EquivalenceUtility *matchingUtility) override;
	void replaceChildStatement(LogicStatement *oldChildFormula,
	                           LogicStatement *newChildFormula) override;
	void generateRule(QXmlStreamWriter *out) override;
	virtual QString XmlSymbol() = 0;
	bool validFirstOrderStatement() override;
};

class NotStatement : public UnaryOpStatement
{
  public:
	NotStatement(LogicStatement *);
	bool isFirstOrderLogic() override;
	Symbol getSymbol() override;
	QString symbol() override;
	bool evaluate() override;
	LogicStatement *clone() override;
	QString XmlSymbol() override;
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
	bool isFirstOrderLogic() override;
	QString print(bool) override;
	LogicStatement *getLeftStatement();
	LogicStatement *getRightStatement();
	Symbol getSymbol() = 0;
	bool evaluate() = 0;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	virtual bool match(LogicStatement *matchingStatement,
	                   EquivalenceUtility *matchingUtility) override;
	LogicStatement *replace(IDTable *idTable) override;
	virtual LogicStatement *clone() = 0;
	bool operator==(LogicStatement &) override;
	virtual ~BinaryOpStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket) override;
	bool variableBounded(Variable *boundedVariable) override;
	void collectFreeVariable(Variable *freeVariable,
	                         QVector<Variable *> *collection) override;
	void rejectionBoundVariables(LogicStatement *rootStatement,
	                             LogicSet *rejectionSet) override;
	bool notOccur(Variable *var) override;
	int numberOfLeibnizReplacedVariable(
	    LogicStatement *other, EquivalenceUtility *matchingUtility) override;
	void replaceChildStatement(LogicStatement *oldChildFormula,
	                           LogicStatement *newChildFormula) override;
	void generateRule(QXmlStreamWriter *out) override;
	virtual QString XmlSymbol() = 0;
	bool validFirstOrderStatement() override;
};

class AndStatement : public BinaryOpStatement
{
  public:
	AndStatement(LogicStatement *, LogicStatement *);
	QString symbol() override;
	Symbol getSymbol() override;
	bool evaluate() override;
	LogicStatement *clone() override;
	QString XmlSymbol() override;
};

class OrStatement : public BinaryOpStatement
{
  public:
	OrStatement(LogicStatement *, LogicStatement *);
	QString symbol() override;
	Symbol getSymbol() override;
	bool evaluate() override;
	LogicStatement *clone() override;
	QString XmlSymbol() override;
};

class IffStatement : public BinaryOpStatement
{
  public:
	IffStatement(LogicStatement *, LogicStatement *);
	QString symbol() override;
	Symbol getSymbol() override;
	bool evaluate() override;
	LogicStatement *clone() override;
	QString XmlSymbol() override;
};

class ImpliesStatement : public BinaryOpStatement
{
  public:
	ImpliesStatement(LogicStatement *, LogicStatement *);
	QString symbol() override;
	Symbol getSymbol() override;
	bool evaluate() override;
	LogicStatement *clone() override;
	/* Added just for Leibniz Rule */
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *matchingUtility) override;
	QString XmlSymbol() override;
};

class FirstOrderStatement : public LogicStatement
{
  public:
	virtual QString print(bool) = 0;
	bool isFirstOrderLogic() override;
	virtual Symbol getSymbol() = 0;
	bool evaluate() override;
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
	QString print(bool) override;
	LogicStatement *getStatement();
	Variable *getQuantifier();
	Symbol getSymbol() override;
	~ForAllStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket) override;
	bool variableBounded(Variable *boundedVariable) override;
	void collectFreeVariable(Variable *freeVariable,
	                         QVector<Variable *> *collection) override;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	LogicStatement *clone() override;
	bool operator==(LogicStatement &) override;
	void rejectionBoundVariables(LogicStatement *rootStatement,
	                             LogicSet *rejectionSet) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *matchingUtility) override;
	LogicStatement *replace(IDTable *idTable) override;
	bool notOccur(Variable *var) override;
	int numberOfLeibnizReplacedVariable(
	    LogicStatement *other, EquivalenceUtility *matchingUtility) override;
	void replaceChildStatement(LogicStatement *oldChildFormula,
	                           LogicStatement *newChildFormula) override;
	QString XmlSymbol() override;
	void generateRule(QXmlStreamWriter *out) override;
	bool validFirstOrderStatement() override;
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
	QString print(bool) override;
	LogicStatement *getStatement();
	Variable *getQuantifier();
	Symbol getSymbol() override;
	~ThereExistsStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket) override;
	bool variableBounded(Variable *boundedVariable) override;
	void collectFreeVariable(Variable *freeVariable,
	                         QVector<Variable *> *collection) override;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	LogicStatement *clone() override;
	bool operator==(LogicStatement &) override;
	void rejectionBoundVariables(LogicStatement *rootStatement,
	                             LogicSet *rejectionSet) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *matchingUtility) override;
	LogicStatement *replace(IDTable *idTable) override;
	bool notOccur(Variable *var) override;
	int numberOfLeibnizReplacedVariable(
	    LogicStatement *other, EquivalenceUtility *matchingUtility) override;
	void replaceChildStatement(LogicStatement *oldChildFormula,
	                           LogicStatement *newChildFormula) override;
	QString XmlSymbol() override;
	void generateRule(QXmlStreamWriter *out) override;
	bool validFirstOrderStatement() override;
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
	bool isFirstOrderLogic() override;
	QString print(bool) override;
	Symbol getSymbol() override;
	bool evaluate() override;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *matchingUtility) override;
	LogicStatement *clone() override;
	LogicStatement *replace(IDTable *idTable) override;
	bool operator==(LogicStatement &) override;
	~Parameters();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket) override;
	bool variableBounded(Variable *boundedVariable) override;
	void collectFreeVariable(Variable *freeVariable,
	                         QVector<Variable *> *collection) override;
	void rejectionBoundVariables(LogicStatement *root,
	                             LogicSet *rejectionSet) override;
	bool notOccur(Variable *var) override;
	int numberOfLeibnizReplacedVariable(
	    LogicStatement *other, EquivalenceUtility *matchingUtility) override;
	void replaceChildStatement(LogicStatement *, LogicStatement *) override;
	QString XmlSymbol() override;
	void generateRule(QXmlStreamWriter *out) override;
	bool validFirstOrderStatement() override;
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
	QString print(bool) override;
	Symbol getSymbol() override;
	~PredicateSymbolStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket) override;
	bool variableBounded(Variable *boundedVariable) override;
	void collectFreeVariable(Variable *freeVariable,
	                         QVector<Variable *> *collection) override;
	void collectVariables(QVector<QVector<Variable *> *> *) override;
	bool equals(LogicStatement *) override;
	LogicStatement *clone() override;
	bool operator==(LogicStatement &) override;
	void rejectionBoundVariables(LogicStatement *root,
	                             LogicSet *rejectionSet) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *matchingUtility) override;
	LogicStatement *replace(IDTable *idTable) override;
	bool notOccur(Variable *var) override;
	int numberOfLeibnizReplacedVariable(
	    LogicStatement *other, EquivalenceUtility *matchingUtility) override;
	void replaceChildStatement(LogicStatement *, LogicStatement *) override;
	QString XmlSymbol() override;
	void generateRule(QXmlStreamWriter *out) override;
	bool validFirstOrderStatement() override;
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
	QString print(bool fullBracket) override;
	Symbol getSymbol() override;
	~EqualityStatement();
	QVector<QPair<QString, LogicStatement *> >
	getStringMapping(bool fullBracket) override;
	bool variableBounded(Variable *boundedVariable) override;
	void collectFreeVariable(Variable *freeVariable,
	                         QVector<Variable *> *collection) override;
	void collectVariables(QVector<QVector<Variable *> *> *collection) override;
	bool equals(LogicStatement *) override;
	LogicStatement *clone() override;
	bool operator==(LogicStatement &) override;
	void rejectionBoundVariables(LogicStatement *root,
	                             LogicSet *rejectionSet) override;
	bool match(LogicStatement *matchingStatement,
	           EquivalenceUtility *matchingUtility) override;
	LogicStatement *replace(IDTable *idTable) override;
	bool notOccur(Variable *var) override;
	int numberOfLeibnizReplacedVariable(
	    LogicStatement *other, EquivalenceUtility *matchingUtility) override;
	void replaceChildStatement(LogicStatement *, LogicStatement *) override;
	QString XmlSymbol() override;
	void generateRule(QXmlStreamWriter *out) override;
	bool validFirstOrderStatement() override;
};

LogicStatement *parse(QString expression);
}

#endif // AST_HPP
