#include "equivalenceengine.hpp"

EquivalenceEngine::EquivalenceEngine()
{
	ruleEngine = new RuleEngine();
	rules = ruleEngine->parseRuleXml();
}

EquivalenceEngine::~EquivalenceEngine()
{
	if (rules != nullptr && !rules->isEmpty())
		for (LogicSet *logic_set : *rules) {
			logic_set->deepDeleteContent();
			delete logic_set;
		}

	delete rules;
	delete ruleEngine;
}

EquivalenceUtility *EquivalenceEngine::tryMatchRule(LogicStatement *formula,
                                                    Rule *rule)
{
	auto matchingUtility = new EquivalenceUtility();

	if (rule->match(formula, matchingUtility))
		return matchingUtility;

	delete matchingUtility;
	return nullptr;
}

QVector<LogicSet *> *EquivalenceEngine::match(LogicStatement *input)
{

	auto relatedEquivalence = new QVector<LogicSet *>();

	for (LogicSet *ruleSet : *rules)
		if (ruleApplicable(input, ruleSet))
			relatedEquivalence->append(ruleSet);

	return relatedEquivalence;
}

QVector<Rule *> *EquivalenceEngine::getMatchedRules(LogicStatement *input,
                                                    LogicSet *ruleSet)
{
	auto matchedRules = new QVector<Rule *>();

	EquivalenceUtility *matchingResult;

	for (Rule *rule : *ruleSet->getSet()) {
		matchingResult = tryMatchRule(input, rule);
		if (matchingResult != nullptr) {
			matchedRules->append(rule);
			delete matchingResult;
		}

		if (rule->isLeibnizRule())
			break;
	}

	return matchedRules;
}

LogicStatement *EquivalenceEngine::replaceStatement(LogicStatement *formula,
                                                    Rule *baseRule,
                                                    Rule *transformationRule,
                                                    SolutionTabWidget *UI)
{
	/* A matched leibniz rule does not need further processing */
	if (baseRule->isLeibnizRule()) {
		delete formula;
		return transformationRule->clone();
	}

	EquivalenceUtility *matchingUtility = tryMatchRule(formula, baseRule);
	Variable *boundVariable = matchingUtility->getBoundVariable();
	QVector<Variable *> *freeVariableList =
	    matchingUtility->getFreeVariableList();
	IDTable *idTable = matchingUtility->getIDTable();
	LogicSet *undefinedVariableSet =
	    transformationRule->getExtraVariables(baseRule);
	LogicStatement *result;

	/* Bound Variable Yet to be decided by user */
	if (boundVariable != nullptr) {
		Variable *userdefinedVariable =
		    getQualifiedBoundVariable(formula, boundVariable, UI);
		idTable->add(boundVariable, userdefinedVariable);

		result = transformationRule->clone()->replace(idTable);
		delete userdefinedVariable;
		delete formula;
		delete matchingUtility;
		delete undefinedVariableSet;

		return result;

		/* Variable Rename */
	} else if (freeVariableList != nullptr) {

		Variable *userdefinedVariable =
		    getQualifiedRenameVariable(formula, freeVariableList->front(), UI);
		QString newName(userdefinedVariable->getName());

		for (Variable *freeVariable : *freeVariableList)
			freeVariable->setName(newName);

		delete userdefinedVariable;
		delete matchingUtility;
		delete undefinedVariableSet;

		return formula;
	}

	/* Further input for patterns required */
	if (!undefinedVariableSet->isEmpty()) {
		bool firstOrder = baseRule->isFirstOrderLogic() ||
		                  transformationRule->isFirstOrderLogic();
		Variable *identifier;

		for (LogicStatement *id : *undefinedVariableSet->getSet()) {
			identifier = dynamic_cast<Variable *>(id);

			if (!firstOrder)
				idTable->add(identifier,
				             getAnyFormula(FORMULA_REQUEST_PREFIX_MESSAGE,
				                           identifier, UI, NO_MESSAGE));
			else
				idTable->add(identifier,
				             getAnyVariable(VARIABLE_REQUEST_PREFIX_MESSAGE,
				                            identifier, UI, NO_MESSAGE));
		}
	}

	/* No further input for patterns required */
	result = transformationRule->clone()->replace(idTable);
	delete matchingUtility;
	delete formula;
	delete undefinedVariableSet;

	return result;
}

