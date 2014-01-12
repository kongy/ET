#ifndef EQUIVALENCEENGINE_H
#define EQUIVALENCEENGINE_H

#include "equivalenceutility.hpp"
#include "solutiontabwidget.hpp"
#include "ruleengine.hpp"

using namespace AST;

class EquivalenceEngine {
private:
	const QString EXPECT_VARIABLE_ERROR_MESSAGE = "Error: Expecting input of type Variable";
	const QString VARIABLE_MUST_NOT_OCCUR_ERROR_MESSAGE = "Error: Variable must not occur in the statement to be replaced";
	const QString RENAME_PREFIX_MESSAGE = "Please enter a variable to rename ";
	const QString VARIABLE_REQUEST_PREFIX_MESSAGE = "Please enter a variable for ";
	const QString FORMULA_REQUEST_PREFIX_MESSAGE = "Please enter a formula for ";
	const QString BOUND_VARIABLE_REQUEST_PREFIX_MESSAGE = "Please enter a bounded variable for ";
	const QString VARIABLE_OCCURS_FREE_ERROR_MESSAGE = "Error: User defined variable occured free in the statement to be replaced";
	const QString NONE = "";
	QVector<LogicSet *> *rules;
	bool ruleApplicable(LogicStatement *, LogicSet *);
	EquivalenceUtility *tryMatchRule(LogicStatement *, Rule *);
	bool isVariable(LogicStatement *statement);
	bool acceptedRenamedVariable(LogicStatement *originalFormula, Variable *userDefinedVariable);
	bool acceptedBoundedVariable(LogicStatement *originalFormula, Variable *userDefinedVariable);
	Variable *getQualifiedRenameVariable(LogicStatement *originalFormula, Variable *toBeReplaced, SolutionTabWidget *UI);
	Variable *getQualifiedBoundVariable(LogicStatement *originalFormula, Variable *boundIdentifier, SolutionTabWidget *UI);
	LogicStatement *getAnyVariable(const QString msg, SolutionTabWidget *UI, QString errorMessage);
	LogicStatement *getAnyFormula(const QString msg, SolutionTabWidget *UI, const QString errorMessage);
	Variable *getAnyVariableCasted(const QString msg, SolutionTabWidget *UI, QString errorMessage);
	RuleEngine *ruleEngine;
public:
	EquivalenceEngine();
	~EquivalenceEngine();

	/* Returns a vector of matching rule sets */
	QVector<LogicSet *> *match(LogicStatement *);

	/* Returns a vector of rule that has been matched onto */
	QVector<Rule *> *getMatchedRules(LogicStatement *, LogicSet *);

	/* Input must be modifiable i.e. cloned before passed, this function will take care deleting junk originalFormula */
	LogicStatement *replaceStatement(LogicStatement *originalFormula, Rule *baseRule, Rule *transformationRule, SolutionTabWidget *userWindow);

    /* Function for user defined rules, note the ruleSet must be proven to be equivalent and can only be propositional,
     * returns true if successfully added and false if already exists */
	bool addNewPropositionalEquivalence(LogicSet *ruleSet);
};

#endif // EQUIVALENCEENGINE_H
