#ifndef EQUIVALENCEENGINE_H
#define EQUIVALENCEENGINE_H

#include "equivalenceutility.hpp"
#include "solutiontabwidget.hpp"
#include "ruleengine.hpp"
#include "utility.hpp"

using namespace AST;

class EquivalenceEngine
{
  private:
	QVector<LogicSet *> *rules;
	bool ruleApplicable(LogicStatement *, LogicSet *);
	EquivalenceUtility *tryMatchRule(LogicStatement *, Rule *);
	bool isVariable(LogicStatement *statement);
	bool acceptedRenamedVariable(LogicStatement *originalFormula,
	                             Variable *userDefinedVariable);
	bool acceptedBoundedVariable(LogicStatement *originalFormula,
	                             Variable *userDefinedVariable);
	Variable *getQualifiedRenameVariable(LogicStatement *originalFormula,
	                                     Variable *toBeReplaced,
	                                     SolutionTabWidget *UI);
	Variable *getQualifiedBoundVariable(LogicStatement *originalFormula,
	                                    Variable *boundIdentifier,
	                                    SolutionTabWidget *UI);
	LogicStatement *getAnyVariable(QString &prefixMessage, Variable *suffix,
	                               SolutionTabWidget *UI,
	                               QString &errorMessage);
	LogicStatement *getAnyFormula(QString &prefixMessage, Variable *suffix,
	                              SolutionTabWidget *UI, QString &errorMessage);
	Variable *getAnyVariableCasted(QString &prefixMessage, Variable *suffix,
	                               SolutionTabWidget *UI,
	                               QString &errorMessage);
	RuleEngine *ruleEngine;

  public:
	EquivalenceEngine();
	~EquivalenceEngine();

	/* Returns a vector of matching rule sets */
	QVector<LogicSet *> *match(LogicStatement *);

	/* Returns a vector of rule that has been matched onto */
	QVector<Rule *> *getMatchedRules(LogicStatement *, LogicSet *);

	/* Input must be modifiable i.e. cloned before passed, this function will
	 * take care deleting junk originalFormula */
	LogicStatement *replaceStatement(LogicStatement *originalFormula,
	                                 Rule *baseRule, Rule *transformationRule,
	                                 SolutionTabWidget *userWindow);

	/* Function for user defined rules, note both rules must be proven to be
	 * equivalent and returns true if both are propositional
	 * and successfully added and false if already
	 * exists or is First order */
	bool addNewPropositionalEquivalence(LogicStatement *rawFormulaFrom,
	                                    LogicStatement *rawFormulaTo);
};

#endif // EQUIVALENCEENGINE_H