bool EquivalenceEngine::ruleApplicable(LogicStatement *input, LogicSet *ruleSet)
{
	EquivalenceUtility *matchingResult;
	QVector<Rule *> rulesMatched;

	for (Rule *rule : *ruleSet->getSet()) {
		matchingResult = tryMatchRule(input, rule);

		if (matchingResult != nullptr) {
			delete matchingResult;
			rulesMatched.append(rule);
		}

		if (rule->isLeibnizRule())
			break;
	}

	for (Rule *r : rulesMatched) {
		ruleSet->remove(r);
		ruleSet->addFront(r);
	}

	return !rulesMatched.isEmpty();
}

bool EquivalenceEngine::isVariable(LogicStatement *statement)
{
	return statement->getSymbol() == VARIABLE_SYMBOL;
}

bool EquivalenceEngine::acceptedRenamedVariable(LogicStatement *originalFormula,
                                                Variable *userDefinedVariable)
{
	return originalFormula->notOccur(userDefinedVariable);
}

bool EquivalenceEngine::acceptedBoundedVariable(LogicStatement *originalFormula,
                                                Variable *userDefinedVariable)
{
	return originalFormula->variableBounded(userDefinedVariable);
}

Variable *
EquivalenceEngine::getQualifiedRenameVariable(LogicStatement *originalFormula,
                                              Variable *toBeReplaced,
                                              SolutionTabWidget *UI)
{

	Variable *userInputVariable;
	QString errorMessage;

	while (!acceptedRenamedVariable(
	            originalFormula,
	            userInputVariable = getAnyVariableCasted(
	                RENAME_PREFIX_MESSAGE, toBeReplaced, UI, errorMessage))) {
		errorMessage = VARIABLE_MUST_NOT_OCCUR_ERROR_MESSAGE;
		delete userInputVariable;
	}

	return userInputVariable;
}

Variable *
EquivalenceEngine::getQualifiedBoundVariable(LogicStatement *originalFormula,
                                             Variable *boundIdentifier,
                                             SolutionTabWidget *UI)
{

	Variable *userInputVariable;
	QString errorMessage;

	while (!acceptedBoundedVariable(originalFormula,
	                                userInputVariable = getAnyVariableCasted(
	                                    BOUND_VARIABLE_REQUEST_PREFIX_MESSAGE,
	                                    boundIdentifier, UI, errorMessage))) {
		errorMessage = VARIABLE_OCCURS_FREE_ERROR_MESSAGE;
		delete userInputVariable;
	}

	return userInputVariable;
}

LogicStatement *EquivalenceEngine::getAnyVariable(QString &prefixMessage,
                                                  Variable *suffix,
                                                  SolutionTabWidget *UI,
                                                  QString &errorMessage)
{
	LogicStatement *userInput;

	while (!isVariable(userInput = getAnyFormula(prefixMessage, suffix, UI,
	                                             errorMessage))) {
		errorMessage = EXPECT_VARIABLE_ERROR_MESSAGE;
		delete userInput;
	}

	return userInput;
}

Variable *EquivalenceEngine::getAnyVariableCasted(QString &prefixMessage,
                                                  Variable *suffix,
                                                  SolutionTabWidget *UI,
                                                  QString &errorMessage)
{
	return dynamic_cast<Variable *>(
	    getAnyVariable(prefixMessage, suffix, UI, errorMessage));
}

LogicStatement *EquivalenceEngine::getAnyFormula(QString &prefixMessage,
                                                 Variable *suffix,
                                                 SolutionTabWidget *UI,
                                                 QString &errorMessage)
{
	return UI->getReplacement(prefixMessage, suffix, errorMessage);
}

bool EquivalenceEngine::addNewPropositionalEquivalence(
    LogicStatement *rawFormulaFrom, LogicStatement *rawFormulaTo)
{
	return ruleEngine->addRule(rawFormulaFrom, rawFormulaTo);
}
